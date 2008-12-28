/*
 * clsLegacyNotificationIcon.h
 *
 *  Created on: 28 Dec 2008
 *      Author: shaun
 */

#ifndef CLSLEGACYNOTIFICATIONICON_H_
#define CLSLEGACYNOTIFICATIONICON_H_

namespace ShellServices
{

class NotificationIcon;

class LegacyNotificationIcon
{
public:
	virtual ~LegacyNotificationIcon() = 0;

	virtual void updateLegacy(NotificationIcon *p_pNotifyIcon) = 0;
	virtual void updateLegacyPrev(NotificationIcon *p_pNotifyIcon) = 0;
	virtual void updateLegacyNext(NotificationIcon *p_pNotifyIcon) = 0;
};

}

#endif /* CLSLEGACYNOTIFICATIONICON_H_ */
