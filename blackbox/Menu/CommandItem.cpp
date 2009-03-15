/*
 ============================================================================

  This file is part of the bbLean source code
  Copyright � 2001-2003 The Blackbox for Windows Development Team
  Copyright � 2004 grischka

  http://bb4win.sourceforge.net/bblean
  http://sourceforge.net/projects/bb4win

 ============================================================================

  bbLean and bb4win are free software, released under the GNU General
  Public License (GPL version 2 or later), with an extension that allows
  linking of proprietary modules under a controlled interface. This means
  that plugins etc. are allowed to be released under any license the author
  wishes. For details see:

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/

#include "../BB.h"
#include "../Settings.h"
#include "../Pidl.h"
#include "../Desk.h"
#include "MenuMaker.h"
#include "Menu.h"
#include "../VolumeControl.h"

//===========================================================================
//
// CommandItem
//
//===========================================================================

CommandItem::CommandItem(const char* pszCommand, const char* pszTitle, bool isChecked)
		: MenuItem(pszTitle)
{
	m_pszCommand = new_str(pszCommand);
	m_isChecked = isChecked;
	m_ItemID = MENUITEM_ID_CI;
}

//====================
void CommandItem::Invoke(int button)
{
	if (INVOKE_LEFT & button)
	{
		m_pMenu->hide_on_click();
		if (m_pszCommand) post_command(m_pszCommand);
		else
			if (m_pidl) exec_pidl(m_pidl, NULL, NULL);
		return;
	}

	if (INVOKE_RIGHT & button)
	{
		ShowContextMenu(m_pszCommand, m_pidl);
		return;
	}

	if (INVOKE_DRAG & button)
	{
		m_pMenu->start_drag(m_pszCommand, m_pidl);
		return;
	}
}

//===========================================================================
// base class for IntegerItem / StringItem

CommandItemEx::CommandItemEx(const char *pszCommand, const char *fmt)
		: CommandItem(NULL, "", false)
{
	char buffer[1000];
	if (NULL == strchr(pszCommand, '%'))
		pszCommand = strcat(strcpy(buffer, pszCommand), fmt);
	m_pszCommand = new_str(pszCommand);
}

void CommandItemEx::next_item (UINT wParam)
{
	Menu *pm = m_pMenu->m_pParent;
	if (pm)
	{
		pm->set_focus();
		if (wParam) PostMessage(pm->m_hwnd, WM_KEYDOWN, wParam, 0);
		Active(0);
	}
}

//===========================================================================
//
// VolumeItem
//
//===========================================================================

VolumeItem::VolumeItem(const char* pszTitle): CommandItem(NULL, pszTitle, false)
{
	m_ItemID = MENUITEM_ID_CIInt;
	m_isNOP = MI_NOP_TEXT;
}

void VolumeItem::Measure(HDC hDC, SIZE *size)
{
	RECT r = { 0, 0, 0, 0 };
	char title[128];
	*title = 0;
	if (m_pszTitle && *m_pszTitle)
	{
		strcat(title, m_pszTitle);
		strcat(title, " (mute)");
	}
	else
		strcat(title, "(mute)");
	DrawText(hDC, title, -1, &r, DT_MENU_MEASURE_STANDARD);

	int is = m_pMenu->m_iconSize;
	if (-2 == is) is = MenuInfo.nIconSize;
	size->cx = max(Settings_VolumeWidth, r.right + 2) + 4 - 2 * MenuInfo.nItemIndent[is];
	size->cy = max(Settings_VolumeHeight, r.bottom + 6);
}

void VolumeItem::Mouse(HWND hwnd, UINT uMsg, DWORD wParam, DWORD lParam)
{
	int xmouse = (short)LOWORD(lParam);
	RECT r;
	GetItemRect(&r);
	InflateRect(&r, -2, -2);
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		MenuItem::Mouse(hwnd, uMsg, wParam, lParam);
		Active(2);
		if (!(wParam & MK_LBUTTON)) return;

	case WM_LBUTTONDOWN:
		volume_set(100 * (xmouse - r.left) / (r.right - r.left));
		Active(2);
		m_pMenu->redraw();
		return;

	case WM_MBUTTONDOWN:
		PostMessage(BBhwnd, BB_EXECUTE, 0, (LPARAM) "control.exe mmsys.cpl");
		return;

	case WM_RBUTTONDOWN:
		volume_mute(true);
		Active(2);
		m_pMenu->redraw();
		return;
	}
}

void VolumeItem::Paint(HDC hDC)
{
	RECT r;
	COLORREF cr0 = (COLORREF)-1;
	int vol = volume_get();
	StyleItem *style = &mStyle.MenuVolume;
	if (m_bActive && Settings_VolumeHilite)
	{
		// draw hilite bar
		GetItemRect(&r);
		StyleItem *pSI = &mStyle.MenuHilite;
		MakeStyleGradient(hDC, &r, pSI, pSI->bordered);
		style = &mStyle.MenuFrame;
	}

	// draw volume bar
	GetItemRect(&r);
	InflateRect(&r, -2, -2);
	r.right = (r.right - r.left) * vol / 100 + r.left;
	if (1 <= r.right - r.left)
		MakeStyleGradient(hDC, &r, style, style->bordered);

	// draw menu item text
	char title[128];
	if (volume_mute(false))
		if (m_pszTitle && *m_pszTitle)
			sprintf(title, "%s (mute)", m_pszTitle);
		else
			strcpy(title, "(mute)");
	else
		if (m_pszTitle && *m_pszTitle)
			sprintf(title, "%s %d%%", m_pszTitle, vol);
		else
			sprintf(title, "%d%%", vol);

	/* Eliteforce: take icon size into account */
	int iconSize = m_pMenu->m_iconSize;
	if (-2 == iconSize) iconSize = MenuInfo.nIconSize;
	GetTextRect(&r, iconSize);

	// draw text
	cr0 = SetTextColor(hDC, style->TextColor);
	BBDrawText(hDC, title, -1, &r, mStyle.MenuVolume.Justify | DT_MENU_STANDARD, style);

	// set back previous textColor
	if ((COLORREF)-1 != cr0) SetTextColor(hDC, cr0);
}

