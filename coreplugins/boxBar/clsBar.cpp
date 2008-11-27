#include "BBApi.h"
#include "clsBar.h"
#include "clsTrayItemCollection.h"
#include "clsTaskItemCollection.h"
#include "clsClockItem.h"
#include "clsFlexiSpacer.h"

#include <shellapi.h>


clsBar::clsBar(TCHAR *pClassName, HINSTANCE pInstance, bool pVertical): clsItemCollection(pVertical)
{
	trackMouse = false;
	hInstance = pInstance;
	_tcscpy(className, pClassName);
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof wc);

	hBlackboxWnd = GetBBWnd();

	const CHAR *configFileName = ConfigFileExists("plugins/boxBar/boxBar.rc", NULL);
	strcpy(configFile, configFileName);

	int x = ReadInt(configFile, "boxBar.x:", 0);
	int y = ReadInt(configFile, "boxBar.y:", 0);
	sizePercentage = ReadInt(configFile, "boxBar.percentage:", 80);
	setMargin = ReadBool(configFile, "boxBar.setMargin:", true);
	vertical = ReadBool(configFile, "boxBar.vertical:", false);

	fixed = (vertical ? DIM_HORIZONTAL : DIM_VERTICAL);

	margin = 0;

	if (bbApiLoader.requestApiPresence(L"boxCore::hasSetTaskbarPos"))
		SetTaskbarPos = (fnSetTaskbarPos)bbApiLoader.requestApiPointer("SetTaskbarPos");
	else
		SetTaskbarPos = NULL;

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

	barWnd = CreateWindowEx(
				 WS_EX_TOOLWINDOW,   // window ex-style
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

	spacingBorder = 3;
	spacingItems = 2;


	const CHAR * barItems = ReadString(configFile, "boxBar.items:", "tray, clock");
	CHAR barItem[MAX_PATH];
	do
	{
		barItems = Tokenize(barItems, barItem, ",");
		if (!stricmp(barItem, "tray"))
		{
			addItem(new clsTrayItemCollection(vertical));
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
	}
	while (strlen(barItems));

	calculateSizes();
}

clsBar::~clsBar()
{
	for (list<clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		delete (*i);
	itemList.clear();
	bbApiLoader.freeLibrary();
	DestroyWindow(barWnd);
	dbg_printf("Window destroyed");
	UnregisterClass(className, hInstance);
	dbg_printf("Class unregistered");
	//dtor
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
		//ReadRCSettings();
		//GetStyleSettings();
		//set_window_modes();
		InvalidateRect(barWnd, &itemArea, TRUE);
		break;

		// ----------------------------------------------------------
		// Painting with a cached double-buffer.

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			draw(hdc);
			EndPaint(hWnd, &ps);
			break;
		}

	case BOXBAR_UPDATESIZE:
		calculateSizes();
		break;
		/*

			// ----------------------------------------------------------
			// Blackbox sends Broams to all windows...

		case BB_BROADCAST:
		{
			const char *msg_string = (LPCSTR)lParam;

			// check general broams
			if (!stricmp(msg_string, "@BBShowPlugins"))
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
			//moving = false;
			WINDOWPOS* wp = (WINDOWPOS*)lParam;
			RECT monRect;
			GetMonitorRect(hWnd, &monRect, GETMON_FROM_WINDOW);
			int leftthirdX = (monRect.right + 2 * monRect.left) / 3;
			int rightthirdX = (2 * monRect.right + monRect.left) / 3;
			//if (margin)
			//	SetDesktopMargin(barWnd, 0, 0);
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
			if (SetTaskbarPos)
				SetTaskbarPos(wp->x, wp->y, wp->x + wp->cx, wp->y + wp->cy, barEdge);
		}
		break;
	case WM_LBUTTONDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			// start moving, when control-key is held down
			moving = true;
			PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | 2, 0);
			return 0;
		}
	case WM_MOUSELEAVE:
		trackMouse = false;
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	case WM_MOUSEMOVE:
		if (!trackMouse)
		{
			TRACKMOUSEEVENT mouseTrack;
			ZeroMemory(&mouseTrack, sizeof(mouseTrack));
			mouseTrack.cbSize = sizeof(mouseTrack);
			mouseTrack.dwFlags = TME_LEAVE;
			mouseTrack.hwndTrack = barWnd;
			TrackMouseEvent(&mouseTrack);
			trackMouse = true;
		}
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	default:
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	}

	return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief draw
  *
  * @todo: document this function
  */
void clsBar::draw(HDC pContext)
{
	MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(SN_TOOLBAR), true);//bbStyle.getStyleBorder(SN_TOOLBAR));
	clsItemCollection::draw(pContext);
}

/** @brief resize
  *
  * @todo: document this function
  */
dimType clsBar::resize(int pX, int pY)
{
	RECT barRect;
	GetWindowRect(barWnd, &barRect);
	int newX = barRect.left;
	int newY = barRect.top;
	if (pX < minSizeX)
		pX = minSizeX;
	int dX = getSize(DIM_HORIZONTAL) - pX;
	if (barLocation&POS_LEFT)
		newX = barRect.left;
	else if (barLocation&POS_CENTER)
		newX = barRect.left + dX / 2;
	else
		newX = barRect.left + dX;

	if (pY < minSizeY)
		pY = minSizeY;
	int dY = getSize(DIM_VERTICAL) - pY;
	if (barLocation&POS_TOP)
		newY = barRect.top;
	else if (barLocation&POS_VCENTER)
		newY = barRect.top + dY / 2;
	else
		newY = barRect.top + dY;

	SetWindowPos(barWnd, NULL, newX, newY, pX, pY, SWP_NOACTIVATE | SWP_NOZORDER);
	dimType tempReturn = clsItemCollection::resize(pX, pY);
	InvalidateRect(barWnd, &itemArea, TRUE);
	return tempReturn;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsBar::calculateSizes(bool pSizeGiven)
{
	clsItemCollection::calculateSizes(false);
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



