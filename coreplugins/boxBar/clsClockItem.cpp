#include "clsClockItem.h"
#include <ctime>
#include <tchar.h>
#include <locale.h>

namespace Plugin_boxBar
{

clsClockItem::clsClockItem(bool pVertical): clsLabelItem(pVertical)
{
	style = SN_TOOLBARCLOCK;

	ClockTimer = getTimerID();
	readSettings();

	m_leftDblClick = TimeControlPanel;
	_tsetlocale(LC_ALL,TEXT(".ACP"));
}

clsClockItem::~clsClockItem()
{
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsClockItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TIMER:
		if (wParam == ClockTimer)
		{
			time_t systemTime;
			time(&systemTime);
			TCHAR buffer[256];
			struct tm *ltp = localtime(&systemTime);
			_tcsftime(buffer, 256, clockFormat, ltp);
			m_textItem->setText(buffer);
			drawNow();
			if (tipText)
				delete[] tipText;
			tipText = new TCHAR[256];
			_tcsftime(tipText, 256, clockTipFormat, ltp);
			setTooltip();
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			bool seconds = _tcsstr(clockFormat, TEXT("%S")) || _tcsstr(clockFormat, TEXT("%#S"));
			SetTimer(barWnd, ClockTimer, seconds ? 1100 - lt.wMilliseconds : 61000 - lt.wSecond * 1000, NULL);
			calculateSizes();
			InvalidateRect(barWnd, &itemArea, TRUE);
			PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
			return 0;
		}
		break;
	case BB_RECONFIGURE:
	{
		readSettings();
		calculateSizes();
		return 0;
	}
	}

	return clsLabelItem::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief showMenu
  *
  * @todo: document this function
  */
void clsClockItem::showMenu(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_BROADCAST, 0, (LPARAM)"@bbCore.ShowMenu");
}

#include <string>

void clsClockItem::TimeControlPanel(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BBExecute(NULL, "open", "control.exe","timedate.cpl",NULL,SW_SHOWNORMAL, true);
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void clsClockItem::readSettings()
{
	LPCSTR textBuffer;
	textBuffer = ReadString(configFile, "boxBar.clock.format:", "%#H:%M");
	CopyString(clockFormat, textBuffer, 256);
	textBuffer = ReadString(configFile, "boxBar.clock.tipformat:", "%A %d %B %Y");
	CopyString(clockTipFormat, textBuffer, 256);
	textBuffer = ReadString(configFile, "boxBar.clock.leftClick:","");
	AssignButton(textBuffer, leftClick, m_broamLeft);
	textBuffer = ReadString(configFile, "boxBar.clock.leftDblClick:","TimeControlPanel");
	AssignButton(textBuffer, m_leftDblClick, m_broamLeftDbl);
	textBuffer = ReadString(configFile, "boxBar.clock.rightClick:","@bbCore.ShowMenu");
	AssignButton(textBuffer, rightClick, m_broamRight);
	SetTimer(barWnd, ClockTimer, 1, NULL);
}

bool clsClockItem::AssignButton(LPCSTR p_data, mouseFunction &p_hook, LPCSTR &p_broamSlot)
{
	if (Item::AssignButton(p_data, p_hook, p_broamSlot))
	{
		return true;
	}
	else if (!stricmp(p_data, "TimeControlPanel"))
	{
		p_hook = TimeControlPanel;
		return true;
	}
	return false;
}

}
