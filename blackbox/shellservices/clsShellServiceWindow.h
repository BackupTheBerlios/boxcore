/** @internal
  * @file
  * @brief This file contains the definition the ShellServiceWindow class.
  *
  * It also contains additional vlaues needed for proper interaction with this class.
  *
  * ShellServiceWindow is part of the Shell Services implementation used by boxCore.
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2008 Carsomyr
  * @par links
  * http://developer.berlios.de/projects/boxcore @n
  * @par License
  * boxCore is free software, released under the GNU General
  * Public License (GPL version 2 or later), with an extension that allows
  * linking of proprietary modules under a controlled interface. This means
  * that plugins etc. are allowed to be released under any license the author
  * wishes. For details see:
  * @par
  * http://www.fsf.org/licenses/gpl.html @n
  * http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface
  * @par
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * The basic operations of this class is based on the functions in Tray.cpp in
  * the bbLean source code.
  */

#ifndef CLSSHELLSERVICEWINDOW_H
#define CLSSHELLSERVICEWINDOW_H

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <map>

#include "clsShellServiceHandler.h"

/** @internal
  * @brief Contains classes and functions which provide some sort of service to the shell
  */
namespace ShellServices
{

/** @internal
  * @brief Handler types for WM_COPYDATA messages to the shell window
  */
enum eShellHandlers
{
	HANDLER_APPBAR, ///< @brief Indicates a handler for messages from SHAppbarMessage()
	HANDLER_NOTIFYICON, ///< @brief Indicates a handler for messages from Shell_Notifyicon()
	HANDLER_RESERVED, ///< @brief Reserved, will implement later
	HANDLER_NOTIFYICONRECT ///< @brief Indicates a handler for messages from Shell_NotifyIconRect()
};

/** @internal
  * @brief Creates a Shell_Traywnd and provides hooks to
  * allow processing of WM_COPYDAYA messages sent to the window.
  *
  */
class ShellServiceWindow
{
public:
	ShellServiceWindow(HINSTANCE pInstance, bool pTopmost);
	virtual ~ShellServiceWindow();

	void RegisterHandler(UINT pType, ShellServiceHandler *pHandler);
	void SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge);
private:
	UINT TaskbarCreated;
	static UINT m_forwarderModuleMsg;
	static HMODULE m_forwarderModule;

	HWND hTrayWnd;
	std::map<UINT, ShellServiceHandler *> handlers;
	HINSTANCE hInstance;

	HMODULE hUser32;
	BOOL (WINAPI *ChangeWindowMessageFilter)(UINT message, DWORD dwFlag);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK childWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void announceWindow();
	HWND createChild(HWND pParent, LPCTSTR pClsName, LPCTSTR p_windowTitle = NULL);
};

}

#endif // CLSSHELLSERVICEWINDOW_H
