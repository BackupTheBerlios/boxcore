/** @internal
 * @file
 *
 *
 *  Created on: 27 Dec 2008
 *      Author: shaun
 */

#include "clsLegacyNotificationIcon.h"
#include <windows.h>
#include <utility>
#include <list>
#include "nid.h"

#ifndef CLSNOTIFICATIONICON_H_
#define CLSNOTIFICATIONICON_H_

namespace ShellServices
{

class NotifyIconHandler;

enum eUpdateResult {ICON_FAILURE = 0, ICON_ADDED = 1, ICON_MODIFIED = 2, ICON_DELETED = 4, ICON_SHOW = 8, ICON_HIDE = 16};

class NotificationIcon
{
public:
	NotificationIcon(LegacyNotificationIcon *p_legacyData, NotifyIconHandler *p_handler);
	virtual ~NotificationIcon();

	bool IsVisible()
	{
		return (!m_hidden && m_hIcon && m_uCallbackMessage);
	}

	bool IsIcon(std::pair<HWND, UINT> p_Test);
	eUpdateResult UpdateIcon(NID_INTERNAL &p_nid);
	eUpdateResult VersionIcon(NID_INTERNAL &p_nid);
	bool ShareIcon(NotificationIcon * const p_sharer, bool p_share);
private:
	LegacyNotificationIcon *m_legacyData;
	NotifyIconHandler *m_handler;

	HWND m_hWnd;
	UINT m_uID;
	UINT m_uCallbackMessage;
	bool m_hidden;
	HICON m_hIcon;
	HICON m_hIconOrig;
	WCHAR m_szTip[128];
	WCHAR m_szInfo[256];
	UINT m_uTimeout;
	UINT m_uVersion;
	WCHAR m_szInfoTitle[64];
	DWORD m_dwInfoFlags;
	HICON m_hBalloonIcon;
	GUID m_guidItem;

	bool m_showTip;

	NotificationIcon *m_sharedIcon;

	std::list<NotificationIcon *> m_iconSharers;

	friend class NotifyIconHandler;
};

}

#endif /* CLSNOTIFICATIONICON_H_ */
