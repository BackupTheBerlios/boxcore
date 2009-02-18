#include "clsItem.h"
#include <commctrl.h>
#include <tchar.h>
#include <algorithm>
#include <limits.h>

#ifndef TTS_USEVISUALSTYLE
#define TTS_USEVISUALSTYLE 0x100
#endif

bool clsItem::AssignButton(LPCSTR p_data, mouseFunction & p_hook, LPCSTR & p_broamSlot)
{
	if (p_data[0] == '@')
	{
		LPSTR broamContent = new CHAR[strlen(p_data)+1];
		p_broamSlot = broamContent;
		CopyString(broamContent, p_data, strlen(p_data)+1);
		p_hook = broam;
		return true;
	}
	return false;
}

/** @brief Base constructor
  *
  * @param[in] pVertical Sets if this item is vertical or not
  *
  * Initialises all members to safe values, and sets an items vertical state as requested
  */
clsItem::clsItem(bool pVertical)
{
	vertical = pVertical;
	style = 0;
	itemArea.left = itemArea.right = itemArea.top = itemArea.bottom = 0;
	m_knowsSize = DIM_NONE;
	m_wantsStretch = DIM_NONE;
	mouseDown = false;
	m_dblClk = false;
	mouseButton = 0;
	minSizeX = 0;
	minSizeY = 0;
	m_minSizeX = 0;
	m_minSizeY = 0;
	m_maxSizeX = INT_MAX;
	m_maxSizeY = INT_MAX;

	m_broamLeft = NULL;
	m_broamLeftDbl = NULL;
	m_broamRight = NULL;
	m_broamMid = NULL;

	tipText = NULL;
	m_hasTooltip = false;
	leftClick = NULL;
	m_leftDblClick = NULL;
	rightClick = NULL;
	midClick = NULL;
	X1Click = NULL;
	X2Click = NULL;

	m_itemPrefix = NULL;

	itemAlpha = 255;

	itemBlend.BlendOp = AC_SRC_OVER;
	itemBlend.BlendFlags = 0;
	itemBlend.SourceConstantAlpha = itemAlpha;
	itemBlend.AlphaFormat = 0;

	ZeroMemory(&itemBitmapInfo, sizeof(itemBitmapInfo));
	itemBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	itemBitmapInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
	itemBitmapInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
	itemBitmapInfo.bmiHeader.biPlanes = 1;
	itemBitmapInfo.bmiHeader.biBitCount = 32;
}

/** @brief Base destructor
  *
  * Removes the tooltip, if it was set.
  */
clsItem::~clsItem()
{
	for (UINT i = 0; i < m_workers.size(); ++i)
	{
		delete m_workers[i];
	}
	delete[] tipText;
	delete[] m_broamLeft;
	delete[] m_broamLeftDbl;
	delete[] m_broamMid;
	delete[] m_broamRight;
	delete[] m_itemPrefix;
	tipText = NULL;
	ClearTooltip();
}

/** @brief Test if point is within the item
  *
  * For use on mouse events, when it is necessary to determine if an item was clicked
  */
bool clsItem::hitTest(int pX, int pY)
{
	if ((pX >= itemArea.left) && (pX <= itemArea.right) && (pY >= itemArea.top) && (pY <= itemArea.bottom))
		return true;
	else
		return false;
}

/** @brief Base moving function for items
  *
  * Moves the item without any checks. Also moves the tootip location if one is set.
  */
void clsItem::move(int pX, int pY)
{
	ClearTooltip();
	int sizeX = itemArea.right - itemArea.left;
	int sizeY = itemArea.bottom - itemArea.top;
	itemArea.left = pX;
	itemArea.right = pX + sizeX;
	itemArea.top = pY;
	itemArea.bottom = pY + sizeY;
	if (tipText)
		setTooltip();
}

/** @brief Base resizing funtion for items
  *
  * @param[in] pX The desired horizontal size
  * @param[in] pY The desired vertical size
  * @return Return the directions in which resizing actually occurred
  *
  * Resizes the item if the requested size is not negative.This allows a negative size
  * to be used to indicate that the old  size should be kept for that dimension.
  */
