#ifndef CLSNOTIFYICONHANDLER_H
#define CLSNOTIFYICONHANDLER_H
#include "clsShellServiceHandler.h"
#include "clsNotificationIcon.h"
#include "nid.h"
#include <list>
#include <map>
#include <utility>

namespace ShellServices
{

class NotificationIcon;

typedef std::list<NotificationIcon *> IconList;

enum eNotificationIconInfo {NI_HWND, NI_ID, NI_CALLBACKMESSAGE, NI_ICON, NI_TIP, NI_INFOTEXT, NI_INFOTITLE, NI_INFOTIMEOUT, NI_INFOFLAGS, NI_INFOICON, NI_VERSION, NI_LEGACY, NI_ENUMSIZE};

enum eTrayCallbackType {TCALLBACK_ADD, TCALLBACK_MOD, TCALLBACK_DEL};

typedef void (*NotificationIconCallback)(void *);
typedef LegacyNotificationIcon *(*LegacyNotficationIconFactory)();

typedef std::pair<HWND, UINT> iconPair_t;

class NotifyIconHandler : public ShellServiceHandler
{
public:
	NotifyIconHandler(LegacyNotficationIconFactory p_legacyFactory, bool p_enableProxy);
	virtual ~NotifyIconHandler();

	void RegisterCallback(eTrayCallbackType p_type, NotificationIconCallback p_callback);
	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);

	bool GetNotificationIconInfo(UINT p_index, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);
	bool GetNotificationIconInfo(HWND p_hWnd, UINT p_uID, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);
	bool GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);

	UINT GetTraySize();
	void CleanTray();

	NotificationIcon *LookupIcon(HWND p_hWnd, UINT p_uID);
	NotificationIcon *LookupIcon(UINT p_index);

	NotificationIcon *FindIconToShare(HICON p_icon);
	eUpdateResult DeleteIcon(HWND p_hWnd, UINT p_uID);
private:
	eUpdateResult UpdateIcon(NID_INTERNAL &p_nid);
	eUpdateResult DeleteIcon(NID_INTERNAL &p_nid);
	eUpdateResult VersionIcon(NID_INTERNAL &p_nid);
	eUpdateResult FocusIcon(NID_INTERNAL &p_nid);

	IconList m_IconList;

	LegacyNotficationIconFactory m_legacyFactory;

	NotificationIconCallback createdCallback;
	NotificationIconCallback modifiedCallback;
	NotificationIconCallback deletedCallback;

	bool m_proxyEnabled;
	std::map<UINT, iconPair_t> m_iconLookup;
	std::map<iconPair_t, UINT> m_iconLookupReverse;
	UINT currentHash;
	HWND m_proxyWnd;
	UINT m_lastPopup;
	std::pair<HWND, UINT> m_lastTarget;
	UINT m_lastMessage;

	static LRESULT CALLBACK ExtProxyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK ProxyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}

#endif // CLSNOTIFYICONHANDLER_H
