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

enum eTrayCallbackType {TCALLBACK_ADD, TCALLBACK_MOD, TCALLBACK_DEL};

typedef void (*NotificationIconCallback)(void *);
typedef LegacyNotificationIcon *(*LegacyNotficationIconFactory)();

class NotifyIconHandler : public ShellServiceHandler
{
public:
	NotifyIconHandler(LegacyNotficationIconFactory p_legacyFactory);
	virtual ~NotifyIconHandler();

	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);
private:
	eUpdateResult UpdateIcon(NID_INTERNAL &p_nid);
	eUpdateResult DeleteIcon(NID_INTERNAL &p_nid);
	eUpdateResult DeleteIcon(HWND p_HWnd, UINT p_uID);
	NotificationIcon *LookupIcon(HWND p_hwnd, UINT p_uID);

	IconList m_IconList;

	LegacyNotficationIconFactory m_legacyFactory;

	NotificationIconCallback createdCallback;
	NotificationIconCallback modifiedCallback;
	NotificationIconCallback deletedCallback;
};

}

#endif // CLSNOTIFYICONHANDLER_H
