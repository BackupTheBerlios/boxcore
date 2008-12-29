#include "BBApi.h"
//#include "commctrl.h"
#define DM_CREATEICON WM_USER+5

/*
Storage in windows
0 iconData *
*/

enum {BBLEAN,BB4WIN,XOBLITE};
int branch=0;

char rcpath[MAX_PATH],hookPath[MAX_PATH],myPath[MAX_PATH];
HINSTANCE hInstance;
static HANDLE   hProcess = NULL;
static HWND     hPluginWnd = NULL;
HWND tooltipWnd = NULL;
static int      iconMessages[] = {BB_TASKSUPDATE, 0};
static int      pluginMessages[] = {BB_RECONFIGURE, BB_TASKSUPDATE, 0};
void ReadRCSettings(void);
HWND last=NULL;
UINT msgBroadcast;
enum {DM_CHECKWND=1000,DM_KILL,DM_ICONREMOVED,DM_RECONFIGURE};
RECT workspace;
int numIcons=0;

enum {ORIENT_HORIZONTAL,ORIENT_VERTICAL};
int orientation;
int directionX,directionY;
int startX,startY;
bool drawBorder;

static const char szIconClass[] = "IconClass";
static const char szPluginClass[] = "bbDeskMinimize";

HWND lastActive;

struct
{
	StyleItem ToolbarStyle;
	HFONT ToolbarFont;
} currentStyle;

struct
{
	int sizeX,sizeY;
	int spacingX,spacingY;
	int iconPadding;
	int iconSize;
	bool caption;
	int captionPosition;
	int textWidth;
	int iconsPerRow,iconsPerCol;
	bool drawBorder;
} iconParams;

class IconData
{
public:
	IconData();
	HWND target;
	int iconNumber;
	int workspace;
	HICON icon;
	char *caption;
};

IconData::IconData()
{
	target=NULL;
	iconNumber=0;
	workspace=0;
	icon=NULL;
	caption=NULL;
}

HINSTANCE hookLibrary=NULL;

typedef void (*HookSetProc)(void);

HWND newIconTask = NULL;

int getPosX(int iconNumber)
{
	switch (orientation)
	{
	case ORIENT_VERTICAL:
		return startX+directionX*((iconNumber/iconParams.iconsPerCol)*(iconParams.sizeX+iconParams.spacingX)+iconParams.spacingX)+(directionX==-1?-iconParams.sizeX:0);
	case ORIENT_HORIZONTAL:
		return startX+directionX*((iconNumber%iconParams.iconsPerRow)*(iconParams.sizeX+iconParams.spacingX)+iconParams.spacingX)+(directionX==-1?-iconParams.sizeX:0);
	}
	return 0;
}

int getPosY(int iconNumber)
{
	switch (orientation)
	{
	case ORIENT_VERTICAL:
		return startY+directionY*((iconNumber%iconParams.iconsPerCol)*(iconParams.sizeY+iconParams.spacingY)+iconParams.spacingY)+(directionY==-1?-iconParams.sizeY:0);
	case ORIENT_HORIZONTAL:
		return startY+directionY*((iconNumber/iconParams.iconsPerRow)*(iconParams.sizeY+iconParams.spacingY)+iconParams.spacingY)+(directionY==-1?-iconParams.sizeY:0);
	}
	return 0;
}

void MakeIcon(HWND source)
{
	bool valid=false;
	for (int i=0;i<GetTaskListSize();++i)
		if (GetTask(i)==source)
		{
			valid=true;
			break;
		}
	if (valid)
	{
		newIconTask=source;
		HWND icon = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_LAYERED, szIconClass, NULL, WS_POPUP|WS_VISIBLE, getPosX(numIcons), getPosY(numIcons), iconParams.sizeX, iconParams.sizeY, NULL, NULL, hInstance, NULL);
		SetLayeredWindowAttributes(icon,RGB(255,0,255),255,LWA_COLORKEY);
	}
}

