#ifndef CLSNOTIFYICONHANDLER_H
#define CLSNOTIFYICONHANDLER_H
#include "clsShellServiceHandler.h"
#include "clsNotificationIcon.h"
#include "nid.h"
#include <list>

namespace ShellServices
{

class NotificationIcon;

typedef std::list<NotificationIcon *> IconList;

enum eNotificationIconInfo {NI_HWND, NI_ID, NI_CALLBACKMESSAGE, NI_ICON, NI_TIP, NI_INFOTEXT, NI_INFOTITLE, NI_INFOTIMEOUT, NI_INFOFLAGS, NI_INFOICON, NI_VERSION, NI_LEGACY};

enum eTrayCallbackType {TCALLBACK_ADD, TCALLBACK_MOD, TCALLBACK_DEL};

typedef void (*NotificationIconCallback)(void *);
typedef LegacyNotificationIcon *(*LegacyNotficationIconFactory)();

class NotifyIconHandler : public ShellServiceHandler
{
public:
	NotifyIconHandler(LegacyNotficationIconFactory p_legacyFactory);
	virtual ~NotifyIconHandler();

	void RegisterCallback(eTrayCallbackType p_type, NotificationIconCallback p_callback);
	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);

	bool GetNotificationIconInfo(UINT p_index, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);
	bool GetNotificationIconInfo(HWND p_hWnd, UINT p_uID, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);
	bool GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);

	UINT GetTraySize();

	NotificationIcon *FindIconToShare(HICON p_icon);
	eUpdateResult DeleteIcon(HWND p_hWnd, UINT p_uID);
private:
	eUpdateResult UpdateIcon(NID_INTERNAL &p_nid);
	eUpdateResult DeleteIcon(NID_INTERNAL &p_nid);
	eUpdateResult VersionIcon(NID_INTERNAL &p_nid);
	eUpdateResult FocusIcon(NID_INTERNAL &p_nid);
	NotificationIcon *LookupIcon(HWND p_hWnd, UINT p_uID);
	NotificationIcon *LookupIcon(UINT p_index);

	IconList m_IconList;

	LegacyNotficationIconFactory m_legacyFactory;

	NotificationIconCallback createdCallback;
	NotificationIconCallback modifiedCallback;
	NotificationIconCallback deletedCallback;
};

}

#endif // CLSNOTIFYICONHANDLER_H