//===========================================================================
//
// IntegerItem
//
//===========================================================================

IntegerItem::IntegerItem(const char* pszCommand, int value, int minval, int maxval)
		: CommandItemEx(pszCommand, " %d")
{
	m_ItemID = MENUITEM_ID_CIInt;
	m_value = value;
	m_min = minval;
	m_max = maxval;
	oldsize = direction = 0;
	offvalue = 0x7FFFFFFF;
	offstring = NULL;
}

void IntegerItem::Measure(HDC hDC, SIZE *size)
{
	char buf[128], val[128];
	sprintf(val, offstring && offvalue == m_value ? NLS1(offstring) : "%d", m_value);
	sprintf(buf, "%c %s %c",
			m_value == m_min ? ' ': '-',
			val,
			m_value == m_max ? ' ': '+'
		   );
	replace_str(&m_pszTitle, buf);

	MenuItem::Measure(hDC, size);
	if (size->cx > oldsize+5 || size->cx < oldsize-5)
		oldsize = size->cx;
	size->cx = oldsize + 5;
}

//====================

void IntegerItem::Mouse(HWND hwnd, UINT uMsg, DWORD wParam, DWORD lParam)
{
	if (false == Settings_menusBroamMode)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		{
			Active(2);
			int xmouse = (short)LOWORD(lParam);
			int xwidth = m_pMenu->m_width;
			direction = (xmouse < xwidth / 2) ? -1 : 1;
			m_pMenu->m_captureflg |= MENU_CAPT_TWEAKINT;
			SetCapture(hwnd);
			m_count = 0;
			ItemTimer(MENU_INTITEM_TIMER);
		}
		return;

		case WM_LBUTTONUP:
			if (direction)
			{
				direction = 0;
				Invoke(INVOKE_LEFT);
			}
			return;
		}
	}
	MenuItem::Mouse(hwnd, uMsg, wParam, lParam);
}

void IntegerItem::set_next_value(void)
{
	int value = m_value;
	int dir = direction;
	if (//m_count >= 12 ||
		GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		dir *= 10;
		int mod = value % dir;
		if (mod>0 && dir<0) mod+=dir;
		value -= mod;
	}
	m_value = iminmax(value + dir, m_min, m_max);
	m_pMenu->redraw();
}

