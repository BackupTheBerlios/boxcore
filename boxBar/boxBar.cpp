/*
 ============================================================================
  SDK example plugin for Blackbox for Windows.
  Copyright © 2004 grischka

  This program is free software, released under the GNU General Public
  License (GPL version 2 or later). See:

  http://www.fsf.org/licenses/gpl.html

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

 ============================================================================

  Description:

  This is an example Slit-Plugin for Blackbox for Windows. It displays
  a little stylized window with an inscription.

  The plugin window can be moved with:
	- left mouse and the control key held down,

  and resized with
	- left mouse and the alt key held down

  Some standard plugin window properties can be configured with the
  right-click menu, also the inscription text can be set.

 ============================================================================
*/

#include "version.h"
#include "BBApi.h"
#include <stdlib.h>
#include <tchar.h>

#include "clsBoxBar.h"

clsBoxBar boxBar;

// ----------------------------------
// plugin info

LPCSTR szVersion        = PLUGININFO_NAME " " PLUGININFO_VERSION;
LPCSTR szAppName        = PLUGININFO_NAME;
LPCSTR szInfoVersion    = PLUGININFO_VERSION;
LPCSTR szInfoAuthor     = PLUGININFO_AUTHOR;
LPCSTR szInfoRelDate    =  __DATE__ ;
LPCSTR szInfoLink       = PLUGININFO_URL;
LPCSTR szInfoEmail      = PLUGININFO_EMAIL;

// ----------------------------------
// The About MessageBox

void about_box(void)
{
	TCHAR szTemp[1000];
	_stprintf(szTemp,
			  TEXT("%s - A replacement system bar for BB4win.")
			  TEXT("\n")
			  TEXT("\n© 2008 %s")
			  TEXT("\n%s")
			  , szVersion, szInfoEmail, szInfoLink
			 );
	MessageBox(NULL, szTemp, TEXT("About"), MB_OK | MB_TOPMOST);
}

// ----------------------------------
// Interface declaration

extern "C"
{
	DLL_EXPORT int beginPlugin(HINSTANCE hPluginInstance);
	DLL_EXPORT int beginSlitPlugin(HINSTANCE hPluginInstance, HWND hSlit);
	DLL_EXPORT int beginPluginEx(HINSTANCE hPluginInstance, HWND hSlit);
	DLL_EXPORT void endPlugin(HINSTANCE hPluginInstance);
	DLL_EXPORT LPCSTR pluginInfo(int field);
};

// ----------------------------------
// Global vars

HINSTANCE hInstance;
HWND BBhwnd;
HWND hSlit_present;
HRGN RoundCorners;
bool is_bblean;

// receives the path to "bbSDK.rc"
CHAR rcpath[MAX_PATH];


// ----------------------------------
// Style info

struct style_info
{
	StyleItem Frame;
	int bevelWidth;
	int borderWidth;
	COLORREF borderColor;
} style_info;

// ----------------------------------
// Plugin window properties

struct plugin_properties
{
	// settings
	int xpos, ypos;
	int width, height;

	bool useSlit;
	bool alwaysOnTop;
	bool snapWindow;
	bool pluginToggle;
	bool alphaEnabled;
	bool drawBorder;
	int  alphaValue;

	// our plugin window
	HWND hwnd;

	// current state variables
	bool is_ontop;
	bool is_moving;
	bool is_sizing;
	bool is_hidden;

	// the Slit window, if we are in it.
	HWND hSlit;

	// GDI objects
	HBITMAP bufbmp;
	HFONT hFont;

	// the text
	TCHAR window_text[100];

} my;

// ----------------------------------
// some function prototypes

void GetStyleSettings();
void ReadRCSettings();
void WriteRCSettings();
void ShowMyMenu(bool popup);
void invalidate_window(void);
void set_window_modes(void);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ----------------------------------
// helper to handle commands  from the menu

void eval_menu_cmd(int mode, void *pValue, const char *sub_message);
enum eval_menu_cmd_modes
{
	M_BOL = 1,
	M_INT = 2,
	M_STR = 3,
};

//*****************************************************************************
// utilities

// case insensitive string compare, up to lenght of second string
int my_substr_icmp(const char *a, const char *b)
{
	return memicmp(a, b, strlen(b));
}

// debugging (checkout "DBGVIEW" from "http://www.sysinternals.com/")
void dbg_printf (const TCHAR *fmt, ...)
{
	TCHAR buffer[4096];
	va_list arg;
	va_start(arg, fmt);
	_vstprintf (buffer, fmt, arg);
	OutputDebugString(buffer);
}

//===========================================================================

//===========================================================================
// The startup interface

int beginSlitPlugin(HINSTANCE hPluginInstance, HWND hSlit)
{
	return boxBar.beginPluginEx(hPluginInstance, hSlit);
}

//===========================================================================
// xoblite type slit interface

int beginPluginEx(HINSTANCE hPluginInstance, HWND hSlit)
{
	return boxBar.beginPluginEx(hPluginInstance, hSlit);
}

