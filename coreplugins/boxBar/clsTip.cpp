/*
 * clsTip.cpp
 *
 *  Created on: 05 Jan 2009
 *      Author: shaun
 */

#include "clsTip.h"
#include <tchar.h>
#include "../../debug/debug.h"
#include "BBApi.h"

#define BOXBAR_BALLOONDONE WM_USER+100

#ifndef NIN_BALLOONSHOW
#define NIN_BALLOONSHOW 0x0402
#endif
#ifndef NIN_BALLOONHIDE
#define NIN_BALLOONHIDE 0x0403
#endif
#ifndef NIN_BALLOONTIMEOUT
#define NIN_BALLOONTIMEOUT 0x0404
#endif
#ifndef NIN_BALLOONUSERCLICK
#define NIN_BALLOONUSERCLICK 0x0405
#endif

#undef max
LONG max(LONG p_a, LONG p_b)
{
	return ((p_a > p_b) ? p_a : p_b);
}

Tip::Tip(HINSTANCE p_instance, HWND p_hWnd, UINT p_uID, UINT p_uCallbackMessage, UINT p_uVersion, LPCSTR p_info, LPCSTR p_infoTitle, UINT p_timeout)
{
	m_instance = p_instance;
	m_iconWnd = p_hWnd;
	m_iconID = p_uID;
	m_iconCallback = p_uCallbackMessage;
	m_iconVersion = p_uVersion;
	m_info = new CHAR[strlen(p_info)+1];
	m_infoTitle = new CHAR[strlen(p_infoTitle)+1];
	m_leftDown = false;
	m_rightDown = false;
	strcpy(m_info, p_info);
	strcpy(m_infoTitle, p_infoTitle);
	m_timeout = p_timeout;
	m_notifyWindow = NULL;
	m_tipWindow = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,   // window ex-style
								 TEXT("boxBarTip"),          // window class name
								 NULL,               // window caption text
								 WS_POPUP | WS_OVERLAPPED, // window style
								 0,            // x position
								 0,            // y position
								 0,           // window width
								 0,          // window height
								 NULL,               // parent window
								 NULL,               // window menu
								 m_instance,          // hInstance of .dll
								 this                // creation data
								);
}

Tip::~Tip()
{
	delete m_info;
	delete m_infoTitle;
}

LRESULT CALLBACK Tip::realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Tip *realClass = NULL;
	switch (msg)
	{
	case WM_CREATE:
	case WM_NCCREATE:
		realClass = (Tip *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)realClass);
		break;
	default:
		realClass = (Tip *)GetWindowLongPtr(hWnd, 0);
		break;
	}
	if (realClass)
		return realClass->WndProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Tip::Timeout()
{
	KillTimer(m_tipWindow,1);
	DestroyWindow(m_tipWindow);
	if (m_notifyWindow)
		{
			PostMessage(m_notifyWindow, BOXBAR_BALLOONDONE, 0, reinterpret_cast<LPARAM>(this));
		}
	switch (m_iconVersion)
	{
	case 4:
		SendNotifyMessage(m_iconWnd, m_iconCallback, NULL, MAKELPARAM(NIN_BALLOONTIMEOUT,m_iconID));
		break;
	default:
		SendNotifyMessage(m_iconWnd, m_iconCallback, m_iconID, NIN_BALLOONTIMEOUT);
	}
}

void Tip::Click()
{
	KillTimer(m_tipWindow,1);
	DestroyWindow(m_tipWindow);
	if (m_notifyWindow)
		{
			PostMessage(m_notifyWindow, BOXBAR_BALLOONDONE, 0, reinterpret_cast<LPARAM>(this));
		}
	switch (m_iconVersion)
	{
	case 4:
		SendNotifyMessage(m_iconWnd, m_iconCallback, NULL, MAKELPARAM(NIN_BALLOONUSERCLICK,m_iconID));
		break;
	default:
		SendNotifyMessage(m_iconWnd, m_iconCallback, m_iconID, NIN_BALLOONUSERCLICK);
	}
}

bool Tip::BelongsTo(HWND p_hWnd, UINT p_uID)
{
	return ((m_iconWnd == p_hWnd) && (m_iconID == p_uID));
}

void Tip::Kill()
{
	KillTimer(m_tipWindow,1);
	DestroyWindow(m_tipWindow);
	if (m_notifyWindow)
	{
		PostMessage(m_notifyWindow, BOXBAR_BALLOONDONE, 0, reinterpret_cast<LPARAM>(this));
	}
	switch (m_iconVersion)
	{
	case 4:
		SendNotifyMessage(m_iconWnd, m_iconCallback, NULL, MAKELPARAM(NIN_BALLOONHIDE,m_iconID));
		break;
	default:
		SendNotifyMessage(m_iconWnd, m_iconCallback, m_iconID, NIN_BALLOONHIDE);
	}
}