dimType clsItem::resize(int pX, int pY)
{
	ClearTooltip();
	dimType done = DIM_NONE;
	if (pX >= 0)
	{
		pX = std::min(std::max(pX, m_minSizeX),m_maxSizeX);
		itemArea.right = itemArea.left + pX;
		done = DIM_HORIZONTAL;
	}
	if (pY >= 0)
	{
		pY = std::min(std::max(pY, m_minSizeY),m_maxSizeY);
		itemArea.bottom = itemArea.top + pY;
		done = ((done == DIM_HORIZONTAL) ? DIM_BOTH : DIM_VERTICAL);
	}
	itemBitmapInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
	itemBitmapInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
	if (tipText)
		setTooltip();
	return done;
}

/** @brief Retrieve the size of an item
  *
  * @param[in] pDim The dimension of the size to return
  *
  * Returns the vertical or horizontal size of the item, as requested.
  */
int clsItem::getSize(dimType pDim)
{
	switch (pDim)
	{
	case DIM_HORIZONTAL:
		return itemArea.right - itemArea.left;
	case DIM_VERTICAL:
		return itemArea.bottom - itemArea.top;
	default:
		return -1;
	}
}

/** @brief Base sizing function for items
  *
  * @param[in] pSizegiven Indicates whether the size of the item has been set externally
  *
  * This base implementation sets the minimum size to 0 in both dimensions and then resizes
  * the item to that size. Calculations and sizing are only performed when the size
  * was not set externally.
  */
void clsItem::calculateSizes(bool pSizeGiven)
{
	if (!pSizeGiven)
	{
		minSizeX = m_minSizeX;
		minSizeY = m_minSizeY;
		resize(minSizeX, minSizeY);
	}
}

/** @brief Base window procedure for items
  *
  * Handles mouse button messages and runs handlers if installed when
  * a click is detected. Passes all other message to DefWindowProc
  */