void IntegerItem::ItemTimer(UINT nTimer)
{
	if (MENU_INTITEM_TIMER != nTimer)
	{
		MenuItem::ItemTimer(nTimer);
		return;
	}
	HWND hwnd = m_pMenu->m_hwnd;
	if (0 == direction || GetCapture() != hwnd)
	{
		KillTimer(hwnd, nTimer);
		return;
	}
	set_next_value();
	SetTimer(hwnd, MENU_INTITEM_TIMER, 0 == m_count ? 320 : 80, NULL);
	++m_count;
}

void IntegerItem::Key(UINT nMsg, WPARAM wParam)
{
	if (WM_KEYDOWN == nMsg)
	{
		if (VK_LEFT == wParam)
		{
			direction = -1;
			set_next_value();
		}
		else
			if (VK_RIGHT == wParam)
			{
				direction = 1;
				set_next_value();
			}
			else
				if (VK_UP == wParam || VK_DOWN==wParam)
				{
					next_item(wParam);
				}
	}
	else
		if (WM_KEYUP == nMsg)
		{
			if (VK_LEFT == wParam || VK_RIGHT == wParam)
			{
				if (direction)
				{
					Invoke(INVOKE_LEFT);
					direction = 0;
				}
			}
		}
		else
			if (WM_CHAR == nMsg && (VK_ESCAPE == wParam || VK_RETURN==wParam))
			{
				next_item(0);
			}

}

void IntegerItem::Invoke(int button)
{
	if (INVOKE_LEFT & button)
	{
		if (Settings_menusBroamMode) return;
		char result[1024];
		sprintf(result, m_pszCommand, m_value);
		post_command(result);
	}
}

//===========================================================================
//
// StringItem
//
//===========================================================================

StringItem::StringItem(const char* pszCommand, const char *init_string)
		: CommandItemEx(pszCommand, " %s")
{
	if (init_string) replace_str(&m_pszTitle, init_string);
	m_ItemID = MENUITEM_ID_CIStr;
	hText = NULL;
}

StringItem::~StringItem()
{
	if (hText) DestroyWindow(hText);
}

void StringItem::Paint(HDC hDC)
{
	if (false == Settings_menusBroamMode)
		m_bActive = m_pMenu->m_bHasFocus && 0 == m_isNOP;
	MenuItem::Paint(hDC);
}

void StringItem::Measure(HDC hDC, SIZE *size)
{
	if (Settings_menusBroamMode)
	{
		MenuItem::Measure(hDC, size);
		return;
	}
	MenuItem::Measure(hDC, size);
	size->cx = imax(size->cx + 20, 120);
	size->cy += 6;
}

void StringItem::Key(UINT nMsg, WPARAM wParam)
{
	if (WM_KEYDOWN == nMsg && (VK_UP == wParam || VK_DOWN == wParam))
	{
		next_item(wParam);
	}
	else
		if (WM_CHAR == nMsg && wParam==VK_ESCAPE)
		{
			SetWindowText(hText, m_pszTitle);
			next_item(0);
		}
}

void StringItem::Invoke(int button)
{
	if (INVOKE_LEFT & button)
	{
		if (Settings_menusBroamMode) return;

		char buff[1024];
		GetWindowText(hText, buff, sizeof buff);
		replace_str(&m_pszTitle, buff);
		SendMessage(hText, EM_SETMODIFY, FALSE, 0);
		if (NULL == m_pMenu->m_IDString)
			SetWindowText(hText, m_pszTitle);

		char result[1024];
		sprintf(result, m_pszCommand, buff);
		post_command(result);
	}
}

