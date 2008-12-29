/*
 * clsNotificationIcon.cpp
 *
 *  Created on: 27 Dec 2008
 *      Author: shaun
 */

#include "clsNotificationIcon.h"
#include "clsNotifyIconHandler.h"

namespace ShellServices
{

NotificationIcon::NotificationIcon(LegacyNotificationIcon *p_legacyData, NotifyIconHandler *p_handler)
{
	m_legacyData = p_legacyData;
	m_handler = p_handler;
	m_hWnd = NULL;
	m_uID = 0;
	m_hidden = false;

	m_sharedIcon = NULL;
}

NotificationIcon::~NotificationIcon()
{
	delete m_legacyData;
	if (!m_sharedIcon && m_hIcon)
		DestroyIcon(m_hIcon);
	if (m_hBalloonIcon)
		DestroyIcon(m_hBalloonIcon);
	for (std::list<NotificationIcon *>::iterator i = m_iconSharers.begin(); i != m_iconSharers.end(); ++i)
	{
		m_handler->DeleteIcon((*i)->m_hWnd, (*i)->m_uID);
	}
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
	if (p_nid.uFlags & NIF_MESSAGE)
	{
		m_uCallbackMessage = p_nid.uCallbackMessage;
	}
	bool useSharedIcon = false;
	if (p_nid.uFlags & NIF_STATE)
	{
		if (p_nid.dwStateMask & NIS_SHAREDICON)
		{
			if (p_nid.dwState & NIS_SHAREDICON)
			{
				useSharedIcon = true;
			}
		}
		if (p_nid.dwStateMask & NIS_HIDDEN)
		{
			if (p_nid.dwState & NIS_HIDDEN)
			{
				m_hidden = true;
			}
			else
			{
				m_hidden = false;
			}
		}
	}
	if (p_nid.uFlags & NIF_ICON)
	{
		if (m_sharedIcon)
		{
			m_sharedIcon->ShareIcon(this, false);
			m_sharedIcon = NULL;
		}
		if (useSharedIcon)
		{
			m_sharedIcon = m_handler->FindIconToShare(p_nid.hIcon);
			if (m_sharedIcon)
			{
				m_hIconOrig = p_nid.hIcon;
				m_hIcon = m_sharedIcon->m_hIcon;
				m_sharedIcon->ShareIcon(this, true);
			}
			else
			{
				return ICON_FAILURE;
			}
		}
		else
		{
			m_hIconOrig = p_nid.hIcon;
			m_hIcon = CopyIcon(p_nid.hIcon);
		}

	}
	if (p_nid.uFlags & NIF_TIP)
	{
		lstrcpyW(m_szTip, p_nid.szTip);
	}
	if (p_nid.uFlags & NIF_INFO)
	{
		lstrcpyW(m_szInfo, p_nid.szInfo);
		lstrcpyW(m_szInfoTitle, p_nid.szInfoTitle);
		m_uTimeout = p_nid.uTimeout;
		m_dwInfoFlags = p_nid.dwInfoFlags;
		if (p_nid.hBalloonIcon)
		{
			m_hBalloonIcon = CopyIcon(p_nid.hBalloonIcon);
		}
	}
	if (p_nid.uFlags & NIF_GUID)
	{
		m_guidItem = p_nid.guidItem;
	}
	if (p_nid.uFlags & NIF_SHOWTIP)
	{
		m_showTip = true;
	}
	if (m_legacyData)
	{
		m_legacyData->updateLegacy(this);
	}
	return static_cast<eUpdateResult>((newIcon ? ICON_ADDED : ICON_MODIFIED) | (hideIcon ? ICON_HIDE : 0) | (showIcon ? ICON_SHOW : 0));
}

eUpdateResult NotificationIcon::VersionIcon(NID_INTERNAL & p_nid)
{
	m_uVersion = p_nid.uVersion;
	if (m_uVersion == 4)
	{
		m_showTip = false;
	}
	else
	{
		m_showTip = true;
	}
	return ICON_MODIFIED;
}

}