LRESULT clsItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDBLCLK:
		m_dblClk = true;
	case WM_LBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_LBUTTON;
		break;
	case WM_RBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_RBUTTON;
		break;
	case WM_MBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_MBUTTON;
		break;
	case WM_XBUTTONDOWN:
		mouseDown = true;
		switch (HIWORD(wParam))
		{
		case XBUTTON1:
			mouseButton |= MK_XBUTTON1;
			break;
		case XBUTTON2:
			mouseButton |= MK_XBUTTON2;
			break;
		}
		break;
	case WM_LBUTTONUP:
		if (mouseDown && (mouseButton & MK_LBUTTON))
		{
			if (m_dblClk && m_leftDblClick)
			{
				m_leftDblClick(this, msg, wParam, lParam);
			}
			else if (leftClick)
			{
				leftClick(this, msg, wParam, lParam);
			}
			mouseButton = 0;
			mouseDown = false;
			m_dblClk = false;
		}
		break;
	case WM_RBUTTONUP:
		if (mouseDown && (mouseButton & MK_RBUTTON))
		{
			if (rightClick)
				rightClick(this, msg, wParam, lParam);
			mouseButton = 0;
			mouseDown = false;
			m_dblClk = false;
		}
		break;
	case WM_MBUTTONUP:
		if (mouseDown && (mouseButton & MK_MBUTTON))
		{
			if (midClick)
				midClick(this, msg, wParam, lParam);
			mouseButton = 0;
			mouseDown = false;
			m_dblClk = false;
		}
		break;
	case WM_XBUTTONUP:
		switch (HIWORD(wParam))
		{
		case XBUTTON1:
			if (mouseDown && (mouseButton & MK_XBUTTON1))
			{
				if (X1Click)
					X1Click(this, msg, wParam, lParam);
				mouseButton = 0;
				mouseDown = false;
				m_dblClk = false;
			}
			break;
		case XBUTTON2:
			if (mouseDown && (mouseButton & MK_XBUTTON2))
			{
				if (X2Click)
					X2Click(this, msg, wParam, lParam);
				mouseButton = 0;
				mouseDown = false;
				m_dblClk = false;
			}
			break;
		}
		break;
		case WM_NOTIFY:
		{
			NMHDR &messageHeader = *reinterpret_cast<NMHDR*>(lParam);
		switch(messageHeader.code)
		{
		case TTN_NEEDTEXT:
		{
			if (tipText) OutputDebugString(tipText);
			NMTTDISPINFO &tooltipInfo = *reinterpret_cast<NMTTDISPINFO*>(lParam);
			if (tooltipInfo.hdr.idFrom == reinterpret_cast<UINT_PTR>(this))
			{
				tooltipInfo.lpszText = tipText;
				tooltipInfo.hinst = NULL;
				return TRUE;
			}
			return FALSE;
		}
		}
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/** @brief Sets a tooltip for this item
  *
  * Tooltip is set so it is triggered anywhere in the item area. We use the @b this pointer
  * as id, so we don't need extra storage to know which tooltip is ours. When called for the first time
  * in a plugin we call initTooltips to create a tooltip window to handle requests.
  */
void clsItem::setTooltip()
{
	if (!tooltipWnd)
		initTooltips();
	if (tipText)
	{
		UINT writeLoc = 0;
		TCHAR tempTip[256];
		bool skipAmp = false;
		for (UINT i = 0; i <= _tcslen(tipText); ++i)
		{
			if (!(skipAmp && tipText[i] == TEXT('&')))
			{
				tempTip[writeLoc++] = tipText[i];
				if (!(tipText[i] == TEXT('&')))
				{
					skipAmp = false;
				}
			}
			if (!skipAmp && tipText[i] == TEXT('&'))
			{
				skipAmp = true;
			}
		}
		CopyString(tipText, tempTip, _tcslen(tipText)+1);
	}
	TOOLINFO toolInfo;
	ZeroMemory(&toolInfo, sizeof(toolInfo));
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = barWnd;
	toolInfo.uId = (UINT_PTR)this;
	toolInfo.uFlags = TTF_SUBCLASS | TTF_TRANSPARENT | TTF_PARSELINKS;
	toolInfo.rect = itemArea;
	toolInfo.hinst = hInstance;
	toolInfo.lpszText = LPSTR_TEXTCALLBACK;
	SendMessage(tooltipWnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(tooltipWnd, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	m_hasTooltip = true;
}

/** @brief Create a tooltip control for use by items
  *
  * This function creates a tooltip control window and then sets the parameter for that
  * window to values that are suitable for our use.
  * @note This is copied verbatim from bbLeanBars tooltip initalisation
  */
void clsItem::initTooltips()
{
	INITCOMMONCONTROLSEX ic;
	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ic.dwICC = ICC_BAR_CLASSES;

	InitCommonControlsEx(&ic);

	tooltipWnd = CreateWindowEx(
					 WS_EX_TOPMOST,
					 TOOLTIPS_CLASS,
					 NULL,
					 TTS_ALWAYSTIP | TTS_USEVISUALSTYLE | TTS_NOPREFIX,
					 CW_USEDEFAULT,
					 CW_USEDEFAULT,
					 CW_USEDEFAULT,
					 CW_USEDEFAULT,
					 barWnd,
					 NULL,
					 hInstance,
					 NULL);

	SetWindowPos(tooltipWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	SendMessage(tooltipWnd, TTM_SETMAXTIPWIDTH, 0, 300);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_AUTOMATIC, 300);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, 5000);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_INITIAL,  120);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_RESHOW,    60);
}

/** @brief Base drawing function for items
  *
  * @param[in,out] pContext The drawing context to use, passed on from the WM_PAINT message
  *
  * If the item has a style set, draws the stylegradient in the item area.
  */
void clsItem::draw(HDC pContext)
{
	//bool alphaDraw = false;
	if (style)
	{
		HDC internalDC;
		HBITMAP internalBitmap, origBitmap;
		if (alphaDraw)
		{
			internalDC = CreateCompatibleDC(pContext);
			internalBitmap = CreateDIBSection(internalDC, &itemBitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0);
			origBitmap = (HBITMAP) SelectObject(internalDC, internalBitmap);
			RECT tempArea;
			tempArea.left = tempArea.top = 0;
			tempArea.right = itemArea.right - itemArea.left;
			tempArea.bottom = itemArea.bottom - itemArea.top;
			MakeStyleGradient(internalDC, &tempArea, bbStyle.getStyle(style), bbStyle.getStyleBorder(style));
			msimg32.AlphaBlend(pContext, itemArea.left, itemArea.top,
							   itemArea.right - itemArea.left, itemArea.bottom - itemArea.top,
							   internalDC, 0, 0, itemArea.right - itemArea.left, itemArea.bottom - itemArea.top, itemBlend);
			SelectObject(internalDC, origBitmap);
			DeleteObject(internalBitmap);
			DeleteDC(internalDC);
		}
		else
		{
			MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(style), bbStyle.getStyleBorder(style));
		}

	}
}


