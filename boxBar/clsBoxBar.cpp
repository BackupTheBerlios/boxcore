#include "clsBoxBar.h"
#include "BBApi.h"

clsBoxBar::clsBoxBar()
{
	//ctor
}

clsBoxBar::~clsBoxBar()
{
	//dtor
}

/** @brief beginPluginEx
  *
  * @todo: document this function
  */
int clsBoxBar::beginPluginEx(HINSTANCE hPluginInstance, HWND hSlit)
{
	started = false;
	TCHAR pluginPath[MAX_PATH];
	GetModuleFileName(hPluginInstance, pluginPath, MAX_PATH);
	TCHAR *nameStart = _tcsrchr(pluginPath, '\\');
	if (!nameStart)
		nameStart = pluginPath;
	_tcscpy(pluginName, nameStart + 1);
	TCHAR *nameDot = _tcsrchr(pluginName, '.');
	if (nameDot)
		(*nameDot) = '\0';
	if (!FindAtom(pluginName))
	{
		started = true;
		AddAtom(pluginName);
		hBlackboxWnd = GetBBWnd();
		hPlugin = hPluginInstance;
		hSlitWnd   = hSlit;

		//ReadRCSettings();
		//GetStyleSettings();

		// ---------------------------------------------------
		// create the window



		// ---------------------------------------------------
		// set window location and properties

		//set_window_modes();
		//ShowWindow(my.hwnd, SW_SHOWNA);
		bar = new clsBar(pluginName, hPlugin, true);
		return 0;
	}
	else
	{
		TCHAR msg[MAX_PATH];
		_stprintf(msg,TEXT("This plugin is already loaded under this name. To load multiple instances, you must rename the plugin DLL. %d"),GlobalFindAtom(pluginName));
		MessageBox(NULL, msg
				   , pluginName, MB_OK | MB_TOPMOST);
		return 1;
	}
}
/** @brief endPlugin
  *
  * @todo: document this function
  */
void clsBoxBar::endPlugin(HINSTANCE hPluginInstance)
{
	if (started)
	{
		DeleteAtom(FindAtom(pluginName));
		delete bar;
	}
}



