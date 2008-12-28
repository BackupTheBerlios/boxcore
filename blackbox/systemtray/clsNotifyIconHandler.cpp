#include "clsNotifyIconHandler.h"
#include "clsNotificationIcon.h"
#include "nid.h"
#include <utility>
#include <algorithm>

namespace ShellServices
{

struct SHELLTRAYDATA
{
	DWORD dwMagic; // e.g. 0x34753423;
	DWORD dwMessage;
	NOTIFYICONDATA iconData;
};

NotifyIconHandler::NotifyIconHandler(LegacyNotficationIconFactory p_legacyFactory)
{
	//ctor
}

NotifyIconHandler::~NotifyIconHandler()
{
	//dtor
}

class NotificationIconPredicate
{
public:
	NotificationIconPredicate(HWND p_hWnd, UINT p_uID):m_lookup(p_hWnd, p_uID)
	{}

	bool operator()(NotificationIcon *p_icon)
	{
		return p_icon->IsIcon(m_lookup);
	}
private:
	std::pair<HWND, UINT> m_lookup;
};

HRESULT NotifyIconHandler::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
{
	SHELLTRAYDATA *trayData = reinterpret_cast<SHELLTRAYDATA *> (p_lpData);
	NID_ANSI &ansiNid = *reinterpret_cast<NID_ANSI *> (&(trayData->iconData));
	NID_UNICODE &uniNid = *reinterpret_cast<NID_UNICODE *> (&(trayData->iconData));
	NID_INTERNAL realNid;
	ZeroMemory(&realNid, sizeof(realNid));
	realNid.cbSize = sizeof(realNid);
	realNid.hWnd = reinterpret_cast<HWND>(ansiNid.hWnd);
	realNid.uID = ansiNid.uID;
	realNid.uFlags = ansiNid.uFlags;
	switch (trayData->dwMessage)
	{
	case NIM_ADD:
	case NIM_MODIFY:
		switch (p_cbData)
		{
		case NID_VISTAA_SIZE:
			if ((ansiNid.uFlags & NIF_INFO) && (ansiNid.dwInfoFlags & NIIF_USER))
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
			break;
		case NID_VISTAW_SIZE:
			if ((uniNid.uFlags & NIF_INFO) && (uniNid.dwInfoFlags & NIIF_USER))
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
				lstrcpyW(realNid.szTip, uniNid.szTip);
			}
			if (uniNid.uFlags & NIF_STATE)
			{
				realNid.dwState = uniNid.dwState;
				realNid.dwStateMask = uniNid.dwStateMask;
			}
			if (uniNid.uFlags & NIF_INFO)
			{
				lstrcpyW(realNid.szInfo, uniNid.szInfo);
				lstrcpyW(realNid.szInfoTitle, uniNid.szInfoTitle);
				realNid.uTimeout = uniNid.uTimeout;
				realNid.dwInfoFlags = uniNid.dwInfoFlags;
			}
			if (ansiNid.uFlags & NIF_GUID)
			{
				realNid.guidItem = uniNid.guidItem;
			}
			if (ansiNid.uFlags & NIF_TIP)
			{
				lstrcpyW(realNid.szTip, uniNid.szTip);
			}
			bool result = UpdateIcon(realNid);
			if (NIM_ADD == trayData->dwMessage)
			{
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
			else
			{
				if (result & ICON_MODIFIED)
				{
					if (modifiedCallback)
					{
						switch (result ^ ICON_MODIFIED)
						{
						case ICON_SHOW:
						case ICON_HIDE:
						default:
							modifiedCallback(LookupIcon(realNid.hWnd, realNid.uID));
							return TRUE;
						}
					}
				}
			}
			break;

		}

	}
	return FALSE;
}

eUpdateResult NotifyIconHandler::UpdateIcon(NID_INTERNAL & p_nid)
{
	NotificationIconPredicate iconTest(p_nid.hWnd, p_nid.uID);
	IconList::iterator position = std::find_if(m_IconList.begin(), m_IconList.end(), iconTest);
	NotificationIcon *icon;
	if (position == m_IconList.end())
	{
		if (m_legacyFactory)
		{
		icon = new NotificationIcon(m_legacyFactory());
		}
		else
		{
			icon = new NotificationIcon(NULL);
		}
	}
	else
	{
		icon = *position;
	}
	return icon->UpdateIcon(p_nid);
}

}
