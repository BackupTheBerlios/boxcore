#include "clsNotifyIconHandler.h"
#include "clsNotificationIcon.h"
#include "nid.h"
#include <utility>
#include <algorithm>
#include <functional>
#include "../../debug/debug.h"
#include "hash.h"
#include "../../dynwinapi/clsUser32.h"

#include <tchar.h>
#include <cstdio>

namespace ShellServices
{
struct SHELLTRAYDATA
{
	DWORD dwMagic;
	DWORD dwMessage;
	NOTIFYICONDATA iconData;
};

NotifyIconHandler::NotifyIconHandler(LegacyNotficationIconFactory p_legacyFactory, bool p_enableProxy) : m_lastTarget(NULL, 0)
{
	m_legacyFactory = p_legacyFactory;
	createdCallback = NULL;
	modifiedCallback = NULL;
	deletedCallback = NULL;
	currentHash = 0xd0d0d0d0;

	m_lastPopup = 0;

	m_proxyEnabled = p_enableProxy;

	if (m_proxyEnabled)
	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof wc);
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc      = ExtProxyWndProc;
		wc.cbClsExtra = sizeof(this);
		wc.hInstance        = GetModuleHandle(NULL);
		wc.lpszClassName    = TEXT("ProxyTrayWindow");
		wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
		wc.style            = CS_DBLCLKS;

		RegisterClassEx(&wc);

		m_proxyWnd = CreateWindowEx(
						 WS_EX_TOOLWINDOW ,   // window ex-style
						 TEXT("ProxyTrayWindow"),          // window class name
						 NULL,               // window caption text
						 WS_POPUP, // window style
						 0,            // x position
						 0,            // y position
						 1,           // window width
						 1,          // window height
						 NULL,               // parent window
						 NULL,               // window menu
						 GetModuleHandle(NULL),          // hInstance of .dll
						 this                // creation data
					 );
	}
}

NotifyIconHandler::~NotifyIconHandler()
{
	if (m_proxyEnabled)
	{
		DestroyWindow(m_proxyWnd);
		m_proxyWnd = NULL;
		UnregisterClass(TEXT("ProxyTrayWindow"), GetModuleHandle(NULL));
	}
}

class NotificationIconPredicate
{
public:
	NotificationIconPredicate(HWND p_hWnd, UINT p_uID)
			:m_lookup(p_hWnd, p_uID)
	{
	}

	bool operator ()(NotificationIcon *p_icon)
	{
		return p_icon->IsIcon(m_lookup);
	}

private:
	std::pair<HWND,UINT> m_lookup;
};

