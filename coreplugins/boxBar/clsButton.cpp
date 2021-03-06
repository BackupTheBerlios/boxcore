/*
 * clsItButton.cpp
 *
 *  Created on: 16 Jan 2009
 *      Author: shaun
 */

#include "clsButton.h"
#include "clsText.h"
#include "clsIcon.h"
#include "clsFlexiSpacer.h"

namespace boxBar
{

/**
 * @page boxBarRC
 * @section boxBarButton Button
 * The button item is a nameable item. See @ref boxBarNameable for what this means.
 * The button is also a collection item, so settings for @ref boxBarCollection "Collection Items" apply.
 * The following properties can be set for buttons
 * @code boxBar.Button.ShowIcon: true @endcode
 * Sets whether the button should display an icon. The icon displayed is currently limited
 * to the icon of the blackbox executable in use.
 *
 * @code boxBar.Button.ShowText: true @endcode
 * Sets whether the button should display text. The text to be displayed is set separately.
 *
 * @code boxBar.Button.Text: boxButton @endcode
 * Sets the text that the button displays. Ignored if ShowText is false
 */

Button::Button(LPCSTR p_itemName) : Collection(false, p_itemName, 3),
		m_hasIcon(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "ShowIcon", true)),
		m_hasText(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "ShowText", true)),
		m_buttonText(s_settingsManager.AssociateString(m_pluginPrefix, p_itemName, "Text", "boxButton"))
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
	style = SN_TOOLBARBUTTON;
	PRINT(TEXT("Button"));
	spacingBorder = 3;
	addItem(new clsFlexiSpacer());
	if (m_hasIcon)
	{
		m_iconItem = new Icon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101)), 32);
		addItem(m_iconItem);
	}
	else
	{
		m_iconItem = NULL;
	}
	if (m_hasText)
	{
		m_textItem = new Text(m_buttonText.c_str(),SN_TOOLBARBUTTON,DIM_BOTH);
		addItem(m_textItem);
	}
	else
	{
		m_textItem = NULL;
	}
	addItem(new clsFlexiSpacer());
	//m_minSizeX = 100;
	readSettings();
}

Button::~Button()
{
	// TODO Auto-generated destructor stub
}

LRESULT Button::wndProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
	switch (p_msg)
	{
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		style = SN_TOOLBARBUTTONP;
		RedrawWindow(barWnd, &itemArea,NULL,RDW_INVALIDATE);
		PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
		return Item::wndProc(p_hWnd, p_msg, p_wParam, p_lParam);
	case WM_MOUSELEAVE:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
		style = SN_TOOLBARBUTTON;
		RedrawWindow(barWnd, NULL, NULL,RDW_INVALIDATE|RDW_INTERNALPAINT);
		return Item::wndProc(p_hWnd, p_msg, p_wParam, p_lParam);
	}
	return Collection::wndProc(p_hWnd, p_msg, p_wParam, p_lParam);
}

void Button::readSettings()
{
	if (m_textItem)
	{
		m_textItem->SetText(m_buttonText.c_str());
	}
	m_minSizeX = ReadInt(configFile, "boxBar.button.minSize.x:",0);
	m_minSizeY = ReadInt(configFile, "boxBar.button.minSize.y:",0);
	const char *textBuffer = ReadString(configFile, "boxBar.button.leftClick:","@bbCore.ShowMenu");
	AssignButton(textBuffer, leftClick, m_broamLeft);
	textBuffer = ReadString(configFile, "boxBar.button.rightClick:","");
	AssignButton(textBuffer, rightClick, m_broamRight);
	textBuffer = ReadString(configFile, "boxBar.button.midClick:","");
	AssignButton(textBuffer, midClick, m_broamMid);
}

}
