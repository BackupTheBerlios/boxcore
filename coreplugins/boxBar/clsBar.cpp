#include "BBApi.h"
#include "clsBar.h"
#include "clsClockItem.h"
#include "clsTrayItemCollection.h"
#include "clsTaskItemCollection.h"
#include "clsFlexiSpacer.h"
#include "clsWorkspaceLabel.h"

#include <shellapi.h>
#include <stdlib.h>

#include "../../dynwinapi/clsUser32.h"

clsUser32 user32;
clsMsimg32 msimg32;

clsBar::clsBar(TCHAR *pClassName, HINSTANCE pInstance, HWND pSlit, bool pVertical): clsItemCollection(pVertical)
{
	trackMouse = false;
	isBar = true;
	slitWnd = pSlit;
	ZeroMemory(&barBlend, sizeof(barBlend));
	barBlend.BlendOp = AC_SRC_OVER;
	brushBitmap = NULL;
	eraseBrush = NULL;
	hInstance = pInstance;
	_tcscpy(className, pClassName);

	hBlackboxWnd = GetBBWnd();
	char rcname[100];
	char pluginpath[MAX_PATH];
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0 , pClassName, -1, rcname, 95, NULL, NULL);
#else
	strcpy(rcname, pClassName);
#endif
	strcat(rcname, ".rc");
	GetModuleFileNameA(hInstance, pluginpath, MAX_PATH);
	if (strrchr(pluginpath, '\\'))
		(*(strrchr(pluginpath, '\\') + 1)) = '\0';
	const CHAR *configFileName = ConfigFileExists(rcname, pluginpath);
	strcpy(configFile, configFileName);
	if (strlen(configFile) == 0)
	{
		strcpy(configFile, pluginpath);
		strcat(configFile, rcname);
	}
	readSettings();

	margin = 0;

	if (bbApiLoader.requestApiPresence(TEXT("boxCore::hasSetTaskbarPos")))
		SetTaskbarPos = (fnSetTaskbarPos)bbApiLoader.requestApiPointer("SetTaskbarPos");
	else
		SetTaskbarPos = NULL;

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof wc);
	wc.lpfnWndProc      = realWndProc;
	wc.cbClsExtra = sizeof(this);
	wc.hInstance        = pInstance;
	wc.lpszClassName    = pClassName;
	wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
	wc.style            = CS_DBLCLKS;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL ,
				   TEXT("Error registering window class"), pClassName,
				   MB_OK | MB_ICONERROR | MB_TOPMOST);
		return;
	}

	int x = ReadInt(configFile, "boxBar.x:", 0);
	int y = ReadInt(configFile, "boxBar.y:", 0);

	barWnd = CreateWindowEx(
				 WS_EX_TOOLWINDOW | WS_EX_TOPMOST | (enableTransparency ? WS_EX_LAYERED : 0),   // window ex-style
				 pClassName,          // window class name
				 NULL,               // window caption text
				 WS_POPUP | WS_OVERLAPPED, // window style
				 x,            // x position
				 y,            // y position
				 200,           // window width
				 200,          // window height
				 NULL,               // parent window
				 NULL,               // window menu
				 pInstance,          // hInstance of .dll
				 this                // creation data
			 );
	ShowWindow(barWnd, SW_SHOWNA);
	if (useSlit && slitWnd)
	{
		PostMessage(slitWnd, SLIT_ADD, 0, (LPARAM)barWnd);
		inSlit = true;
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

}