HRESULT NotifyIconHandler::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
{
	SHELLTRAYDATA *trayData = reinterpret_cast<SHELLTRAYDATA*>(p_lpData);
	if (trayData->dwMagic != 0x34753423)
	{
		TRACE("Bad Magic : %d",trayData->dwMagic);
		return 0;
	}
	NID_ANSI & ansiNid = *reinterpret_cast<NID_ANSI*>(&(trayData->iconData));
	NID_UNICODE & uniNid = *reinterpret_cast<NID_UNICODE*>(&(trayData->iconData));
	NID_INTERNAL realNid;
	ZeroMemory(&realNid, sizeof(realNid));
	realNid.cbSize = sizeof (realNid);
	realNid.hWnd = reinterpret_cast<HWND>(ansiNid.hWnd);
	realNid.uID = ansiNid.uID;
	realNid.uFlags = ansiNid.uFlags;

	switch (p_cbData - offsetof(SHELLTRAYDATA, iconData))
	{
	case NID_VISTAA_SIZE:
		if ((ansiNid.uFlags & NIF_INFO) && (ansiNid.dwInfoFlags & NIIF_USER) && ansiNid.hBalloonIcon)
		{
			realNid.hBalloonIcon = reinterpret_cast<HICON>(ansiNid.hBalloonIcon);
		}
	case NID_XPA_SIZE:
		if ((!realNid.hBalloonIcon) && (ansiNid.uFlags & NIF_INFO) && (ansiNid.dwInfoFlags & NIIF_USER))
		{
			realNid.hBalloonIcon = reinterpret_cast<HICON>(ansiNid.hIcon);
		}
	case NID_2KA_SIZE:
	case NID_WIN95_SIZE:
		if (ansiNid.uFlags & NIF_MESSAGE)
		{
			realNid.uCallbackMessage = ansiNid.uCallbackMessage;
		}
		if (ansiNid.uFlags & NIF_ICON)
		{
			realNid.hIcon = reinterpret_cast<HICON>(ansiNid.hIcon);
		}
		if (ansiNid.uFlags & NIF_TIP)
		{
			MultiByteToWideChar(CP_ACP, 0, ansiNid.szTip, -1, realNid.szTip, 256);
		}
		if (ansiNid.uFlags & NIF_STATE)
		{
			realNid.dwState = ansiNid.dwState;
			realNid.dwStateMask = ansiNid.dwStateMask;
		}
		if (ansiNid.uFlags & NIF_INFO)
		{
			MultiByteToWideChar(CP_ACP, 0, ansiNid.szInfo, -1, realNid.szInfo, 256);
			MultiByteToWideChar(CP_ACP, 0, ansiNid.szInfoTitle, -1, realNid.szInfoTitle, 256);
			realNid.uTimeout = ansiNid.uTimeout;
			realNid.dwInfoFlags = ansiNid.dwInfoFlags;
		}
		if (ansiNid.uFlags & NIF_GUID)
		{
			realNid.guidItem = ansiNid.guidItem;
		}
		if (trayData->dwMessage == NIM_SETVERSION)
		{
			realNid.uVersion = ansiNid.uVersion;
		}
		break;
	default:
	case NID_VISTAW_SIZE:
		if ((uniNid.uFlags & NIF_INFO) && (uniNid.dwInfoFlags & NIIF_USER) && uniNid.hBalloonIcon)
		{
			realNid.hBalloonIcon = reinterpret_cast<HICON>(uniNid.hBalloonIcon);
		}
	case NID_XPW_SIZE:
		if ((!realNid.hBalloonIcon) && (uniNid.uFlags & NIF_INFO) && (uniNid.dwInfoFlags & NIIF_USER))
		{
			realNid.hBalloonIcon = reinterpret_cast<HICON>(uniNid.hIcon);
		}
	case NID_2KW_SIZE:
	case NID_NT_SIZE:
		if (uniNid.uFlags & NIF_MESSAGE)
		{
			realNid.uCallbackMessage = uniNid.uCallbackMessage;
		}
		if (uniNid.uFlags & NIF_ICON)
		{
			realNid.hIcon = reinterpret_cast<HICON>(uniNid.hIcon);
		}
		if (uniNid.uFlags & NIF_TIP)
		{
			wcscpy(realNid.szTip, uniNid.szTip);
		}
		if (uniNid.uFlags & NIF_STATE)
		{
			realNid.dwState = uniNid.dwState;
			realNid.dwStateMask = uniNid.dwStateMask;
		}
		if (uniNid.uFlags & NIF_INFO)
		{
			wcscpy(realNid.szInfo, uniNid.szInfo);
			wcscpy(realNid.szInfoTitle, uniNid.szInfoTitle);
			realNid.uTimeout = uniNid.uTimeout;
			realNid.dwInfoFlags = uniNid.dwInfoFlags;
		}
		if (uniNid.uFlags & NIF_GUID)
		{
			realNid.guidItem = uniNid.guidItem;
		}
		if (uniNid.uFlags & NIF_TIP)
		{
			wcscpy(realNid.szTip, uniNid.szTip);
		}
		if (trayData->dwMessage == NIM_SETVERSION)
		{
			realNid.uVersion = uniNid.uVersion;
		}
		break;
	}

	if (!IsWindow(realNid.hWnd))
	{
		PRINT("Converting to delete request");
		trayData->dwMessage = NIM_DELETE;
	}

	if (m_proxyEnabled)
	{
		iconPair_t thisPair(realNid.hWnd, realNid.uID);
		std::map<iconPair_t, UINT>::iterator thisIt = m_iconLookupReverse.find(thisPair);
		if ( thisIt != m_iconLookupReverse.end())
		{
			realNid.hWnd = m_proxyWnd;
			realNid.uID = thisIt->second;
		}
		else
		{
			IconHash(realNid.hWnd, realNid.uID, currentHash);
			m_iconLookup[currentHash] = thisPair;
			m_iconLookupReverse[thisPair] = currentHash;
			realNid.hWnd = m_proxyWnd;
			realNid.uID = currentHash;
		}
	}
	switch (trayData->dwMessage)
	{
	case NIM_ADD:
	{
		if (LookupIcon(realNid.hWnd, realNid.uID))
		{
			PRINT("Trying to add an icon that exists");
			return FALSE;
		}
		eUpdateResult result = UpdateIcon(realNid);
		if (result & ICON_ADDED)
		{
			if (createdCallback && (result & ICON_SHOW))
				createdCallback(LookupIcon(realNid.hWnd, realNid.uID));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	case NIM_MODIFY:

	{
		//if (!LookupIcon(realNid.hWnd, realNid.uID))
		//{
		//	PRINT("Trying to modify a non-existant icon");
		//	return FALSE;
		//}
		eUpdateResult result = UpdateIcon(realNid);
		if ((result & ICON_MODIFIED))
		{
			switch (result ^ (ICON_MODIFIED))
			{
			case ICON_SHOW:
				if (createdCallback)
					createdCallback(LookupIcon(realNid.hWnd, realNid.uID));
				return TRUE;
			case ICON_HIDE:
				if (deletedCallback)
					deletedCallback(LookupIcon(realNid.hWnd, realNid.uID));
				return TRUE;
			default:
				if (modifiedCallback)
					modifiedCallback(LookupIcon(realNid.hWnd, realNid.uID));
				return TRUE;
			}
		}
		else if (result & ICON_ADDED)
		{
			if (createdCallback && (result & ICON_SHOW))
				createdCallback(LookupIcon(realNid.hWnd, realNid.uID));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		break;

	}
	case NIM_SETVERSION:
	{
		PRINT("Set icon version");
		eUpdateResult result = VersionIcon(realNid);
		if (result & ICON_MODIFIED)
		{
			if (modifiedCallback)
				modifiedCallback(LookupIcon(realNid.hWnd, realNid.uID));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	case NIM_DELETE:
	{
		TRACE("Deleting icon %p %u", realNid.hWnd, realNid.uID);
		NotificationIcon * iconToDelete = LookupIcon(realNid.hWnd, realNid.uID);
		eUpdateResult result = DeleteIcon(realNid);
		if (result & ICON_DELETED)
		{
			if (deletedCallback)
				deletedCallback(iconToDelete);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		return result;
	}
	case NIM_SETFOCUS:
		PRINT("Attempt to focus icon");
		return TRUE;
		break;
	}
	return FALSE;
}

void NotifyIconHandler::RegisterCallback(eTrayCallbackType p_type, NotificationIconCallback p_callback)
{
	switch (p_type)
	{
	case TCALLBACK_ADD:
		createdCallback = p_callback;
		break;
	case TCALLBACK_MOD:
		modifiedCallback = p_callback;
		break;
	case TCALLBACK_DEL:
		deletedCallback = p_callback;
		break;
	}
}

eUpdateResult NotifyIconHandler::UpdateIcon(NID_INTERNAL & p_nid)
{
	NotificationIconPredicate iconTest(p_nid.hWnd, p_nid.uID);
	IconList::iterator position = std::find_if(m_IconList.begin(), m_IconList.end(), iconTest);
	for (IconList::iterator i = m_IconList.begin();i != m_IconList.end();++i)
	{
		if (((*i)->m_hWnd == p_nid.hWnd) && ((*i)->m_uID == p_nid.uID))
			position = i;

	}
	NotificationIcon *icon;
	if (position == m_IconList.end())
	{
		if (m_legacyFactory)
		{
			icon = new NotificationIcon(m_legacyFactory(), this);
		}
		else
		{
			icon = new NotificationIcon(NULL, this);
		}
	}
	else
	{
		icon = *position;
	}
	eUpdateResult returnVal = icon->UpdateIcon(p_nid);
	if (position == m_IconList.end())
	{
		if (returnVal & ICON_ADDED)
		{
			m_IconList.push_back(icon);
		}
		else
			delete icon;

	}
	return returnVal;
}

eUpdateResult NotifyIconHandler::DeleteIcon(NID_INTERNAL & p_nid)
{
	return DeleteIcon(p_nid.hWnd, p_nid.uID);
}

eUpdateResult NotifyIconHandler::DeleteIcon(HWND p_hWnd, UINT p_uID)
{
	NotificationIconPredicate iconTest(p_hWnd, p_uID);
	IconList::iterator position = std::find_if(m_IconList.begin(), m_IconList.end(), iconTest);
	for (IconList::iterator i = m_IconList.begin();i != m_IconList.end();++i)
	{
		if (((*i)->m_hWnd == p_hWnd) && ((*i)->m_uID == p_uID))
			position = i;
	}
	if (m_IconList.end() != position)
	{
		IconList::iterator prev =  position;
		IconList::iterator next =  position;
		NotificationIcon *nextIcon = NULL;
		next++;
		if (m_IconList.end() != next)
		{
			nextIcon = *next;
		}
		NotificationIcon *prevIcon = NULL;
		if (m_IconList.begin() != prev)
		{
			prev--;
			prevIcon = *prev;
		}
		if (nextIcon)
		{
			nextIcon->m_legacyData->updateLegacyPrev(prevIcon);
		}
		if (prevIcon)
		{
			prevIcon->m_legacyData->updateLegacyNext(nextIcon);
		}
		if (m_proxyEnabled)
		{
			iconPair_t delPair = m_iconLookup[(*position)->m_uID];
			m_iconLookup.erase((*position)->m_uID);
			m_iconLookupReverse.erase(delPair);
		}
		delete (*position);
		m_IconList.erase(position);
		return ICON_DELETED;
	}
	else
	{
		return ICON_FAILURE;
	}
}

bool NotifyIconHandler::GetNotificationIconInfo(UINT p_index, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count)
{
	NotificationIcon *icon = LookupIcon(p_index);
	if (icon)
	{
		return GetNotificationIconInfo(icon, p_return, p_info, p_count);
	}
	else
	{
		return false;
	}
}

bool NotifyIconHandler::GetNotificationIconInfo(HWND p_hWnd, UINT p_uID, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count)
{
	NotificationIcon *icon = LookupIcon(p_hWnd, p_uID);
	if (icon)
	{
		return GetNotificationIconInfo(icon, p_return, p_info, p_count);
	}
	else
	{
		return false;
	}
}

bool NotifyIconHandler::GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count)
{
	static WCHAR empty[1] = L"";
	for (UINT i = 0;i < p_count;++i)
	{
		switch (p_info[i])
		{
		case NI_CALLBACKMESSAGE:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_uCallbackMessage);
			break;
		case NI_HWND:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_hWnd);
			break;
		case NI_ICON:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_hIcon);
			break;
		case NI_ID:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_uID);
			break;
		case NI_INFOFLAGS:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_dwInfoFlags);
			break;
		case NI_INFOICON:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_hBalloonIcon);
			break;
		case NI_INFOTEXT:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_szInfo);
			break;
		case NI_INFOTIMEOUT:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_uTimeout);
			break;
		case NI_INFOTITLE:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_szInfoTitle);
			break;
		case NI_TIP:
			if (p_icon->m_showTip)
			{
				p_return[i] = reinterpret_cast<PVOID>(p_icon->m_szTip);
			}
			else
			{
				p_return[i] = empty;
			}
			break;
		case NI_VERSION:
			p_return[i] = reinterpret_cast<PVOID>(m_proxyEnabled ? 0 : p_icon->m_uVersion);
			break;
		case NI_LEGACY:
			p_return[i] = reinterpret_cast<PVOID>(p_icon->m_legacyData);
			break;
		default:
			return false;
		}
	}

	return true;
}

