/*
 ============================================================================

  This file is part of the bbLeanSkin source code.
  Copyright � 2003 grischka (grischka@users.sourceforge.net)

  bbLeanSkin is a plugin for Blackbox for Windows

  http://bb4win.sourceforge.net/bblean
  http://bb4win.sourceforge.net/


  bbLeanSkin is free software, released under the GNU General Public License
  (GPL version 2 or later) For details see:

	http://www.fsf.org/licenses/gpl.html

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/ // subclass.h

// This file contains information private to the skinner

#define NUMOFGDIOBJS (11+1) // 11 bitmaps, 1 font
#define hFont gdiobjs[11]

struct SizeInfo
{
	int width, height;
	int HiddenTop;
	int HiddenBottom;
	int HiddenSide;
	int BottomAdjust;
	RECT rcClient;
};

struct WinInfo
{
	HMODULE hModule;
	HWND hwnd;
	LRESULT (WINAPI *pCallWindowProc)(WNDPROC,HWND,UINT,WPARAM,LPARAM);
	WNDPROC wpOrigWindowProc;
	LONG style, exstyle;

	SizeInfo S;

	HGDIOBJ gdiobjs[NUMOFGDIOBJS];
	HDC buf;

	bool is_unicode;
	bool apply_skin;
	bool dont_draw;
	bool in_set_region;

	bool is_active;
	bool is_zoomed;
	bool is_iconic;
	bool is_moving;
	bool is_transparentdrag;

	bool is_rolled;
	bool is_ontop;
	bool is_sticky;

	int right_btn;
	char button_set[15];
	char capture_button;
	char button_down;
	char bbsm_option;
};

enum
{
	btn_None      = -1,

	btn_Close     = 0,
	btn_Max       = 1,
	btn_Min       = 2,
	btn_Rollup    = 3,
	btn_OnTop     = 4,
	btn_Sticky    = 5,
	btn_Icon      = 6,

	btn_Lower     = 7,
	btn_VMax        ,
	btn_HMax        ,

	btn_Caption     ,
	btn_Nowhere     ,

	btn_Topleft     ,
	btn_Topright    ,
	btn_Top
};

//-----------------------------------------------------------------

#define IMAGEDITHER mSkin.imageDither
#define ISNEWSTYLE mSkin.isNewStyle

extern HINSTANCE hInstance;
extern unsigned bbSkinMsg;
extern SkinStruct mSkin;

bool GetSkin(void);
void send_log(HWND, char *msg);
void subclass_window(HWND hwnd);
HWND GetRootWindow(HWND hwnd);

#define get_WinInfo(hwnd) ((struct WinInfo*)GetProp(hwnd, BBLEANSKIN_INFOPROP))
#define set_WinInfo(hwnd, WI) SetProp(hwnd, BBLEANSKIN_INFOPROP, WI)
#define del_WinInfo(hwnd) RemoveProp(hwnd, BBLEANSKIN_INFOPROP)

LRESULT APIENTRY WindowSubclassProc(HWND, UINT, WPARAM, LPARAM);

#define CALLORIGWINDOWPROC(hwnd, msg, wp, lp) \
	WI->pCallWindowProc(WI->wpOrigWindowProc, hwnd, msg, wp, lp)

//-----------------------------------------------------------------
