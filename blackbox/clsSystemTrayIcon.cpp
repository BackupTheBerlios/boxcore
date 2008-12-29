/*
 * clsSystemTrayIcon.cpp
 *
 *  Created on: 29 Dec 2008
 *      Author: shaun
 */

#include "clsSystemTrayIcon.h"
#include "systemtray/clsNotificationIcon.h"
#include "../debug/debug.h"

SystemTrayIcon::SystemTrayIcon()
{
	m_systemTray.hIcon = NULL;
	m_systemTray.hWnd = NULL;
	m_systemTray.uCallbackMessage = 0;
	m_systemTray.uID = 0;
	ZeroMemory(&m_systemTray.szTip, sizeof(m_systemTray.szTip));
	m_systemTray.pBalloon = NULL;
}

SystemTrayIcon::~SystemTrayIcon()
{
	delete m_systemTray.pBalloon;
}

void SystemTrayIcon::updateLegacy(ShellServices::NotificationIcon *p_icon)
{
	m_systemTray.hIcon = p_icon->getHIcon();
	m_systemTray.hWnd = p_icon->getHWnd();
	if (p_icon->getShowTip())
	{
		WideCharToMultiByte(CP_ACP, 0, p_icon->getSzTip(),-1,m_systemTray.szTip,sizeof(m_systemTray.szTip),NULL,NULL);
	}
	else
	{
		ZeroMemory(&m_systemTray.szTip, sizeof(m_systemTray.szTip));
	}
	m_systemTray.uCallbackMessage = p_icon->getUCallbackMessage();
	m_systemTray.uID = p_icon->getUID();
	if (p_icon->getSzInfo()[0])
	{
		if (!m_systemTray.pBalloon)
			m_systemTray.pBalloon = new systemTrayBalloon;
		m_systemTray.pBalloon->uInfoTimeout = p_icon->getUTimeout();
		m_systemTray.pBalloon->dwInfoFlags = p_icon->getDwInfoFlags();
		WideCharToMultiByte(CP_ACP, 0, p_icon->getSzInfo(),-1,m_systemTray.pBalloon->szInfo,sizeof(m_systemTray.pBalloon->szInfo),NULL,NULL);
		WideCharToMultiByte(CP_ACP, 0, p_icon->getSzInfoTitle(),-1,m_systemTray.pBalloon->szInfoTitle,sizeof(m_systemTray.pBalloon->szInfoTitle),NULL,NULL);
	}
	else
	{
		delete m_systemTray.pBalloon;
		m_systemTray.pBalloon = NULL;
	}
}

void SystemTrayIcon::updateLegacyPrev(ShellServices::NotificationIcon *p_icon)
{
}

void SystemTrayIcon::updateLegacyNext(ShellServices::NotificationIcon *p_icon)
{
}