eUpdateResult NotifyIconHandler::FocusIcon(NID_INTERNAL & p_nid)
{
	return static_cast<eUpdateResult>(0);
}

eUpdateResult NotifyIconHandler::VersionIcon(NID_INTERNAL & p_nid)
{
	NotificationIconPredicate iconTest(p_nid.hWnd, p_nid.uID);
	IconList::iterator position = std::find_if(m_IconList.begin(), m_IconList.end(), iconTest);
	if (m_IconList.end() != position)
	{
		return (*position)->VersionIcon(p_nid);
	}
	else
	{
		return static_cast<eUpdateResult>(0);
	}
}

NotificationIcon *NotifyIconHandler::LookupIcon(HWND p_hWnd, UINT p_uID)
{
	//NotificationIconPredicate iconTest(p_hWnd, p_uID);
	IconList::iterator position = m_IconList.end();// = std::find_if(m_IconList.begin(), m_IconList.end(), iconTest);
	for (IconList::iterator i = m_IconList.begin();i != m_IconList.end();++i)
	{
		if (((*i)->m_hWnd == p_hWnd) && ((*i)->m_uID == p_uID))
			position = i;

	}
	if (m_IconList.end() != position)
	{
		return *position;
	}
	else
	{
		return NULL;
	}
}

NotificationIcon *NotifyIconHandler::FindIconToShare(HICON p_icon)
{
	for (IconList::iterator i = m_IconList.begin();i != m_IconList.end();++i)
	{
		if ((*i)->m_hIconOrig == p_icon && !((*i)->m_sharedIcon))
			return *i;

	}
	return NULL;
}

