/** @internal
 * @file
 *
 *
 *  Created on: 27 Dec 2008
 *      Author: shaun
 */

#include "clsLegacyNotificationIcon.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <utility>
#include "nid.h"

#ifndef CLSNOTIFICATIONICON_H_
#define CLSNOTIFICATIONICON_H_

namespace ShellServices
{

enum eUpdateResult {ICON_ADDED = 1, ICON_MODIFIED = 2, ICON_DELETED = 4, ICON_SHOW = 8, ICON_HIDE = 16};

class NotificationIcon
{
public:
	NotificationIcon(LegacyNotificationIcon *p_legacyData);
	virtual ~NotificationIcon();

	bool IsIcon(std::pair<HWND, UINT> p_Test);
	eUpdateResult UpdateIcon(NID_INTERNAL &p_nid);
private:
	LegacyNotificationIcon *m_legacyData;

	HWND m_hWnd;
	UINT m_uID;
};

}

#endif /* CLSNOTIFICATIONICON_H_ */