void Tip::Position(INT p_x, INT p_y)
{
	RECT calcRect;
	GetMonitorRect(m_tipWindow, &calcRect, GETMON_FROM_WINDOW);
	int xCenter = (calcRect.right + calcRect.left)/2;
	int yCenter = (calcRect.top + calcRect.bottom)/2;
	bool left = true;
	bool top = true;
	if (p_x > xCenter)
	{
		left = false;
	}
	if (p_y > yCenter)
	{
		top = false;
	}
	GetClientRect(m_tipWindow, &calcRect);
	SetWindowPos(m_tipWindow, NULL, (left ? p_x : (p_x - calcRect.right)), (top ? p_y : (p_y - calcRect.bottom)), 0, 0,SWP_NOSIZE | SWP_NOZORDER);
}

void Tip::NotifyWindow(HWND p_notify)
{
	m_notifyWindow = p_notify;
}

LRESULT Tip::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		HDC tempDC = GetDC(hWnd);
		RECT infoRect = {0,0,0,0};
		RECT infoTitleRect = {0,0,0,0};
		LOGFONT fontDesc;
		GetObject(bbStyle.getStyleFont(SN_TOOLBAR), sizeof(fontDesc), &fontDesc);
		fontDesc.lfWeight = FW_BOLD;
		HFONT boldFont = CreateFontIndirect(&fontDesc);
		HFONT oldFont = (HFONT) SelectObject(tempDC, bbStyle.getStyleFont(SN_TOOLBAR));
		DrawTextA(tempDC,m_info,-1,&infoRect,DT_CALCRECT | DT_LEFT);
		if (m_infoTitle[0])
		{
			SelectObject(tempDC, boldFont);
			DrawTextA(tempDC,m_infoTitle,-1,&infoTitleRect,DT_CALCRECT | DT_LEFT);
		}
		SelectObject(tempDC, oldFont);
		DeleteObject(boldFont);
		SetWindowPos(hWnd, NULL, 0, 0, 6 + max((infoRect.right - infoRect.left) , (infoTitleRect.right - infoTitleRect.left)), infoRect.bottom - infoRect.top + infoTitleRect.bottom - infoTitleRect.top + 9,SWP_NOMOVE | SWP_NOZORDER);
		DeleteDC(tempDC);
	}
	return 0;
	case WM_PAINT:
	{
		RECT drawRect;
		GetClientRect(m_tipWindow, &drawRect);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		MakeStyleGradient(hdc,&drawRect,bbStyle.getStyle(SN_TOOLBAR),true);
		SetBkMode(hdc, TRANSPARENT);
		COLORREF oldColor = SetTextColor(hdc, bbStyle.getStyleTextColor(SN_TOOLBAR));
		LOGFONT fontDesc;
		GetObject(bbStyle.getStyleFont(SN_TOOLBAR), sizeof(fontDesc), &fontDesc);
		fontDesc.lfWeight = FW_BOLD;
		HFONT boldFont = CreateFontIndirect(&fontDesc);

		RECT infoRect = {3,3,3,3};
		RECT infoTitleRect = {3,3,3,3};
		if (m_infoTitle[0])
		{
			HFONT oldFont = (HFONT) SelectObject(hdc, boldFont);
			DrawTextA(hdc,m_infoTitle,-1,&infoTitleRect,DT_CALCRECT | DT_LEFT);
			DrawTextA(hdc,m_infoTitle,-1,&infoTitleRect,DT_LEFT);
			infoRect.top = infoRect.bottom = infoTitleRect.bottom + 3;
			HGDIOBJ oldPen = SelectObject(hdc, CreatePen(PS_SOLID, 1, bbStyle.getStyleTextColor(SN_TOOLBAR)));
			MoveToEx(hdc, infoTitleRect.left, infoTitleRect.bottom + 2, NULL);
			LineTo  (hdc, drawRect.right - 3, infoTitleRect.bottom + 2);
			SelectObject(hdc, oldFont);
			DeleteObject(SelectObject(hdc, oldPen));
			DeleteObject(boldFont);
		}
		HFONT oldFont = (HFONT) SelectObject(hdc, bbStyle.getStyleFont(SN_TOOLBAR));
		DrawTextA(hdc,m_info,-1,&infoRect,DT_CALCRECT | DT_LEFT);
		DrawTextA(hdc,m_info,-1,&infoRect,DT_LEFT);
		SelectObject(hdc, oldFont);
		SetTextColor(hdc, oldColor);
		EndPaint(hWnd, &ps);
	}
	return 0;
	case WM_TIMER:
		Timeout();
		return 0;
	case WM_RBUTTONDOWN:
		m_rightDown = true;
		return 0;
	case WM_RBUTTONUP:
		if (m_rightDown)
		{
			Timeout();
		}
		return 0;
	case WM_LBUTTONDOWN:
		m_leftDown = true;
		return 0;
	case WM_LBUTTONUP:
		if (m_leftDown)
		{
			Click();
		}
		return 0;
	}
	//TRACE("Message %u",msg);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Tip::Display()
{
	ShowWindow(m_tipWindow, SW_SHOW);
	SetTimer(m_tipWindow, 1, m_timeout, NULL);
	switch (m_iconVersion)
	{
	case 4:
		SendNotifyMessage(m_iconWnd, m_iconCallback, NULL, MAKELPARAM(NIN_BALLOONSHOW,m_iconID));
		break;
	default:
		SendNotifyMessage(m_iconWnd, m_iconCallback, m_iconID, NIN_BALLOONSHOW);
	}
}

clsStyle Tip::bbStyle;