/** @brief Base for reading settings from the RC file
  *
  * Items should implement this if they need to perform additional processing besides just having
  * their RCWorkers driven.
  */
void clsItem::readSettings()
{
	for (std::vector<RCWorkers::RCWorker *>::iterator i = m_workers.begin(); i != m_workers.end(); ++i)
	{
		(*i)->ReadValue();
	}
}

/** @brief Stub for config menu addition
  *
  * @param[in,out] pMenu Menu pointer to which items can be added
  *
  * This function should be implemented when an item wants to add something to the config menu.
  * Each item should handle message which these items will generate itself. This stub does nothing,
  * and is present so that items can ignore this function if they do not need it.
  */
void clsItem::configMenu(Menu *pMenu, bool p_update)
{
}

/** @brief Draws item immediately
  *
  * Retrieves a DC for the bar window and then draws the item to it. Use this
  * when an item updates, but does not move or change size.
  */
void clsItem::drawNow()
{
	HDC barDC = GetDC(barWnd);
	draw(barDC);
	ReleaseDC(barWnd, barDC);
}



clsApiLoader clsItem::bbApiLoader;
HWND clsItem::barWnd = NULL;
HWND clsItem::tooltipWnd = NULL;
CHAR clsItem::configFile[MAX_PATH] = {'\0'};
clsStyle clsItem::bbStyle;
HWND clsItem::hBlackboxWnd = NULL;
HINSTANCE clsItem::hInstance = NULL;
bool clsItem::alphaDraw = true;
LPSTR clsItem::m_pluginPrefix = NULL;

void clsItem::ClearTooltip()
{
	if (!tooltipWnd)
		initTooltips();
	TOOLINFO toolInfo;
	ZeroMemory(&toolInfo, sizeof(toolInfo));
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.uFlags = TTF_SUBCLASS | TTF_TRANSPARENT;
	toolInfo.hwnd = barWnd;
	toolInfo.uId = (UINT_PTR)this;
	toolInfo.rect = itemArea;
	toolInfo.hinst = hInstance;
	SendMessage(tooltipWnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
	m_hasTooltip = false;
}

void clsItem::broam(clsItem *p_item, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
	switch (p_msg)
	{
	case WM_LBUTTONUP:
		PostMessage(hBlackboxWnd, BB_BROADCAST, 0, reinterpret_cast<LPARAM>(p_item->m_broamLeft));
		break;
	case WM_RBUTTONUP:
		PostMessage(hBlackboxWnd, BB_BROADCAST, 0, reinterpret_cast<LPARAM>(p_item->m_broamRight));
		break;
	case WM_MBUTTONUP:
		PostMessage(hBlackboxWnd, BB_BROADCAST, 0, reinterpret_cast<LPARAM>(p_item->m_broamMid));
		break;
	}
}

LPCSTR clsItem::PluginBroam(LPSTR p_buffer, LPCSTR p_broam)
{
	sprintf(p_buffer, "@%s.%s", m_pluginPrefix, p_broam);
	return p_buffer;
}

LPCSTR clsItem::ItemBroam(LPSTR p_buffer, LPCSTR p_broam)
{
	sprintf(p_buffer, "@%s.%s.%s", m_pluginPrefix, m_itemPrefix, p_broam);
	return p_buffer;
}

LPCSTR clsItem::ItemRCKey(LPSTR p_buffer, LPCSTR p_key)
{
	sprintf(p_buffer, "%s.%s.%s:", m_pluginPrefix, m_itemPrefix, p_key);
	return p_buffer;
}



