/*
 ============================================================================
 This file is part of the bbLeanSkinMod source code.
 Copyright © 2003-2005 grischka (grischka@users.sourceforge.net)
 Copyright © 2003-2004/2006 The Blackbox for Windows Development Team
 http://bb4win.sourceforge.net/bblean - http://bb4win.org/
 ============================================================================
 Partly based on ShellWM © 2002 ShellWM Development Team
 Partly based on CustomEyes © 1999-2001 Didier Abderrahmane
 ============================================================================

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  http://www.fsf.org/licenses/gpl.html

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 ============================================================================
*/

#ifndef __BBLEANSKINMOD_H
#define __BBLEANSKINMOD_H

//============================================================================

#include "BBApi.h"
#include "../bbLeanSkinEng/hookinfo.h"

// bbLean BBAPI.h proprietary defines (if not already defined)
#ifndef BB_REDRAWGUI
  #define BB_REDRAWGUI            10881

  #define BBRG_TOOLBAR (1<<0)
  #define BBRG_MENU    (1<<1)
  #define BBRG_WINDOW  (1<<2)
  #define BBRG_DESK    (1<<3)
  #define BBRG_FOCUS   (1<<4)
  #define BBRG_PRESSED (1<<5)
  #define BBRG_STICKY  (1<<6)
  #define BBRG_FOLDER  (1<<7)

  #define VALID_MARGIN        (1<<9)
#endif

//============================================================================
// exported names

#if 0
extern "C"
{
    DLL_EXPORT void startEngine();
    DLL_EXPORT void stopEngine();
    DLL_EXPORT void reconfigureEngine(void);
    DLL_EXPORT const char* engineInfo(int field);
    DLL_EXPORT void setEngineOption(UINT id);
};
#endif

//============================================================================

	// blackbox and logwindow stuff
    HINSTANCE hInstance;
    HWND BBhwnd;
    int BBVersion;
    HWND m_hwnd;
    HWND hwndLog;
    bool is_plugin;
    bool enableLog;

	// settings
    bool applyToOpen;
    bool adjustCaptionHeight;
    char rcpath[MAX_PATH];

	// additional windows options
    char windows_menu_fontFace[120];
    int windows_menu_fontHeight;
    int ScrollbarSize;
    int MenuHeight;
    bool setTTColor;

	// skin info passed via shared mem
    UINT bbSkinMsg;
    SkinStruct mSkin;

	// for the shared memory
    HANDLE hMapObject;
    SkinStruct *lpvMem;

	// skinner dll
    bool engine_running;

	//====================

	// forward declaration

	BOOL CreateSharedMem(int size);
	void DestroySharedMem();

    void refreshStyle(void);

    void startEngine();
    void stopEngine();
    void reconfigureEngine(void);
    const char* engineInfo(int field);
    void setEngineOption(UINT id);

    void free_dll(void);
    bool load_dll(void);

	void compatibleReadStyle();
	void ReadStyleElement(LPSTR styleFile, GradientItem* gradientItem, LPSTR paramString);
	void ReadFont(LPSTR styleFile);
	static int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, int FontType, LPARAM lParam);

//===========================================================================

// Exported function names for MSVC
extern "C"
{
	__declspec(dllexport) int beginPlugin(HINSTANCE hMainInstance);
	__declspec(dllexport) int beginPluginEx(HINSTANCE hMainInstance, HWND hBBSlit);
	__declspec(dllexport) void endPlugin(HINSTANCE hMainInstance);
	__declspec(dllexport) LPCSTR pluginInfo(int field);
}

//============================================================================

#endif /* __BBLEANSKINMOD_H */