void UnmakeIcon(HWND icon, bool force=false)
{
	IconData* lpIconData=(IconData *)GetWindowLong(icon,0);
	IconData &iconData=*lpIconData;
	if (force||(!IsIconic(iconData.target)))
	{
		BroadcastSystemMessage(0,NULL,msgBroadcast,(WPARAM)iconData.iconNumber,(LPARAM)DM_ICONREMOVED);
		RemoveSticky(icon);
		delete lpIconData;
		DestroyWindow(icon);
		numIcons--;
	}
}

void ActivateIcon(HWND icon)
{
	IconData* lpIconData=(IconData *)GetWindowLongPtr(icon,0);
	IconData &iconData=*lpIconData;
	ShowWindow(iconData.target,SW_RESTORE);
	UnmakeIcon(icon);
}

void updateIconAppearance()
{
	HDC tempDC = CreateCompatibleDC(NULL);
	HGDIOBJ otherFont;
	otherFont = SelectObject(tempDC,currentStyle.ToolbarFont);
	SIZE fontSize;
	GetTextExtentPoint32(tempDC,"()[]{}Testy",11,&fontSize);
	SelectObject(tempDC,otherFont);
	DeleteDC(tempDC);
	iconParams.sizeY = iconParams.iconSize+2*iconParams.iconPadding+(iconParams.drawBorder?2*currentStyle.ToolbarStyle.borderWidth:0);
	if (iconParams.caption)
	{
		iconParams.sizeY +=fontSize.cy+2;
	}
	iconParams.sizeX = iconParams.iconSize+2*iconParams.iconPadding;
	iconParams.sizeX+= (iconParams.textWidth>iconParams.sizeX?iconParams.textWidth-iconParams.sizeX:0)+(iconParams.drawBorder?2*currentStyle.ToolbarStyle.borderWidth:0);
	iconParams.iconsPerRow = (workspace.right-workspace.left-iconParams.spacingX)/(iconParams.sizeX+iconParams.spacingX);
	iconParams.iconsPerCol = (workspace.bottom-workspace.top-iconParams.spacingY)/(iconParams.sizeY+iconParams.spacingY);
	BroadcastSystemMessage(0,NULL,msgBroadcast,0,(LPARAM)DM_RECONFIGURE);
}



void placeIcon(HWND icon)
{
	IconData* lpIconData=(IconData *)GetWindowLong(icon,0);
	IconData &iconData=*lpIconData;
	SetWindowPos(icon,HWND_BOTTOM,getPosX(iconData.iconNumber),getPosY(iconData.iconNumber),iconParams.sizeX,iconParams.sizeY,SWP_NOACTIVATE|SWP_NOZORDER);
}

LPCSTR pluginInfo(int x)
{
	switch (x)
	{
	case PLUGIN_NAME:
		return "bbDeskMinimize";
	case PLUGIN_VERSION:
		return "0.0.3";
	case PLUGIN_AUTHOR:
		return "Carsomyr";
	case PLUGIN_RELEASEDATE:
		return "2006/04/13";
	case PLUGIN_LINK:
		return "None";
	case PLUGIN_EMAIL:
		return "s23120712@tuks.co.za";
	default:
		return "bbDeskMinimize 0.0.3";
	}
}

