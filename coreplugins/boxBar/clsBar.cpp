#include "BBApi.h"
#include "clsBar.h"
#include "clsClockItem.h"
#include "clsTrayItemCollection.h"
#include "clsTaskArea.h"
#include "clsFlexiSpacer.h"
#include "clsWorkspaceLabel.h"
#include "clsButton.h"

#include "clsTip.h"

#include <shellapi.h>
#include <stdlib.h>

#include "../../dynwinapi/clsUser32.h"
#include "../../utility/stringcopy.h"

namespace Plugin_boxBar
{

/**
 * @page boxBarRC
 * @section Bar General settings
 * @code boxBar.Percentage: 80 @endcode
 * Sets the percentage of screen width (vertical: false) or height (vertical: true) that the bar will occupy.
 * Ignored if no items on the bar are allow stretching.
 *
 * @code boxBar.Vertical: false @endcode
 * Determines whether the bar is drawn vertically or horizontally
 *
 * @code boxBar.Items: Button,Tasks,Tray,Clock @endcode
 * Select the items to show on the bar. Available items are @ref boxBarTray, @ref boxBarButton,
 * @ref boxBarTasks and each name links to its appropriate settings in this file.
 *
 * @page boxBarRCAdvanced
 * @section boxBarAdv General Settings
 */

Bar::Bar(TCHAR *pClassName, HINSTANCE pInstance, HWND pSlit, bool pVertical): Plugin(pInstance),
		Collection(pVertical, m_pluginName.c_str(), 3, 2),
		sizePercentage(s_settingsManager.AssociateInt(m_pluginName.c_str(), NULL, "Percentage", 80))
{
	hInstance = pInstance;
	m_pluginPrefix = m_pluginName.c_str();

	m_activeTip = NULL;
	m_tipTimer = getTimerID();
	m_replaceTip = true;
}

Bar::~Bar()
{
}

/** @brief realWndProc
  *
  * @todo: document this function
  */
LRESULT CALLBACK Bar::realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Bar *realClass = NULL;
	switch (msg)
	{
	case WM_CREATE:
	case WM_NCCREATE:
		realClass = (Bar *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		SetClassLongPtr(hWnd, 0, (LONG_PTR)realClass);
		break;
	default:
		realClass = (Bar *)GetClassLongPtr(hWnd, 0);
		break;
	}
	if (realClass)
		return realClass->wndProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);

}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT Bar::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int messages[] = { BB_RECONFIGURE, BB_BROADCAST, BB_TRAYUPDATE, BB_TASKSUPDATE, 0};

	switch (msg)
	{
	case WM_CREATE:
		// Register to receive these message
		SendMessage(hBlackboxWnd, BB_REGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)messages);
		// Make the window appear on all workspaces
		MakeSticky(hWnd);
		return 0;

	case WM_DESTROY:
		SetDesktopMargin(hWnd, 0, 0);
		RemoveSticky(hWnd);
		SendMessage(hBlackboxWnd, BB_UNREGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)messages);
		return 0;

		// ----------------------------------------------------------
		// Blackbox sends a "BB_RECONFIGURE" message on style changes etc.

	case BB_RECONFIGURE:
		bbStyle.Update();
		s_settingsManager.ReadSettings();
		readSettings();
		PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
		break;

		// ----------------------------------------------------------
		// Painting with a cached double-buffer.
	case BOXBAR_REDRAW:
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if (!style)
		{
			FillRect(buffer, &itemArea, eraseBrush);
		}

		draw(buffer);
		POINT dcPoint;
		dcPoint.x = 0;
		dcPoint.y = 0;
		RECT windowRect;
		GetWindowRect(barWnd, &windowRect);
		POINT drawPoint;
		drawPoint.x = windowRect.left;
		drawPoint.y = windowRect.top;
		SIZE drawSize;
		drawSize.cx = windowRect.right - windowRect.left;
		drawSize.cy = windowRect.bottom - windowRect.top;
		if (user32.UpdateLayeredWindow && enableTransparency)
		{
			if (!user32.UpdateLayeredWindow(hWnd, GetDC(NULL), &drawPoint, &drawSize, buffer, &dcPoint, RGB(255, 0, 255), &barBlend, alphaDraw ? ULW_ALPHA : ULW_OPAQUE))
			{
				style = SN_TOOLBAR;
				enableTransparency = false;
			}
		}
		else
			BitBlt(hdc, itemArea.left, itemArea.top,
				   itemArea.right - itemArea.left, itemArea.bottom - itemArea.top,
				   buffer, itemArea.left, itemArea.top, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	}

	//Taken from bbleanbar
	case WM_MOUSEACTIVATE:
		return MA_NOACTIVATE;

	case WM_DISPLAYCHANGE:
		break;

	case BOXBAR_UPDATESIZE:
		if (wParam)
		{
			PRINT(TEXT("Major resize"));
			resize(1,1);
			populateBar();
		}
		calculateSizes();
		if (enableTransparency)
			PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
		else
			RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
		break;

	case BOXBAR_QUEUEBALLOON:
		QueueTip(reinterpret_cast<Tip *>(lParam));
		return 0;

	case BOXBAR_BALLOONDONE:
		if (reinterpret_cast<Tip *>(lParam) == m_activeTip)
		{
			delete m_activeTip;
			m_activeTip = NULL;
			m_replaceTip = true;
			KillTimer(hWnd, m_tipTimer);
			if (m_tipQueue.size() > 0)
			{
				m_activeTip = m_tipQueue.front();
				m_tipQueue.pop_front();
				ShowTip(m_activeTip);
			}
		}
		return 0;


		// ----------------------------------------------------------
		// Blackbox sends Broams to all windows...

		/**
		 * @page boxBarBroams
		 * @section boxBarBar
		 * @code @boxBar.Percentage XX @endcode
		 * Changes the boxBar.Percentage setting
		 */
	case BB_BROADCAST:
	{
		const char *msg_string = (LPCSTR)lParam + 1;
		const char *element = NULL;
		if (!strnicmp(msg_string, m_pluginPrefix, strlen(m_pluginPrefix)))
		{
			msg_string += strlen(m_pluginPrefix) + 1;

			if ((element = "percentage") && !strnicmp(msg_string, element, strlen(element)))
			{
				msg_string += strlen(element);
				sizePercentage = atoi(msg_string);
				s_settingsManager.WriteSetting(m_pluginPrefix, NULL, element);
				readSettings();
				calculateSizes();
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
			}
			else if ((element = "Vertical") && !strnicmp(msg_string, element, strlen(element)))
			{
				vertical = !vertical;
				s_settingsManager.WriteSetting(m_pluginPrefix, NULL, element);
				resize(1, 1);
				populateBar();
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
				PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
			}
			else if ((element = "DumpSettings") && !strnicmp(msg_string, element, strlen(element)))
			{
				s_settingsManager.WriteSettings();
			}
			else if ((element = "align") && !strnicmp(msg_string, element, strlen(element)))
			{
				msg_string += strlen(element) + 1;
				if ((element = "vertical") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element) + 1;
					WriteString(configFile, "boxBar.align.vertical:", msg_string);
					readSettings();
				}
				else if ((element = "horizontal") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element) + 1;
					WriteString(configFile, "boxBar.align.horizontal:", msg_string);
					readSettings();
				}
			}
		}
		else if (toggleWithPlugins && (element = "@BBHidePlugins") && (!stricmp(msg_string, element)))
		{
			ShowWindow(barWnd, SW_HIDE);
			SetDesktopMargin(hWnd, 0, 0);
		}
		else if (toggleWithPlugins && (element = "@BBShowPlugins") && (!stricmp(msg_string, element)))
		{
			ShowWindow(barWnd, SW_SHOW);
			if (margin)
				SetDesktopMargin(barWnd, marginEdge, margin);
		}
		break;
	}

	case WM_CLOSE:
		break;

	case WM_EXITSIZEMOVE:
		moving = false;
		if (margin)
			SetDesktopMargin(barWnd, marginEdge, margin);
		break;

	case WM_WINDOWPOSCHANGING:
	{
		WINDOWPOS* wp = (WINDOWPOS*)lParam;
		SnapWindowToEdge(wp, 10, moving ? SNAP_FULLSCREEN : SNAP_FULLSCREEN | SNAP_SIZING);
	}
	break;

	case WM_WINDOWPOSCHANGED:
	{
		WINDOWPOS* wp = (WINDOWPOS*)lParam;
		RECT monRect;
		GetMonitorRect(hWnd, &monRect, GETMON_FROM_WINDOW);
		int leftthirdX = (monRect.right + 2 * monRect.left) / 3;
		int rightthirdX = (2 * monRect.right + monRect.left) / 3;
		if ((wp->x > rightthirdX))
		{
			barLocation = POS_RIGHT;
		}
		else
		{
			if (wp->x > leftthirdX)
			{
				barLocation = POS_CENTER;
			}
			else
			{
				barLocation = POS_LEFT;
			}
		}
		int third1Y = (monRect.bottom + 2 * monRect.top) / 3;
		int third2Y = (2 * monRect.bottom + monRect.top) / 3;
		if ((wp->y > third2Y))
		{
			barLocation += POS_BOTTOM;
		}
		else
		{
			if (wp->y > third1Y)
			{
				barLocation += POS_VCENTER;
			}
			else
			{
				barLocation += POS_TOP;
			}
		}
		UINT hGrowth = (m_barHGrowth ? m_barHGrowth : barLocation);
		UINT vGrowth = (m_barVGrowth ? m_barVGrowth : barLocation);
		if (hGrowth & POS_RIGHT)
		{
			WriteInt(configFile, "boxBar.x:", wp->x + wp->cx - 1);
		}
		else if (hGrowth & POS_CENTER)
		{
			WriteInt(configFile, "boxBar.x:", wp->x + wp->cx / 2);
		}
		else
		{
			WriteInt(configFile, "boxBar.x:", wp->x);
		}
		if (vGrowth & POS_BOTTOM)
		{
			WriteInt(configFile, "boxBar.y:", wp->y + wp->cy - 1);
		}
		else if (vGrowth & POS_VCENTER)
		{
			WriteInt(configFile, "boxBar.y:", wp->y + wp->cy / 2);
		}
		else
		{
			WriteInt(configFile, "boxBar.y:", wp->y);
		}
		if (setMargin)
		{
			if (vertical)
			{
				if (barLocation & POS_RIGHT)
				{
					margin = monRect.right - wp->x;
					marginEdge = BB_DM_RIGHT;
				}
				else if (barLocation & POS_LEFT)
				{
					margin = wp->x + wp->cx;
					marginEdge = BB_DM_LEFT;
				}
				else
					margin = 0;
			}
			else
			{
				if (barLocation & POS_BOTTOM)
				{
					margin = monRect.bottom - wp->y;
					marginEdge = BB_DM_BOTTOM;
				}
				else if (barLocation & POS_TOP)
				{
					margin = wp->y + wp->cy;
					marginEdge = BB_DM_TOP;
				}
				else
					margin = 0;
			}
			//if (margin)
			//	SetDesktopMargin(barWnd, marginEdge, margin);
		}
		UINT barEdge;
		if (vertical)
		{
			int mid = (monRect.right + monRect.left) / 2;
			if (wp->x > mid)
				barEdge = ABE_RIGHT;
			else
				barEdge = ABE_LEFT;
		}
		else
		{
			int mid = (monRect.bottom + monRect.top) / 2;
			if (wp->y > mid)
				barEdge = ABE_BOTTOM;
			else
				barEdge = ABE_TOP;
		}
		if (hasTray)
		{
			char broam[255];
			sprintf(broam,"@boxCore.TaskbarLocation %d %d %d %d %u",wp->x, wp->y, wp->x + wp->cx - 1, wp->y + wp->cy - 1, barEdge);
			SendMessage(hBlackboxWnd, BB_BROADCAST, NULL, reinterpret_cast<LPARAM>(broam));
		}
	}
	break;
	case WM_LBUTTONDOWN:
		if (!inSlit && GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			moving = true;
			PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | 2, 0);
			return 0;
		}
		break;
	case WM_NCRBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			Menu *mainMenu = MakeNamedMenu("boxBar", "boxBar", true);
			configMenu(mainMenu);
			ShowMenu(mainMenu);
			return 0;
		}
		break;
	case WM_MOUSELEAVE:
		trackMouse = false;
		return Collection::wndProc(hWnd, msg, wParam, lParam);
	case WM_MOUSEMOVE:
		if (user32.TrackMouseEvent && !trackMouse)
		{
			static TRACKMOUSEEVENT mouseTrack;
			ZeroMemory(&mouseTrack, sizeof(mouseTrack));
			mouseTrack.cbSize = sizeof(mouseTrack);
			mouseTrack.dwFlags = TME_LEAVE;
			mouseTrack.hwndTrack = barWnd;
			user32.TrackMouseEvent(&mouseTrack);
			trackMouse = true;
		}
		return Collection::wndProc(hWnd, msg, wParam, lParam);
	case WM_TIMER:
		if (wParam == m_tipTimer)
		{
			m_replaceTip = true;
			if (m_tipQueue.size() > 0)
			{
				m_activeTip->Timeout();
			}
		}
		return Collection::wndProc(hWnd, msg, wParam, lParam);
	default:
		return Collection::wndProc(hWnd, msg, wParam, lParam);
	}

	return Collection::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief Resizes the item and the window
  *
  * @param[in] pX The desired horizontal size
  * @param[in] pY The desired vertical size
  * @return Return the directions in which resizing actually occurred
  *
  * This function resizes the bar item using the base function, but also adjusts the size of the bar window.
  * The bar window adjustment is dependent on the current alignment of the bar so that right aligned bars
  * grow to the left, left aligned bars grow to the right etc.
  */
