#include "clsClock.h"
#include <ctime>
#include <tchar.h>
#include <locale.h>

namespace boxBar
{

/**
 * @page boxBarRC
 * @section boxBarClock Clock
 * The clock item adds a digital clock label to your bar. It also has a tooltip which can display
 * additional information, such as the date.
 * The clock is a collection item, so settings for @ref boxBarCollection "Collection Items" apply.
 * The following properties can be set for the clock
 * @code boxBar.Clock.Format: %#H:%M @endcode
 * This sets the format and text of the normal time display for the clock
 *
 * @code boxBar.Clock.TipFormat: b%A %d %B %Y @endcode
 * This sets the format and additional text for the tooltip
 */

Clock::Clock():
		Label(false, "Clock"),
		clockFormat(s_settingsManager.AssociateString(m_pluginPrefix, m_itemPrefix, "Format", "%#H:%M")),
		clockTipFormat(s_settingsManager.AssociateString(m_pluginPrefix, m_itemPrefix, "TipFormat", "%A %d %B %Y"))
{
	SetStyle(SN_TOOLBARCLOCK);

	ClockTimer = getTimerID();
	readSettings();

	m_leftDblClick = TimeControlPanel;
	_tsetlocale(LC_ALL,TEXT(".ACP"));
}

Clock::~Clock()
{
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT Clock::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			_tcsftime(buffer, 256, clockFormat.c_str(), ltp);
			m_textItem->setText(buffer);
			if (tipText)
				delete[] tipText;
			tipText = new TCHAR[256];
			_tcsftime(tipText, 256, clockTipFormat.c_str(), ltp);
			setTooltip();
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			bool seconds = _tcsstr(clockFormat.c_str(), TEXT("%S")) || _tcsstr(clockFormat.c_str(), TEXT("%#S"));
			SetTimer(barWnd, ClockTimer, seconds ? 1100 - lt.wMilliseconds : 61000 - lt.wSecond * 1000, NULL);
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

	return Label::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief showMenu
  *
  * @todo: document this function
  */
void Clock::showMenu(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_BROADCAST, 0, (LPARAM)"@bbCore.ShowMenu");
}

#include <string>

void Clock::TimeControlPanel(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BBExecute(NULL, "open", "control.exe","timedate.cpl",NULL,SW_SHOWNORMAL, true);
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void Clock::readSettings()
{
	LPCSTR textBuffer;
	textBuffer = ReadString(configFile, "boxBar.clock.leftClick:","");
	AssignButton(textBuffer, leftClick, m_broamLeft);
	textBuffer = ReadString(configFile, "boxBar.clock.leftDblClick:","TimeControlPanel");
	AssignButton(textBuffer, m_leftDblClick, m_broamLeftDbl);
	textBuffer = ReadString(configFile, "boxBar.clock.rightClick:","@bbCore.ShowMenu");
	AssignButton(textBuffer, rightClick, m_broamRight);
	SetTimer(barWnd, ClockTimer, 1, NULL);
}

bool Clock::AssignButton(LPCSTR p_data, mouseFunction &p_hook, LPCSTR &p_broamSlot)
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