LRESULT CALLBACK PluginProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_ACTIVATED:
			if (IsIconic(lastActive)) MakeIcon(lastActive);
			lastActive=(HWND)wParam;
			break;
		}
		break;
	case BB_RECONFIGURE:
		currentStyle.ToolbarStyle=*((StyleItem *)GetSettingPtr(SN_TOOLBAR));
		currentStyle.ToolbarFont = CreateStyleFont(&currentStyle.ToolbarStyle);
		ReadRCSettings();
		updateIconAppearance();
		break;
	case WM_CREATE:
		SendMessage(GetBBWnd(), BB_REGISTERMESSAGE, (WPARAM)hwnd, (LPARAM)pluginMessages);
		break;
	case WM_DESTROY:
		SendMessage(GetBBWnd(), BB_UNREGISTERMESSAGE, (WPARAM)hwnd, (LPARAM)pluginMessages);
		break;
	case DM_CREATEICON:
		MakeIcon((HWND)wParam);
		break;
	default:
		if (message==msgBroadcast)
			switch ((int)lParam)
			{
			case DM_KILL:
				DestroyWindow(hwnd);
				break;
			}
		else
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK IconWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_REMOVED:
		case TASKITEM_ACTIVATED:
		{
			IconData* lpIconData=(IconData *)GetWindowLong(hwnd,0);
			IconData &iconData=*lpIconData;
			if ((HWND)wParam==iconData.target)
				UnmakeIcon(hwnd);
		}
		break;
		}
		return 0;
	case WM_CREATE:
	{
		SendMessage(GetBBWnd(),BB_REGISTERMESSAGE, (WPARAM)hwnd, (LPARAM)iconMessages);
		MakeSticky(hwnd);
		IconData *lpIconData = new IconData;
		IconData &iconData=*lpIconData;
		iconData.target = newIconTask;
		iconData.iconNumber = numIcons++;
		SetWindowLongPtr(hwnd,0,(LONG_PTR)lpIconData);
		BroadcastSystemMessage(0,NULL,msgBroadcast,(WPARAM)iconData.target,(LPARAM)DM_CHECKWND);
	}
	break;
	case WM_DESTROY:
		SendMessage(GetBBWnd(),BB_UNREGISTERMESSAGE, (WPARAM)hwnd, (LPARAM)iconMessages);
		return DefWindowProc(hwnd,message,wParam,lParam);
	case WM_PAINT:
	{
		IconData* lpIconData=(IconData *)GetWindowLong(hwnd,0);
		IconData &iconData=*lpIconData;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HDC buf = CreateCompatibleDC(hdc);
		HGDIOBJ otherbmp,bufbmp,otherFont,myfont;
		RECT r;
		GetClientRect(hwnd,&r);
		SetBkMode(buf, TRANSPARENT);
		bufbmp = CreateCompatibleBitmap(hdc, r.right+1, r.bottom+1);
		otherbmp = SelectObject(buf, bufbmp);
		MakeStyleGradient(buf, &r, &currentStyle.ToolbarStyle, iconParams.drawBorder);
		HICON tempIcon = (HICON)SendMessage(iconData.target,WM_GETICON,ICON_BIG,0);
		if (!tempIcon) tempIcon = (HICON)GetClassLongPtr(iconData.target,GCLP_HICON);
		if (!tempIcon) tempIcon = (HICON)SendMessage(iconData.target,WM_GETICON,ICON_SMALL,0);
		if (!tempIcon) tempIcon = (HICON)GetClassLongPtr(iconData.target,GCLP_HICONSM);
		HICON drawIcon=CopyIcon(tempIcon);
		DrawIconEx(buf,iconParams.iconPadding+(iconParams.drawBorder?currentStyle.ToolbarStyle.borderWidth:0),iconParams.iconPadding+(iconParams.drawBorder?currentStyle.ToolbarStyle.borderWidth:0),drawIcon,iconParams.iconSize,iconParams.iconSize,0,NULL,DI_NORMAL);
		char displayText[100];
		UINT stringSize = SendMessage(iconData.target,WM_GETTEXT,99,(LPARAM)displayText);
		displayText[stringSize]=0;
		r.left+=2+(iconParams.drawBorder?currentStyle.ToolbarStyle.borderWidth:0);
		r.bottom-=2+(iconParams.drawBorder?currentStyle.ToolbarStyle.borderWidth:0);
		r.right-=2+(iconParams.drawBorder?currentStyle.ToolbarStyle.borderWidth:0);
		myfont=CreateStyleFont(&currentStyle.ToolbarStyle);
		otherFont=SelectObject(buf,myfont);
		//MessageBox(NULL,mess,"Text Size",MB_OK);
		INT numChars;
		SIZE fSize;
		GetTextExtentExPoint(buf,displayText,stringSize,r.right-r.left,&numChars,NULL,&fSize);
		UINT format=DT_BOTTOM|DT_SINGLELINE|DT_CENTER;
		SetTextColor(buf,currentStyle.ToolbarStyle.TextColor);
		if (iconParams.caption)
		{
			DrawText(buf, displayText, numChars, &r, format);
		}
		SelectObject(buf,otherFont);
		BitBlt(hdc,
			   ps.rcPaint.left,
			   ps.rcPaint.top,
			   ps.rcPaint.right  - ps.rcPaint.left,
			   ps.rcPaint.bottom - ps.rcPaint.top,
			   buf,
			   ps.rcPaint.left,
			   ps.rcPaint.top,
			   SRCCOPY
			  );
		// Put back the previous default bitmap
		SelectObject(buf, otherbmp);
		DeleteObject(myfont);
		DeleteObject(bufbmp);
		DeleteDC(buf);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
		ActivateIcon(hwnd);
		break;
	default:
		if (message==msgBroadcast)
		{
			switch (lParam)
			{
			case DM_CHECKWND:
			{
				IconData* lpIconData=(IconData *)GetWindowLongPtr(hwnd,0);
				IconData &iconData=*lpIconData;
				if ((HWND)wParam==iconData.target)
					BroadcastSystemMessage(0,NULL,msgBroadcast,wParam,(LPARAM)iconData.iconNumber);
			}
			break;
			case DM_RECONFIGURE:
				RECT wndRect;
				GetWindowRect(hwnd,&wndRect);
				InvalidateRect(hwnd,&wndRect,TRUE);
				placeIcon(hwnd);
				break;
			case DM_ICONREMOVED:
			{
				IconData* lpIconData=(IconData *)GetWindowLong(hwnd,0);
				IconData &iconData=*lpIconData;
				if (iconData.iconNumber>(LONG)wParam)
				{
					iconData.iconNumber--;
					SetWindowLongPtr(hwnd,0,(LONG_PTR)lpIconData);
				}
				placeIcon(hwnd);
			}
			break;
			case DM_KILL:
				DestroyWindow(hwnd);
				RemoveSticky(hwnd);
				break;
			default:
			{
				IconData* lpIconData=(IconData *)GetWindowLongPtr(hwnd,0);
				IconData &iconData=*lpIconData;
				if (((HWND)wParam==iconData.target)&&((int)lParam<iconData.iconNumber))
					UnmakeIcon(hwnd,true);
			}
			}
			break;
		}
		else
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

int beginPluginEx(HINSTANCE h_instance, HWND hSlit)
{
	return beginPlugin(h_instance);
}

int beginSlitPlugin(HINSTANCE h_instance, HWND hSlit)
{
	return beginPlugin(h_instance);
}

int beginPlugin(HINSTANCE h_instance)
{
	//InitCommonControls();
	SystemParametersInfo(SPI_GETWORKAREA,0,&workspace,0);
	hInstance=h_instance;
	msgBroadcast=RegisterWindowMessage("DESKMIN_BROADCAST");
	if (!strnicmp(GetBBVersion(),"bblean",6))
		branch=BBLEAN;
	else if (!strnicmp(GetBBVersion(),"0.",2))
		branch=BB4WIN;
	else
		branch=XOBLITE;
	ReadRCSettings();
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = IconWndProc;
	wc.hInstance = h_instance;
	wc.cbWndExtra=sizeof(void*);
	wc.lpszClassName = szIconClass;
	RegisterClass(&wc);
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = PluginProc;
	wc.hInstance = h_instance;
	wc.cbWndExtra=8;
	wc.lpszClassName = szPluginClass;
	RegisterClass(&wc);
	hPluginWnd=CreateWindowEx(WS_EX_TOOLWINDOW, szPluginClass, szPluginClass, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	currentStyle.ToolbarStyle=*((StyleItem *)GetSettingPtr(SN_TOOLBAR));
	currentStyle.ToolbarFont = CreateStyleFont(&currentStyle.ToolbarStyle);
	updateIconAppearance();
	lastActive=GetActiveWindow();
	for (int i=0;i<GetTaskListSize();++i)
	{
		HWND task=GetTask(i);
		if (IsIconic(task))
		{
			MakeIcon(task);
		}
	}
	hProcess = GetCurrentProcess();
	return 0;
}

//---------------------------------------------------------------------------------

void endPlugin(HINSTANCE h_instance)
{
	BroadcastSystemMessage(0,NULL,msgBroadcast,0,(LPARAM)DM_KILL);
	while (!UnregisterClass(szIconClass, h_instance));
	while (!UnregisterClass(szPluginClass, h_instance));
}

//---------------------------------------------------------------------------------
void ReadRCSettings(void)
{
	char name[MAX_PATH];
	int i = 0;
	GetModuleFileName(hInstance, myPath, sizeof(myPath));
	GetModuleFileName(hInstance, hookPath, sizeof(hookPath));
	*(strrchr(hookPath,'\\')+1)=0;
	strcat(hookPath,"minimizeHook.dll");
	do
	{
		// First and third, we look for the config file
		// in the same folder as the plugin...
		HINSTANCE hInst = hInstance;
		//Allow us to get the name of dll used
		GetModuleFileName(hInst, rcpath, sizeof(rcpath));
		char *plugin_name = strrchr(rcpath, '\\');
		plugin_name++;
		char *extension_start = strrchr(plugin_name, '.');
		extension_start++;
		extension_start[0]='r';
		extension_start[1]='c';
		extension_start[2]=0;
		strcpy(name,plugin_name);
		// second we check the blackbox directory
		if (1 == i) hInst = NULL;

		GetModuleFileName(hInst, rcpath, sizeof(rcpath));
		char *file_name_start = strrchr(rcpath, '\\');

		if (file_name_start) ++file_name_start;
		else file_name_start = strchr(rcpath, 0);
		strcpy(file_name_start, name);
	}
	while (++i < 3 && false == FileExists(rcpath));
	if (ReadBool(rcpath,"bbDeskMinimize.Area.RespectMargins:",true))
		SystemParametersInfo(SPI_GETWORKAREA,0,&workspace,0);
	else
	{
		GetMonitorRect(NULL,&workspace,0);
	}
	workspace.top = ReadInt(rcpath,"bbDeskMinimize.Area.Top:",workspace.top);
	workspace.left = ReadInt(rcpath,"bbDeskMinimize.Area.Left:",workspace.left);
	workspace.bottom = ReadInt(rcpath,"bbDeskMinimize.Area.Bottom:",workspace.bottom);
	workspace.right = ReadInt(rcpath,"bbDeskMinimize.Area.Right:",workspace.right);

	iconParams.spacingX = ReadInt(rcpath,"bbDeskMinimize.IconSpacing.X:",10);
	iconParams.spacingY = ReadInt(rcpath,"bbDeskMinimize.IconSpacing.Y:",iconParams.spacingX);
	iconParams.iconSize = ReadInt(rcpath,"bbDeskMinimize.IconSize:",32);
	iconParams.iconPadding = ReadInt(rcpath,"bbDeskMinimize.IconPadding:",5);

	iconParams.caption = ReadBool(rcpath,"bbDeskMinimize.Caption:",true);

	iconParams.drawBorder = ReadBool(rcpath,"bbDeskMinimize.DrawBorder:",true);

	if (ReadBool(rcpath,"bbDeskMinimize.Placement.Horizontal:",false))
		orientation = ORIENT_HORIZONTAL;
	else
		orientation = ORIENT_VERTICAL;
	if (ReadBool(rcpath,"bbDeskMinimize.Placement.StartRight:",false))
	{
		directionX=-1;
		startX=workspace.right;
	}
	else
	{
		directionX=1;
		startX=workspace.left;
	}
	if (ReadBool(rcpath,"bbDeskMinimize.Placement.StartBottom:",false))
	{
		directionY=-1;
		startY=workspace.bottom;
	}
	else
	{
		directionY=1;
		startY=workspace.top;
	}
}