dimType Bar::resize(int pX, int pY)
{
	RECT barRect;
	GetWindowRect(barWnd, &barRect);
	int newX = barRect.left;
	int newY = barRect.top;
	if (pX < 0)
	{
		pX = getSize(DIM_HORIZONTAL);
	}
	else
	{
		int dX = getSize(DIM_HORIZONTAL) - pX;
		UINT growth = (m_barHGrowth ? m_barHGrowth : barLocation);
		if (growth&POS_LEFT)
			newX = barRect.left;
		else if (growth&POS_CENTER)
			newX = barRect.left + dX / 2;
		else
			newX = barRect.left + dX;
	}
	if (pY < 0)
	{
		pY = getSize(DIM_VERTICAL);
	}
	else
	{
		int dY = getSize(DIM_VERTICAL) - pY;
		UINT growth = (m_barVGrowth ? m_barVGrowth : barLocation);
		if (growth&POS_TOP)
			newY = barRect.top;
		else if (growth&POS_VCENTER)
			newY = barRect.top + dY / 2;
		else
			newY = barRect.top + dY;
	}
	SetWindowPos(barWnd, NULL, newX, newY, pX, pY, SWP_NOACTIVATE | SWP_NOZORDER);
	dimType tempReturn = Collection::resize(pX, pY);
	bufferInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
	bufferInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
	SelectObject(buffer, origBitmap);
	DeleteObject(bufferBitmap);
	bufferBitmap = CreateDIBSection(buffer, &bufferInfo, DIB_RGB_COLORS, NULL, NULL, 0);
	origBitmap = (HBITMAP)SelectObject(buffer, bufferBitmap);
	RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
	PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
	if (inSlit)
		SendMessage(m_hSlit, SLIT_UPDATE, 0, reinterpret_cast<LPARAM>(barWnd));
	if (margin)
		SetDesktopMargin(barWnd, marginEdge, margin);
	return tempReturn;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void Bar::calculateSizes(bool pSizeGiven)
{
	Collection::calculateSizes(true);
	if (margin)
		SetDesktopMargin(barWnd, marginEdge, margin);
}

/** @brief populateBar
  *
  * @todo: document this function
  */
void Bar::QueueTip(Tip *p_tip)
{
	if (m_activeTip)
	{
		m_tipQueue.push_back(p_tip);
		if (m_replaceTip)
		{
			m_activeTip->Timeout();
		}
	}
	else
	{
		m_activeTip = p_tip;
		ShowTip(m_activeTip);
	}
}

void Bar::KillTips(HWND p_hWnd, UINT p_uID)
{
	for (deque<Tip *>::iterator i = m_tipQueue.begin(); i != m_tipQueue.end(); ++i)
	{
		if ((*i)->BelongsTo(p_hWnd, p_uID))
		{
			(*i)->Kill();
			delete *i;
			m_tipQueue.erase(i);
			KillTips(p_hWnd, p_uID);
			return;
		}
	}
	if (m_activeTip->BelongsTo(p_hWnd, p_uID))
	{
		m_activeTip->Kill();
		delete m_activeTip;
		m_activeTip = NULL;
		m_replaceTip = true;
		KillTimer(barWnd, m_tipTimer);
		if (m_tipQueue.size() > 0)
		{
			m_activeTip = m_tipQueue.front();
			m_tipQueue.pop_front();
			ShowTip(m_activeTip);
		}
	}
}

void Bar::populateBar()
{
	hasTray = false;
	lastMouse = NULL;
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();

	const CHAR * barItems = ReadString(configFile, "boxBar.items:", "button,tasks,tray,clock");
	CHAR barItem[MAX_PATH];
	do
	{
		barItems = Tokenize(barItems, barItem, ",");
		if (!stricmp(barItem, "tray"))
		{
			addItem(new TrayArea(vertical));
			hasTray = true;
		}
		else if (!stricmp(barItem, "clock"))
		{
			addItem(new clsClockItem(vertical));
		}
		else if (!stricmp(barItem, "flexispace"))
		{
			addItem(new clsFlexiSpacer(vertical));
		}
		else if (!stricmp(barItem, "tasks"))
		{
			addItem(new TaskArea(vertical));
		}
		else if (!stricmp(barItem, "wslabel"))
		{
			addItem(new clsWorkspaceLabel(vertical));
		}
		else if (!stricmp(barItem, "button"))
		{
			addItem(new Button());
		}
	}
	while (strlen(barItems));
	RECT monRect;
	GetMonitorRect(barWnd, &monRect, GETMON_FROM_WINDOW);
	if (vertical)
		resize(-1, sizePercentage*(monRect.bottom - monRect.top) / 100);
	else
		resize(sizePercentage*(monRect.right - monRect.left) / 100, -1);
	calculateSizes();
}

/** @brief Adds bar settings to the config menu
  *
  * This function adds a submenu for configuration of the main bar.
  */
void Bar::configMenu(Menu *pMenu, bool p_update)
{
	CHAR buffer[256];
	LPCSTR menuTitle;
	Menu *subMenu = MakeNamedMenu("Bar", "boxBar::bar", true);
	MakeSubmenu(pMenu, subMenu, "Bar Configuration");
	MakeMenuItemInt(subMenu, "Percentage Size", PluginBroam(buffer, "Percentage"), sizePercentage, 0, 100);
	MakeMenuItem(subMenu, "Vertical", PluginBroam(buffer, "Vertical"), vertical);
	menuTitle = "Vertical Alignment";
	Menu *subSubMenu = MakeNamedMenu(menuTitle,"boxBar::bar::verticalAlign", true);
	MakeSubmenu(subMenu, subSubMenu, menuTitle);
	MakeMenuItem(subSubMenu, "Top", PluginBroam(buffer, "align.vertical.top"), m_barVGrowth & POS_TOP);
	MakeMenuItem(subSubMenu, "Center", PluginBroam(buffer, "align.vertical.center"), m_barVGrowth & POS_VCENTER);
	MakeMenuItem(subSubMenu, "Bottom", PluginBroam(buffer, "align.vertical.bottom"), m_barVGrowth & POS_BOTTOM);
	MakeMenuItem(subSubMenu, "Auto", PluginBroam(buffer, "align.vertical.auto"), !m_barVGrowth);

	menuTitle = "Horizontal Alignment";
	subSubMenu = MakeNamedMenu(menuTitle, "boxBar::bar::horizontalAlign", true);
	MakeSubmenu(subMenu, subSubMenu, menuTitle);
	MakeMenuItem(subSubMenu, "Left", PluginBroam(buffer, "align.horizontal.left"), m_barHGrowth & POS_LEFT);
	MakeMenuItem(subSubMenu, "Center", PluginBroam(buffer, "align.horizontal.center"), m_barHGrowth & POS_CENTER);
	MakeMenuItem(subSubMenu, "Right", PluginBroam(buffer, "align.horizontal.right"), m_barHGrowth & POS_RIGHT);
	MakeMenuItem(subSubMenu, "Auto", PluginBroam(buffer, "align.horizontal.auto"), !m_barHGrowth);
	Collection::configMenu(pMenu, p_update);
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void Bar::readSettings()
{
	if (!inSlit)
	{
		setMargin = ReadBool(configFile, "boxBar.setMargin:", true);
	}
	else
	{
		setMargin = false;
	}
	m_barVGrowth = 0;
	const char * configString = ReadString(configFile, "boxBar.align.vertical:", "auto");
	if (!stricmp(configString,"top"))
	{
		m_barVGrowth |= POS_TOP;
	}
	else if (!stricmp(configString,"center"))
	{
		m_barVGrowth |= POS_VCENTER;
	}
	else if (!stricmp(configString,"bottom"))
	{
		m_barVGrowth |= POS_BOTTOM;
	}
	m_barHGrowth = 0;
	configString = ReadString(configFile, "boxBar.align.horizontal:", "auto");
	if (!stricmp(configString,"left"))
	{
		m_barHGrowth |= POS_LEFT;
	}
	else if (!stricmp(configString,"center"))
	{
		m_barHGrowth |= POS_CENTER;
	}
	else if (!stricmp(configString,"right"))
	{
		m_barHGrowth |= POS_RIGHT;
	}

	toggleWithPlugins = ReadBool(configFile, "boxBar.TogglewithPlugins:", true);
	useSlit = ReadBool(configFile, "boxBar.useSlit:", false);
	//spacingBorder = ReadInt(configFile, "boxBar.spacingBorder:", 3);
	//spacingItems = ReadInt(configFile, "boxBar.spacingItems:", 2);
	itemAlpha = ReadInt(configFile, "boxBar.alpha:", 255);
	if (itemAlpha > 255)
		itemAlpha = 255;
	else if (itemAlpha < 0)
		itemAlpha = 0;
	if (msimg32.AlphaBlend)
	{
		if (user32.UpdateLayeredWindow)
			style = ReadBool(configFile, "boxBar.drawBackground:", true) ? SN_TOOLBAR : 0;
		else
			style = SN_TOOLBAR;

		if (style)
		{
			enableTransparency = ReadBool(configFile, "boxBar.enableTransparency:", false);
			alphaDraw = ReadBool(configFile, "boxBar.useAlphaBlend:", false);
		}
		else
		{
			enableTransparency = true;
			alphaDraw = true;
			if (!eraseBrush)
			{
				bufferInfo.bmiHeader.biWidth = 1;
				bufferInfo.bmiHeader.biHeight = 1;
				brushBitmap = CreateDIBSection(NULL, &bufferInfo, DIB_RGB_COLORS, NULL, NULL, 0);
				eraseBrush = CreatePatternBrush(brushBitmap);
			}
		}
	}
	else
	{
		style = SN_TOOLBAR;
		alphaDraw = false;
	}

	barBlend.SourceConstantAlpha = itemAlpha;
	barBlend.AlphaFormat = alphaDraw ? AC_SRC_ALPHA : 0;

	m_knowsSize = (vertical ? DIM_HORIZONTAL : DIM_VERTICAL);
	m_wantsStretch = DIM_NONE;
	RECT monRect;
	GetMonitorRect(barWnd, &monRect, GETMON_FROM_WINDOW);
	if (vertical)
		resize(-1, sizePercentage*(monRect.bottom - monRect.top) / 100);
	else
		resize(sizePercentage*(monRect.right - monRect.left) / 100, -1);
	calculateSizes();
}

void Bar::ShowTip(Tip *p_tip)
{
	RECT tipRect;
	RECT barRect;
	GetWindowRect(m_activeTip->getTipWindow(),&tipRect);
	GetWindowRect(barWnd, &barRect);
	if (vertical)
	{
		if (tipRect.right > barRect.left)
		{
			int move = tipRect.right - barRect.left - 1;
			tipRect.right -= move;
			tipRect.left -= move;
		}
		else if (tipRect.left < barRect.right)
		{
			int move = barRect.right - tipRect.left - 1;
			tipRect.right += move;
			tipRect.left += move;
		}
	}
	else
	{
		if (tipRect.bottom > barRect.top)
		{
			int move = tipRect.bottom - barRect.top - 1;
			tipRect.bottom -= move;
			tipRect.top -= move;
		}
		else if (tipRect.top < barRect.bottom)
		{
			int move = barRect.bottom - tipRect.top - 1;
			tipRect.bottom += move;
			tipRect.top += move;
		}
	}
	SetWindowPos(m_activeTip->getTipWindow(), NULL, tipRect.left, tipRect.top, tipRect.right - tipRect.left -1, tipRect.bottom - tipRect.top -1, SWP_NOACTIVATE | SWP_NOZORDER);
	m_activeTip->Display();
	m_replaceTip = false;
	SetTimer(barWnd, m_tipTimer, 10000, NULL);
}

void Bar::SetPluginName(TCHAR *p_pluginName)
{
	delete [] m_pluginPrefix;
	m_pluginPrefix = new CHAR[_tcslen(p_pluginName)+1];
	//CopyString(m_pluginPrefix, p_pluginName, _tcslen(p_pluginName)+1);
}

INT Bar::BeginPlugin()
{
	trackMouse = false;
		isBar = true;
		ZeroMemory(&barBlend, sizeof(barBlend));
		barBlend.BlendOp = AC_SRC_OVER;
		brushBitmap = NULL;
		eraseBrush = NULL;
		CopyString(className, m_pluginName.c_str(), 100);
		hBlackboxWnd = GetBBWnd();
		configFile = m_rcPath.c_str();
		s_settingsManager.AttachFile(m_rcPath.c_str());
		s_settingsManager.ReadSettings();
		readSettings();

		margin = 0;

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof wc);
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc      = realWndProc;
		wc.cbClsExtra = sizeof(this);
		wc.hInstance        = hInstance;
		wc.lpszClassName    = className;
		wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
		wc.style            = CS_DBLCLKS;

		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL ,
					   TEXT("Error registering window class"), className,
					   MB_OK | MB_ICONERROR | MB_TOPMOST);
			return 1;
		}

		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.cbWndExtra = sizeof(this);
		wc.hInstance        = hInstance;
		wc.lpszClassName    = TEXT("boxBarTip");
		wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
		wc.style            = CS_DBLCLKS;
		wc.lpfnWndProc      = Tip::realWndProc;
		RegisterClassEx(&wc);

		int x = ReadInt(configFile, "boxBar.x:", 0);
		int y = ReadInt(configFile, "boxBar.y:", 0);
		itemArea.right = 1;
		itemArea.bottom = 1;
		barWnd = CreateWindowEx(
					 WS_EX_TOOLWINDOW | WS_EX_TOPMOST | (enableTransparency ? WS_EX_LAYERED : 0),   // window ex-style
					 className,          // window class name
					 NULL,               // window caption text
					 WS_POPUP | WS_OVERLAPPED, // window style
					 x,            // x position
					 y,            // y position
					 1,           // window width
					 1,          // window height
					 NULL,               // parent window
					 NULL,               // window menu
					 hInstance,          // hInstance of .dll
					 this                // creation data
				 );
		ShowWindow(barWnd, SW_SHOWNA);
		if (useSlit && m_hSlit)
		{
			SendMessage(m_hSlit, SLIT_ADD, 0, reinterpret_cast<LPARAM>(barWnd));
			SendMessage(m_hSlit, SLIT_UPDATE, 0, reinterpret_cast<LPARAM>(barWnd));
			SetWindowPos(barWnd, NULL, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
			inSlit = true;
			setMargin = false;
		}
		else
		{
			inSlit = false;
		}

		buffer = CreateCompatibleDC(NULL);
		ZeroMemory(&bufferInfo, sizeof(bufferInfo));
		bufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bufferInfo.bmiHeader.biWidth = 1;
		bufferInfo.bmiHeader.biHeight = 1;
		bufferInfo.bmiHeader.biPlanes = 1;
		bufferInfo.bmiHeader.biBitCount = 32;
		brushBitmap = CreateDIBSection(buffer, &bufferInfo, DIB_RGB_COLORS, NULL, NULL, 0);
		eraseBrush = CreatePatternBrush(brushBitmap);

		bufferInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
		bufferInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
		bufferBitmap = CreateDIBSection(buffer, &bufferInfo, DIB_RGB_COLORS, NULL, NULL, 0);
		origBitmap = (HBITMAP)SelectObject(buffer, bufferBitmap);

		populateBar();
		return 0;
}

void Bar::EndPlugin()
{
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
			delete (*i);
		itemList.clear();
		bbApiLoader.freeLibrary();
		if (eraseBrush)
			DeleteObject(eraseBrush);
		if (brushBitmap)
			DeleteObject(brushBitmap);
		SelectObject(buffer, origBitmap);
		DeleteObject(bufferBitmap);
		DeleteObject(eraseBrush);
		DeleteObject(brushBitmap);
		DeleteDC(buffer);
		if (inSlit)
			PostMessage(m_hSlit, SLIT_REMOVE, 0, (LPARAM)barWnd);
		DestroyWindow(barWnd);
		UnregisterClass(className, hInstance);
		UnregisterClass(TEXT("boxBarTip"), hInstance);
}

}
