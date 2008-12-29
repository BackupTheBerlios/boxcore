/*
 * clsSystemTrayIcon.h
 *
 *  Created on: 29 Dec 2008
 *      Author: shaun
 */

#ifndef CLSSYSTEMTRAYICON_H_
#define CLSSYSTEMTRAYICON_H_

#include "systemtray/clsLegacyNotificationIcon.h"
#include "BBApi.h"

class SystemTrayIcon: public ShellServices::LegacyNotificationIcon
{
public:
	SystemTrayIcon();
	virtual ~SystemTrayIcon();
	virtual void updateLegacy(ShellServices::NotificationIcon *p_icon);
	virtual void updateLegacyPrev(ShellServices::NotificationIcon *p_icon);
	virtual void updateLegacyNext(ShellServices::NotificationIcon *p_icon);

	systemTray *getSystemTray()
	{
		return &m_systemTray;
	}
private:
	systemTray m_systemTray;
};

#endif /* CLSSYSTEMTRAYICON_H_ */
