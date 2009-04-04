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
#include <tchar.h>

#include "clsBoxBar.h"

clsBoxBar boxBarPlug;

// ----------------------------------
// plugin info

LPCSTR szVersion        = PLUGININFO_NAME " " PLUGININFO_VERSION;
LPCSTR szAppName        = PLUGININFO_NAME;
LPCSTR szInfoVersion    = PLUGININFO_VERSION;
LPCSTR szInfoAuthor     = PLUGININFO_AUTHOR;
LPCSTR szInfoRelDate    =  __DATE__ ;
LPCSTR szInfoLink       = PLUGININFO_URL;
LPCSTR szInfoEmail      = PLUGININFO_EMAIL;

int beginSlitPlugin(HINSTANCE hPluginInstance, HWND hSlit)
{
	return boxBarPlug.beginPluginEx(hPluginInstance, hSlit);
}

int beginPluginEx(HINSTANCE hPluginInstance, HWND hSlit)
{
	return boxBarPlug.beginPluginEx(hPluginInstance, hSlit);
}

int beginPlugin(HINSTANCE hPluginInstance)
{
	return boxBarPlug.beginPluginEx(hPluginInstance, NULL);
}

void endPlugin(HINSTANCE hPluginInstance)
{
	boxBarPlug.endPlugin(hPluginInstance);
}

LPCSTR pluginInfo(int field)
{
	switch (field)
	{
	case PLUGIN_NAME:
		return szAppName;
	case PLUGIN_VERSION:
		return szInfoVersion;
	case PLUGIN_AUTHOR:
		return szInfoAuthor;
	case PLUGIN_RELEASE:
		return szInfoRelDate;
	case PLUGIN_LINK:
		return szInfoLink;
	case PLUGIN_EMAIL:
		return szInfoEmail;
	default:
		return szVersion;
	}
}
