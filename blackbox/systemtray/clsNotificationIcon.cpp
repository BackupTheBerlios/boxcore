/*
 * clsNotificationIcon.cpp
 *
 *  Created on: 27 Dec 2008
 *      Author: shaun
 */

#include "clsNotificationIcon.h"

namespace ShellServices
{

NotificationIcon::NotificationIcon(LegacyNotificationIcon *p_legacyData)
{
	m_legacyData = p_legacyData;
	m_hWnd = NULL;
	m_uID = 0;

}

NotificationIcon::~NotificationIcon()
{
	delete m_legacyData;
}

bool NotificationIcon::IsIcon(std::pair<HWND,UINT> p_Test)
{
	return (p_Test.first == m_hWnd) && (p_Test.second == m_uID);
}

eUpdateResult NotificationIcon::UpdateIcon(NID_INTERNAL & p_nid)
{
	bool newIcon = (m_hWnd == NULL);
	bool hideIcon = false;
	bool showIcon = false;

	if (m_legacyData)
	{
		m_legacyData->updateLegacy(this);
	}
	return static_cast<eUpdateResult>((newIcon ? ICON_ADDED : ICON_MODIFIED) | (hideIcon ? ICON_HIDE : 0) | (showIcon ? ICON_SHOW : 0));
}

}
