/*
 * clsNotificationIcon.cpp
 *
 *  Created on: 27 Dec 2008
 *      Author: shaun
 */

#include "clsNotificationIcon.h"
#include "clsNotifyIconHandler.h"
#include <algorithm>
#include "../../debug/debug.h"

namespace ShellServices
{

NotificationIcon::NotificationIcon(LegacyNotificationIcon *p_legacyData, NotifyIconHandler *p_handler)
{
	m_legacyData = p_legacyData;
	m_handler = p_handler;
	m_hWnd = NULL;
	m_uID = 0;
	m_hidden = false;
	m_hIcon = NULL;
	m_hIconOrig = NULL;
	m_hBalloonIcon = NULL;
	ZeroMemory(&m_szTip, sizeof(m_szTip));
	ZeroMemory(&m_szInfo, sizeof(m_szInfo));
	ZeroMemory(&m_szInfoTitle, sizeof(m_szInfoTitle));

	m_showTip = false;

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
	bool showIcon = newIcon;
	m_hWnd = p_nid.hWnd;
	m_uID = p_nid.uID;
	if (p_nid.uFlags & NIF_MESSAGE)
	{
		if (!m_uCallbackMessage)
		{
			showIcon = true;
		}
		m_uCallbackMessage = p_nid.uCallbackMessage;
	}
	if (!m_uCallbackMessage)
	{
		hideIcon = true;
		showIcon = false;
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
				showIcon = false;
				hideIcon = true;
			}
			else
			{
				m_hidden = false;
				hideIcon = false;
				showIcon = true;
			}
		}
	}
	if (p_nid.uFlags & NIF_ICON)
	{
		if (!m_hIcon)
		{
			showIcon = true;
		}
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
				m_hIcon = NULL;
				m_hIconOrig = NULL;
			}
		}
		else
		{
			m_hIconOrig = p_nid.hIcon;
			m_hIcon = CopyIcon(p_nid.hIcon);
		}
	}
	if (!m_hIcon)
	{
		hideIcon = true;
		showIcon = false;
	}

	if (p_nid.uFlags & NIF_TIP)
	{
		lstrcpyW(m_szTip, p_nid.szTip);
		m_showTip = true;
	}
	if (p_nid.uFlags & NIF_INFO)
	{
		lstrcpyW(m_szInfo, p_nid.szInfo);
		OutputDebugStringW(m_szInfo);
		lstrcpyW(m_szInfoTitle, p_nid.szInfoTitle);
		OutputDebugStringW(m_szInfoTitle);
		m_uTimeout = p_nid.uTimeout;
		m_dwInfoFlags = p_nid.dwInfoFlags;
		if (m_hBalloonIcon)
		{
			DestroyIcon(m_hBalloonIcon);
			m_hBalloonIcon = NULL;
		}
		if (p_nid.hBalloonIcon)
		{
			m_hBalloonIcon = CopyIcon(p_nid.hBalloonIcon);
		}
	}
	else
	{
		ZeroMemory(&m_szInfo, sizeof(m_szInfo));
		ZeroMemory(&m_szInfoTitle, sizeof(m_szInfoTitle));
		m_uTimeout = 0;
		m_dwInfoFlags = 0;
		if (m_hBalloonIcon)
			DestroyIcon(m_hBalloonIcon);
		m_hBalloonIcon = NULL;
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

bool NotificationIcon::ShareIcon(NotificationIcon *const p_sharer, bool p_share)
{
	if (p_share)
	{
		std::list<NotificationIcon *>::iterator position = std::find(m_iconSharers.begin(), m_iconSharers.end(), p_sharer);
		if (m_iconSharers.end() != position)
		{
			return false;
		}
		else
		{
			m_iconSharers.push_back(p_sharer);
			return true;
		}
	}
	else
	{
		std::list<NotificationIcon *>::iterator position = std::find(m_iconSharers.begin(), m_iconSharers.end(), p_sharer);
		if (m_iconSharers.end() != position)
		{
			m_iconSharers.erase(position);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool NotificationIcon::IsVisible()
	{
		return (!m_hidden && m_hIcon && m_uCallbackMessage);
	}

}