clsBar::~clsBar()
{
	for (list<clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
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
		PostMessage(slitWnd, SLIT_REMOVE, 0, (LPARAM)barWnd);
	DestroyWindow(barWnd);
	UnregisterClass(className, hInstance);
}

/** @brief realWndProc
  *
  * @todo: document this function
  */
LRESULT CALLBACK clsBar::realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	clsBar *realClass = NULL;
	switch (msg)
	{
	case WM_CREATE:
	case WM_NCCREATE:
		realClass = (clsBar *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		SetClassLongPtr(hWnd, 0, (LONG_PTR)realClass);
		break;
	default:
		realClass = (clsBar *)GetClassLongPtr(hWnd, 0);
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
LRESULT clsBar::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int messages[] = { BB_RECONFIGURE, BB_BROADCAST, BB_TRAYUPDATE, BB_TASKSUPDATE, 0};

	switch (msg)
	{
	case WM_CREATE:
		// Register to reveive these message
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
		readSettings();
		populateBar();
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

	case BOXBAR_UPDATESIZE:
		calculateSizes();
		if (enableTransparency)
			PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
		else
			RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
		break;


		// ----------------------------------------------------------
		// Blackbox sends Broams to all windows...

	case BB_BROADCAST:
	{
		const char *msg_string = (LPCSTR)lParam;
		TRACE(msg_string);
		if (!strnicmp(msg_string, "@boxBar.percentage", strlen("@boxBar.percentage")))
		{
			sizePercentage = atoi(msg_string + strlen("@boxBar.percentage"));
			WriteInt(configFile, "boxBar.percentage:", sizePercentage);
			calculateSizes();
		}
		else if (!strnicmp(msg_string, "@boxBar.vertical", strlen("@boxBar.vertical")))
		{
			vertical = !vertical;
			WriteBool(configFile, "boxBar.vertical:", vertical);
			resize(1, 1);
			populateBar();
			RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
		}
		else if (toggleWithPlugins && (!stricmp(msg_string, "@BBHidePlugins")))
		{
			ShowWindow(barWnd, SW_HIDE);
			SetDesktopMargin(hWnd, 0, 0);
		}
		else if (toggleWithPlugins && (!stricmp(msg_string, "@BBShowPlugins")))
		{
			ShowWindow(barWnd, SW_SHOW);
			if (margin)
				SetDesktopMargin(barWnd, marginEdge, margin);
		}
		break;
	}
	// check general broams
	/*if (!stricmp(msg_string, "@BBShowPlugins"))
	{
		if (my.is_hidden)
		{
			my.is_hidden = false;
			ShowWindow(hwnd, SW_SHOWNA);
		}
		break;
	}

	if (!stricmp(msg_string, "@BBHidePlugins"))
	{
		if (my.pluginToggle && NULL == my.hSlit)
		{
			my.is_hidden = true;
			ShowWindow(hwnd, SW_HIDE);
		}
		break;
	}

	// Our broadcast message prefix:
	const char broam_prefix[] = "@bbSDK.";
	const int broam_prefix_len = sizeof broam_prefix - 1; // minus terminating \0

	// check broams sent from our own menu
	if (!memicmp(msg_string, broam_prefix, broam_prefix_len))
	{
		msg_string += broam_prefix_len;
		if (!stricmp(msg_string, "useSlit"))
		{
			eval_menu_cmd(M_BOL, &my.useSlit, msg_string);
			break;
		}

		if (!stricmp(msg_string, "alwaysOnTop"))
		{
			eval_menu_cmd(M_BOL, &my.alwaysOnTop, msg_string);
			break;
		}

		if (!stricmp(msg_string, "drawBorder"))
		{
			eval_menu_cmd(M_BOL, &my.drawBorder, msg_string);
			break;
		}

		if (!stricmp(msg_string, "snapWindow"))
		{
			eval_menu_cmd(M_BOL, &my.snapWindow, msg_string);
			break;
		}

		if (!stricmp(msg_string, "alphaEnabled"))
		{
			eval_menu_cmd(M_BOL, &my.alphaEnabled, msg_string);
			break;
		}

		if (!my_substr_icmp(msg_string, "alphaValue"))
		{
			eval_menu_cmd(M_INT, &my.alphaValue, msg_string);
			break;
		}

		if (!stricmp(msg_string, "pluginToggle"))
		{
			eval_menu_cmd(M_BOL, &my.pluginToggle, msg_string);
			break;
		}

		if (!my_substr_icmp(msg_string, "windowText"))
		{
			eval_menu_cmd(M_STR, &my.window_text, msg_string);
			break;
		}

		if (!stricmp(msg_string, "editRC"))
		{
			SendMessage(BBhwnd, BB_EDITFILE, (WPARAM) - 1, (LPARAM)rcpath);
			break;
		}

		if (!stricmp(msg_string, "About"))
		{
			about_box();
			break;
		}
	}
	break;
	}
	*/
	// ----------------------------------------------------------
	// prevent the user from closing the plugin with alt-F4

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
		if ((wp->x > rightthirdX) || ((wp->x + wp->cx) == monRect.right))
		{
			barLocation = POS_RIGHT;
			WriteInt(configFile, "boxBar.x:", wp->x + wp->cx);
		}
		else
		{
			if (wp->x > leftthirdX)
			{
				barLocation = POS_CENTER;
				WriteInt(configFile, "boxBar.x:", wp->x + wp->cx / 2);
			}
			else
			{
				barLocation = POS_LEFT;
				WriteInt(configFile, "boxBar.x:", wp->x);
			}
		}
		int third1Y = (monRect.bottom + 2 * monRect.top) / 3;
		int third2Y = (2 * monRect.bottom + monRect.top) / 3;
		if ((wp->y > third2Y) || ((wp->y + wp->cy) == monRect.bottom))
		{
			barLocation += POS_BOTTOM;
			WriteInt(configFile, "boxBar.y:", wp->y + wp->cy);
		}
		else
		{
			if (wp->y > third1Y)
			{
				barLocation += POS_VCENTER;
				WriteInt(configFile, "boxBar.y:", wp->y + wp->cy / 2);
			}
			else
			{
				barLocation += POS_TOP;
				WriteInt(configFile, "boxBar.y:", wp->y);
			}
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
		if (hasTray && SetTaskbarPos)
			SetTaskbarPos(wp->x, wp->y, wp->x + wp->cx, wp->y + wp->cy, barEdge);
	}
	break;
	case WM_LBUTTONDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			moving = true;
			PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | 2, 0);
			return 0;
		}
		break;
	case WM_RBUTTONDOWN:
		if (wParam & MK_CONTROL)
		{
			Menu *mainMenu = MakeNamedMenu("boxBar", "boxBar", true);
			configMenu(mainMenu);
			ShowMenu(mainMenu);
			return 0;
		}
		break;
	case WM_MOUSELEAVE:
		trackMouse = false;
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
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
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	default:
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	}

	return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief Resizes the item and the window
  *
  * @param[in] pX The desired horizontal size
  * @param[in] pY The desired vertical size
  * @return Return the directions in which resizing actually occurred
  *
  * This function resizes the bar item using the base function, but also adjusts the size of the bar window.
  * The bar window adjustment is dependant on the current alignemtn of the bar so that right aligned bars
  * grow to the left, left aligned bars grow to the right etc.
  */
dimType clsBar::resize(int pX, int pY)
{
	RECT barRect;
	GetWindowRect(barWnd, &barRect);
	int newX = barRect.left;
	int newY = barRect.top;
	if (pX < 0)
		pX = getSize(DIM_HORIZONTAL);
	int dX = getSize(DIM_HORIZONTAL) - pX;
	if (barLocation&POS_LEFT)
		newX = barRect.left;
	else if (barLocation&POS_CENTER)
		newX = barRect.left + dX / 2;
	else
		newX = barRect.left + dX;

	if (pY < 0)
		pY = getSize(DIM_VERTICAL);
	int dY = getSize(DIM_VERTICAL) - pY;
	if (barLocation&POS_TOP)
		newY = barRect.top;
	else if (barLocation&POS_VCENTER)
		newY = barRect.top + dY / 2;
	else
		newY = barRect.top + dY;

	SetWindowPos(barWnd, NULL, newX, newY, pX, pY, SWP_NOACTIVATE | SWP_NOZORDER);
	dimType tempReturn = clsItemCollection::resize(pX, pY);
	bufferInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
	bufferInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
	SelectObject(buffer, origBitmap);
	DeleteObject(bufferBitmap);
	bufferBitmap = CreateDIBSection(buffer, &bufferInfo, DIB_RGB_COLORS, NULL, NULL, 0);
	origBitmap = (HBITMAP)SelectObject(buffer, bufferBitmap);
	RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
	PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
	if (inSlit)
		PostMessage(slitWnd, SLIT_UPDATE, 0, (LPARAM)barWnd);
	return tempReturn;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsBar::calculateSizes(bool pSizeGiven)
{
	RECT monRect;
	GetMonitorRect(barWnd, &monRect, GETMON_FROM_WINDOW);
	if (vertical)
		resize(-1, sizePercentage*(monRect.bottom - monRect.top) / 100);
	else
		resize(sizePercentage*(monRect.right - monRect.left) / 100, -1);
	clsItemCollection::calculateSizes(true);
	if (margin)
		SetDesktopMargin(barWnd, marginEdge, margin);
}

/** @brief populateBar
  *
  * @todo: document this function
  */
void clsBar::populateBar()
{
	hasTray = false;
	lastMouse = NULL;
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();

	const CHAR * barItems = ReadString(configFile, "boxBar.items:", "tasks,tray, clock");
	CHAR barItem[MAX_PATH];
	do
	{
		barItems = Tokenize(barItems, barItem, ",");
		if (!stricmp(barItem, "tray"))
		{
			addItem(new clsTrayItemCollection(vertical));
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
			addItem(new clsTaskItemCollection(vertical));
		}
		else if (!stricmp(barItem, "wslabel"))
		{
			addItem(new clsWorkspaceLabel(vertical));
		}
	}
	while (strlen(barItems));

	calculateSizes();
}

/** @brief Adds bar settings to the config menu
  *
  * This function adds a submenu for configuration of the main bar.
  */
void clsBar::configMenu(Menu *pMenu)
{
	Menu *subMenu = MakeNamedMenu("Bar", "Bar", true);
	MakeSubmenu(pMenu, subMenu, "Bar Configuration");
	MakeMenuItemInt(subMenu, "Percentage Size", "@boxBar.percentage", sizePercentage, 0, 100);
	MakeMenuItem(subMenu, "Vertical", "@boxBar.vertical", vertical);
	clsItemCollection::configMenu(pMenu);
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void clsBar::readSettings()
{
	sizePercentage = ReadInt(configFile, "boxBar.percentage:", 80);
	setMargin = ReadBool(configFile, "boxBar.setMargin:", true);
	vertical = ReadBool(configFile, "boxBar.vertical:", false);
	toggleWithPlugins = ReadBool(configFile, "boxBar.TogglewithPlugins:", true);
	useSlit = ReadBool(configFile, "boxBar.useSlit:", false);
	spacingBorder = ReadInt(configFile, "boxBar.spacingBorder:", 3);
	spacingItems = ReadInt(configFile, "boxBar.spacingItems:", 2);
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

	fixed = (vertical ? DIM_HORIZONTAL : DIM_VERTICAL);
}