// no-slit interface
int beginPlugin(HINSTANCE hPluginInstance)
{
	return boxBar.beginPluginEx(hPluginInstance, NULL);
}

//===========================================================================
// on unload...

void endPlugin(HINSTANCE hPluginInstance)
{
	boxBar.endPlugin(hPluginInstance);
}

//===========================================================================
// pluginInfo is used by Blackbox for Windows to fetch information about
// a particular plugin.

LPCSTR pluginInfo(int field)
{
	switch (field)
	{
	case PLUGIN_NAME:
		return szAppName;       // Plugin name
	case PLUGIN_VERSION:
		return szInfoVersion;   // Plugin version
	case PLUGIN_AUTHOR:
		return szInfoAuthor;    // Author
	case PLUGIN_RELEASE:
		return szInfoRelDate;   // Release date, preferably in yyyy-mm-dd format
	case PLUGIN_LINK:
		return szInfoLink;      // Link to author's website
	case PLUGIN_EMAIL:
		return szInfoEmail;     // Author's email
	default:
		return szVersion;       // Fallback: Plugin name + version, e.g. "MyPlugin 1.0"
	}
}


//===========================================================================
// this invalidates the window, and resets the bitmap at the same time.

void invalidate_window(void)
{
	if (my.bufbmp)
	{
		// delete the bitmap, so it will be drawn again
		// next time with WM_PAINT
		DeleteObject(my.bufbmp);
		my.bufbmp = NULL;
	}
	// notify the os that the window needs painting
	InvalidateRect(my.hwnd, NULL, FALSE);
}

//===========================================================================



//===========================================================================

//===========================================================================
// Update position and size, as well as onTop, transparency and inSlit states.

void set_window_modes(void)
{
	HWND hwnd = my.hwnd;

	if (my.useSlit && hSlit_present)
	{
		// if in slit, dont move...
		SetWindowPos(hwnd, NULL,
					 0, 0, my.width, my.height,
					 SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOMOVE
					);

		if (my.hSlit) // already we are
		{
			SendMessage(my.hSlit, SLIT_UPDATE, 0, (LPARAM)hwnd);
		}
		else // enter it
		{
			my.hSlit = hSlit_present;
			SendMessage(my.hSlit, SLIT_ADD, 0, (LPARAM)hwnd);
		}
	}
	else
	{
		if (my.hSlit) // leave it
		{
			SendMessage(my.hSlit, SLIT_REMOVE, 0, (LPARAM)hwnd);
			my.hSlit = NULL;
		}

		HWND hwnd_after = NULL;
		UINT flags = SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOZORDER;

		if (my.is_ontop != my.alwaysOnTop)
		{
			my.is_ontop = my.alwaysOnTop;
			hwnd_after = my.is_ontop ? HWND_TOPMOST : HWND_NOTOPMOST;
			flags = SWP_NOACTIVATE | SWP_NOSENDCHANGING;
		}

		SetWindowPos(hwnd, hwnd_after, my.xpos, my.ypos, my.width, my.height, flags);
		SetTransparency(hwnd, (BYTE)(my.alphaEnabled ? my.alphaValue : 255));
	}

	// window needs drawing
	invalidate_window();
}

//===========================================================================

void ReadRCSettings(void)
{
	int i = 0;
	do
	{
		// First and third, we look for the config file
		// in the same folder as the plugin...
		HINSTANCE hInst = hInstance;
		// second we check the blackbox directory
		if (1 == i) hInst = NULL;

		GetModuleFileNameA(hInst, rcpath, sizeof(rcpath));
		char *file_name_start = strrchr(rcpath, '\\');
		if (file_name_start) ++file_name_start;
		else file_name_start = strchr(rcpath, 0);
		strcpy(file_name_start, "bbSDK.rc");

	} while (++i < 3 && false == FileExists(rcpath));

	// If a config file was found we read the plugin settings from it...
	// ...if not, the ReadXXX functions give us just the defaults.

	my.xpos     = ReadInt(rcpath, "bbSDK.xpos:", 10);
	my.ypos     = ReadInt(rcpath, "bbSDK.ypos:", 10);
	my.width    = ReadInt(rcpath, "bbSDK.width:", 80);
	my.height   = ReadInt(rcpath, "bbSDK.height:", 40);

	my.alphaEnabled     = ReadBool(rcpath, "bbSDK.alphaEnabled:", false);
	my.alphaValue       = ReadInt(rcpath,  "bbSDK.alphaValue:", 192);
	my.alwaysOnTop      = ReadBool(rcpath, "bbSDK.alwaysOntop:", true);
	my.drawBorder       = ReadBool(rcpath, "bbSDK.drawBorder:", true);
	my.snapWindow       = ReadBool(rcpath, "bbSDK.snapWindow:", true);
	my.pluginToggle     = ReadBool(rcpath, "bbSDK.pluginToggle:", true);
	my.useSlit          = ReadBool(rcpath, "bbSDK.useSlit:", true);
#ifndef UNICODE
	strcpy(my.window_text, ReadString(rcpath, "bbSDK.windowText:", szAppName));
#else
	MultiByteToWideChar(CP_ACP, 0, ReadString(rcpath, "bbSDK.windowText:", szAppName), -1, my.window_text, 100);
#endif
}