UINT NotifyIconHandler::GetTraySize()
{
	UINT numIcons = 0;
	for (IconList::iterator i = m_IconList.begin();i != m_IconList.end();++i)
	{
		if ((*i)->IsVisible())
		{
			numIcons++;
		}
	}

	return numIcons;
}

void NotifyIconHandler::CleanTray()
{
	PRINT("Clean tray requested");
	if (m_proxyEnabled)
	{
		for (iconMap_t::iterator i = m_iconLookup.begin(); i != m_iconLookup.end();++i)
		{

			if (!IsWindow(i->second.first))
			{
				DeleteIcon(i->second.first, i->second.second);
				if (deletedCallback)
					deletedCallback(i->second.first);
				CleanTray();
				break;
			}
		}
	}
	else
	{
		for (IconList::iterator i = m_IconList.begin(); i != m_IconList.end();++i)
		{
			if (!IsWindow((*i)->m_hWnd))
			{
				DeleteIcon((*i)->m_hWnd, (*i)->m_uID);
				if (deletedCallback)
					deletedCallback(*i);
				CleanTray();
				break;
			}
		}
	}
}

NotificationIcon *NotifyIconHandler::LookupIcon(UINT p_index)
{
	IconList::iterator n = m_IconList.begin();
	UINT count = -1;
	if ((*n)->IsVisible())
	{
		++count;
	}
	while (count != p_index)
	{
		n++;
		if ((*n)->IsVisible())
		{
			++count;
		}
	}
	//advance(n, p_index);
	return *n;
}