//===========================================================================
void StringItem::SetPosition(void)
{
	if (Settings_menusBroamMode)
	{
		if (hText) DestroyWindow(hText);
		return;
	}

	if (NULL == hText)
	{
		hText = CreateWindow(
					TEXT("EDIT"),
					NULL,
					WS_CHILD
					| ES_AUTOHSCROLL
					| ES_MULTILINE,
					0, 0, 0, 0,
					m_pMenu->m_hwnd,
					(HMENU)1234,
					hMainInstance,
					NULL
				);

		SetWindowLongPtr(hText, GWLP_USERDATA, (LONG_PTR)this);
		wpEditProc = (WNDPROC)SetWindowLongPtr(hText, GWLP_WNDPROC, (LONG_PTR)EditProc);
		SetWindowText(hText, m_pszTitle);
		int n = GetWindowTextLength(hText);
		PostMessage(hText, EM_SETSEL, n, n);
		PostMessage(hText, EM_SCROLLCARET, 0, 0);
	}

	HFONT hFont = MenuInfo.hFrameFont;
	SendMessage(hText, WM_SETFONT, (WPARAM)hFont, 0);

	int is = m_pMenu->m_iconSize;
	if (-2 == is) is = MenuInfo.nIconSize;
	RECT r;
	GetTextRect(&r, is);
	int h = r.bottom - r.top - 4;
	int w = r.right - r.left + 2;
	SetWindowPos(hText, NULL, r.left-1, r.top+2, w, h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	int padd = imax(0, (h - get_fontheight(hFont)) / 2);
	r.left  = padd+2;
	r.right = w - (padd+2);
	r.top   = padd;
	r.bottom = h - padd;
	SendMessage(hText, EM_SETRECT, 0, (LPARAM)&r);
}

//===========================================================================

LRESULT CALLBACK StringItem::EditProc(HWND hText, UINT msg, WPARAM wParam, LPARAM lParam)
{
	StringItem *pStringItem = (StringItem*)GetWindowLongPtr(hText, GWLP_USERDATA);
	switch (msg)
	{
		// --------------------------------------------------------
		// Send Result

	case WM_MOUSEMOVE:
		PostMessage(GetParent(hText), WM_MOUSEMOVE, wParam, MAKELPARAM(10, pStringItem->m_nTop+2));
		break;

		// --------------------------------------------------------
		// Key Intercept

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
		case VK_UP:
			pStringItem->Key(msg, wParam);
			return 0;
		}
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case VK_RETURN:
			pStringItem->Invoke(INVOKE_LEFT);
			pStringItem->next_item(0);
			return 0;

		case VK_ESCAPE:
			pStringItem->Key(msg, wParam);

		case VK_TAB:
			return 0;

		case 'A' - 0x40: // ctrl-A: select all
			SendMessage(hText, EM_SETSEL, 0, GetWindowTextLength(hText));
			return 0;
		}
		break;

		// --------------------------------------------------------
		// Paint

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hText, &ps);
		RECT r;
		GetClientRect(hText, &r);
		HDC buf = CreateCompatibleDC(hdc);
		HGDIOBJ oldbuf = SelectObject(buf, CreateCompatibleBitmap(hdc, r.right, r.bottom));

		StyleItem *pSI = &mStyle.MenuFrame;
		MakeGradient(buf, r, pSI->type, pSI->Color, pSI->ColorTo, pSI->interlaced, BEVEL_SUNKEN, BEVEL1, 0, 0, 0);
		CallWindowProc(pStringItem->wpEditProc, hText, msg, (WPARAM)buf, lParam);

		BitBltRect(hdc, buf, &ps.rcPaint);
		DeleteObject(SelectObject(buf, oldbuf));
		DeleteDC(buf);
		EndPaint(hText, &ps);
		return 0;
	}

	case WM_ERASEBKGND:
		return TRUE;

	case WM_DESTROY:
		pStringItem->hText = NULL;
		break;

	case WM_SETFOCUS:
		pStringItem->m_pMenu->m_bHasFocus = true;
#ifdef CHECKFOCUS
		dbg_printf("WM_SETFOCUS %s", "edit");
		InvalidateRect(pStringItem->m_pMenu->m_hwnd, NULL, FALSE);
#endif
		break;

	case WM_KILLFOCUS:
		if (SendMessage(hText, EM_GETMODIFY, 0, 0))
			pStringItem->Invoke(INVOKE_LEFT);

		pStringItem->Active(0);

		pStringItem->m_pMenu->m_bHasFocus = false;
#ifdef CHECKFOCUS
		dbg_printf("WM_KILLFOCUS %s", "edit");
		InvalidateRect(pStringItem->m_pMenu->m_hwnd, NULL, FALSE);
#endif
		break;
	}
	return CallWindowProc(pStringItem->wpEditProc, hText, msg, wParam, lParam);
}

//===========================================================================