//===========================================================================

void GetStyleSettings(void)
{
	style_info.Frame = *(StyleItem *)GetSettingPtr(SN_TOOLBAR);
	if (false == (style_info.Frame.validated & VALID_TEXTCOLOR))
		style_info.Frame.TextColor = ((StyleItem *)GetSettingPtr(SN_TOOLBARLABEL))->TextColor;

	style_info.bevelWidth   = *(int*)GetSettingPtr(SN_BEVELWIDTH);
	style_info.borderWidth  = *(int*)GetSettingPtr(SN_BORDERWIDTH);
	style_info.borderColor  = *(COLORREF*)GetSettingPtr(SN_BORDERCOLOR);

	if (my.hFont) DeleteObject(my.hFont);
	my.hFont = CreateStyleFont((StyleItem *)GetSettingPtr(SN_TOOLBAR));
}

//===========================================================================
void ShowMyMenu(bool popup)
{
	Menu *pMenu, *pSub;

	// Create the main menu, with a title and an unique IDString
	pMenu = MakeNamedMenu("bbSDK", "bbSDK_IDMain", popup);

	// Create a submenu, also with title and unique IDString
	pSub = MakeNamedMenu("Configuration", "bbSDK_IDConfig", popup);

	// Insert first Item
	MakeMenuItem(pSub,      "Draw Border",  "@bbSDK.drawBorder", my.drawBorder);

	if (hSlit_present)
		MakeMenuItem(pSub,  "Use Slit",     "@bbSDK.useSlit", my.useSlit);

	if (NULL == my.hSlit)
	{
		// these are only available if outside the slit
		MakeMenuItem(pSub,      "Always On Top",        "@bbSDK.alwaysOnTop", my.alwaysOnTop);
		MakeMenuItem(pSub,      "Snap To Edges",        "@bbSDK.snapWindow", my.snapWindow);
		MakeMenuItem(pSub,      "Toggle With Plugins",  "@bbSDK.pluginToggle", my.pluginToggle);
		MakeMenuItem(pSub,      "Transparent",          "@bbSDK.alphaEnabled", my.alphaEnabled);
		MakeMenuItemInt(pSub,   "Alpha Value",          "@bbSDK.alphaValue", my.alphaValue, 0, 255);
	}
	// Insert the submenu into the main menu
	MakeSubmenu(pMenu, pSub, "Configuration");

	// The configurable text string
	MakeMenuItemString(pMenu,   "Display Text",     "@bbSDK.windowText", "plop");

	// ----------------------------------
	// add an empty line
	MakeMenuNOP(pMenu, NULL);

	// add an entry to let the user edit the setting file
	MakeMenuItem(pMenu, "Edit Settings", "@bbSDK.editRC", false);

	// and an about box
	MakeMenuItem(pMenu, "About", "@bbSDK.About", false);

	// ----------------------------------
	// Finally, show the menu...
	ShowMenu(pMenu);
}

//===========================================================================
// helper to handle commands  from the menu

void eval_menu_cmd(int mode, void *pValue, const char *msg_string)
{
	// Our rc_key prefix:
	const char rc_prefix[] = "bbSDK.";
	const int rc_prefix_len = sizeof rc_prefix - 1; // minus terminating \0

	char rc_string[80];

	// scan for a second argument after a space, like in "AlphaValue 200"
	const char *p = strchr(msg_string, ' ');
	int msg_len = p ? p++ - msg_string : strlen(msg_string);

	// Build the full rc_key. i.e. "bbSDK.<subkey>:"
	strcpy(rc_string, rc_prefix);
	memcpy(rc_string + rc_prefix_len, msg_string, msg_len);
	strcpy(rc_string + rc_prefix_len + msg_len, ":");

	switch (mode)
	{
	case M_BOL: // --- toggle boolean variable ----------------
		*(bool*)pValue = false == *(bool*)pValue;

		// write the new setting to the rc - file
		WriteBool(rcpath, rc_string, *(bool*)pValue);
		break;

	case M_INT: // --- set integer variable -------------------
		if (p)
		{
			*(int*)pValue = atoi(p);

			// write the new setting to the rc - file
			WriteInt(rcpath, rc_string, *(int*)pValue);
		}
		break;

	case M_STR: // --- set string variable -------------------
		if (p)
		{
			strcpy((char*)pValue, p);

			// bb4win 0.0.9x puts it in quotes, remove them...
			if (false == is_bblean) StrRemoveEncap((char*)pValue);

			// write the new setting to the rc - file
			WriteString(rcpath, rc_string, (char*)pValue);
		}
		break;
	}

	// apply new settings
	set_window_modes();

	// and update the menu checkmarks
	ShowMyMenu(false);
}

//*****************************************************************************