LRESULT CALLBACK NotifyIconHandler::ExtProxyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	NotifyIconHandler *owner = NULL;
	if (uMsg == WM_NCCREATE)
	{
		owner = reinterpret_cast<NotifyIconHandler *>(((CREATESTRUCT *)lParam)->lpCreateParams);
		SetClassLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(owner));
	}
	else
	{
		owner = reinterpret_cast<NotifyIconHandler *>(GetClassLongPtr(hWnd, 0));
	}

	if (owner)
	{
		return owner->ProxyWndProc(hWnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

#ifndef NIN_POPUPOPEN
#define NIN_POPUPOPEN 0x00000406
#endif
#ifndef NIN_POPUPCLOSE
#define NIN_POPUPCLOSE 0x00000407
#endif

LRESULT CALLBACK NotifyIconHandler::ProxyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT mousePos;
	GetCursorPos(&mousePos);

	if (uMsg == WM_TIMER)
	{
		if (m_lastPopup)
		{
			SendNotifyMessage(m_lastTarget.first, m_lastMessage, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPCLOSE, m_lastTarget.second));
			m_lastPopup = 0;
		}
		KillTimer(hWnd, 1);
		return TRUE;
	}
	else if (uMsg >= WM_USER)
	{
		UINT uID = wParam;
		std::map<UINT, iconPair_t>::iterator target = m_iconLookup.find(uID);
		UINT targetMsg;
		if (target != m_iconLookup.end())
		{
			targetMsg = lParam;
		}
		else
		{
			return FALSE;
		}
		HWND targetWnd = target->second.first;
		UINT targetID = target->second.second;
		NotificationIcon *targetIcon = LookupIcon(hWnd, uID);
		if (IsWindow(targetWnd))
		{
			DWORD pid;
			if ( user32.AllowSetForegroundWindow
					&& GetWindowThreadProcessId(targetWnd, &pid))
			{
				user32.AllowSetForegroundWindow(pid);
			}
			if (m_lastPopup != uID)
			{
				SendNotifyMessage(m_lastTarget.first, m_lastMessage, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPCLOSE, m_lastTarget.second));
				m_lastPopup = 0;
			}
			switch (targetIcon->m_uVersion)
			{
			case 4:
			{
				switch (targetMsg)
				{
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPCLOSE, targetID));
					m_lastPopup = 0;
					KillTimer(hWnd, 1);
					break;
				case WM_LBUTTONUP:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_USER, targetID));
					break;
				case WM_RBUTTONUP:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_CONTEXTMENU, targetID));
					break;
				case WM_MOUSEMOVE:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
					if (m_lastPopup != uID)
					{
						SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPOPEN, targetID));
						m_lastPopup = uID;
						m_lastTarget = target->second;
						m_lastMessage = uMsg;
					}
					SetTimer(hWnd, 1, 5000, NULL);
					break;
				case WM_MOUSELEAVE:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPCLOSE, targetID));
					m_lastPopup = 0;
					KillTimer(hWnd, 1);
					break;
				default:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
				}
				break;
			}
			case 3:
			{
				switch (targetMsg)
				{
				case WM_LBUTTONUP:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_USER, targetID));
					break;
				case WM_RBUTTONUP:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_CONTEXTMENU, targetID));
					break;
				default:
					SendNotifyMessage(targetWnd, uMsg, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(targetMsg, targetID));
				}
				break;
			}
			default:
				SendNotifyMessage(targetWnd, uMsg, targetID, targetMsg);
			}
			return TRUE;
		}
		else
		{
			DeleteIcon(hWnd, uID);
			return FALSE;
		}
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

}
