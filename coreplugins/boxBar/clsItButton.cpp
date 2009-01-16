/*
 * clsItButton.cpp
 *
 *  Created on: 16 Jan 2009
 *      Author: shaun
 */

#include "clsItButton.h"
#include "clsTextItem.h"
#include "clsFlexiSpacer.h"

ItButton::ItButton() : clsItemCollection(false)
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
	style = SN_TOOLBARBUTTON;
	PRINT(TEXT("Button"));
	spacingBorder = 3;
	addItem(new clsFlexiSpacer(false));
	m_buttonText = new clsTextItem(NULL,SN_TOOLBARBUTTON,false,DIM_BOTH);
	addItem(m_buttonText);
	addItem(new clsFlexiSpacer(false));
	//m_minSizeX = 100;
	readSettings();
}

ItButton::~ItButton()
{
	// TODO Auto-generated destructor stub
}

LRESULT ItButton::wndProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
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
		return clsItem::wndProc(p_hWnd, p_msg, p_wParam, p_lParam);
	case WM_MOUSELEAVE:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
		style = SN_TOOLBARBUTTON;
		RedrawWindow(barWnd, &itemArea,NULL,RDW_INVALIDATE);
		PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
		return clsItem::wndProc(p_hWnd, p_msg, p_wParam, p_lParam);
	}
	return clsItemCollection::wndProc(p_hWnd, p_msg, p_wParam, p_lParam);
}

void ItButton::readSettings()
{
	m_minSizeX = ReadInt(configFile, "boxBar.button.minSize.x:",0);
	m_minSizeY = ReadInt(configFile, "boxBar.button.minSize.y:",0);
	const char *textBuffer = ReadString(configFile, "boxBar.button.text:","BB");
	m_buttonText->setText(textBuffer);
	textBuffer = ReadString(configFile, "boxBar.button.leftClick:","@bbCore.ShowMenu");
	AssignButton(textBuffer, leftClick, m_broamLeft);
	textBuffer = ReadString(configFile, "boxBar.button.rightClick:","");
	AssignButton(textBuffer, rightClick, m_broamRight);
	textBuffer = ReadString(configFile, "boxBar.button.midClick:","");
	AssignButton(textBuffer, midClick, m_broamMid);
}




