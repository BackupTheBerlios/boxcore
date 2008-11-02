#include "clsSystemTray.h"

#ifndef MSGFLT_ADD
#define MSGFLT_ADD 1
#endif
#ifndef MSGFLT_REMOVE
#define MSGFLT_REMOVE 2
#endif

/**
 * @struct SHELLTRAYDATA
 *
 * @brief Format used by the shell in WM_COPYDATA messages to pass infomration to the system tray
 */
struct SHELLTRAYDATA
{
	DWORD dwMagic; // e.g. 0x34753423;
	DWORD dwMessage;
	NOTIFYICONDATA iconData;
};

/**
 * @brief Performs preperations for system tray creation
 *
 * The constructor registers the TaskbarCreated window message. The window message is also
 * added to the User Interface Privilage Isolation (UIPI) filter if present. This ensures
 * that the message will be able to reach all processes.
 */
clsSystemTray::clsSystemTray(HINSTANCE &phInstance):hInstance(phInstance),trayWndName(TEXT("Shell_TrayWnd"))
{
	trayCreatedMessage=RegisterWindowMessage(TEXT("TaskbarCreated"));
	hUser32 = LoadLibrary(TEXT("user32.dll"));
	if (hUser32)
	{
		ChangeWindowMessageFilter = (BOOL (*)(UINT,DWORD))GetProcAddress(hUser32,"ChangeWindowMessageFilter");
		if (ChangeWindowMessageFilter)
		{
			if(ChangeWindowMessageFilter(trayCreatedMessage,MSGFLT_ADD))
				MessageBox(NULL,TEXT("Message added to filter"),TEXT("UIPI is present"),MB_OK);
			else
				MessageBox(NULL,TEXT("Message not added to filter"),TEXT("UIPI is present"),MB_OK);
		}
		else
		{
			MessageBox(NULL,TEXT("Nothing done"),TEXT("UIPI is not present"),MB_OK);
		}
	}
	else
		ChangeWindowMessageFilter = NULL;
	hTrayWnd = NULL;
	/// @todo Start using ChangeWindowMessageFunction to make sure our message can get through
}

/**
 * @brief Cleans up after the system tray
 *
 * The destructor frees the reference to user32.dll which was used (or attempted to be used)
 * to load the UIPI filter function. The destructor also calls the terminate function to
 * clean up the system tray itself, if this has not already been done.
 */
clsSystemTray::~clsSystemTray()
{
	terminate();
	if (hUser32)
		FreeLibrary(hUser32);
}

/** @brief Performs initalization of the system tray
  *
  * @todo: document this function
  */
void clsSystemTray::initialize()
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass,sizeof(wndClass));
	wndClass.cbSize=sizeof(wndClass);
	wndClass.hInstance=hInstance;
	wndClass.lpfnWndProc=trayWndProc;
	wndClass.lpszClassName=trayWndName.c_str();
	RegisterClassEx(&wndClass);

	hTrayWnd = CreateWindowEx( WS_EX_TOOLWINDOW, trayWndName.c_str(), NULL,
		WS_POPUP|WS_DISABLED, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

	createTrayChild(trayWndName,TEXT("TrayNotifyWnd"));
	createTrayChild(TEXT("TrayNotifyWnd"),TEXT("TrayClockWClass"));
	createTrayChild(TEXT("TrayNotifyWnd"),TEXT("SysPager"));

	announceSystemTray();
}

/** @brief Signals the destruction of the system tray and then cleans up
  *
  * @todo: document this function
  */
void clsSystemTray::terminate()
{
	if (hTrayWnd)
	{
		DestroyWindow(hTrayWnd);
		hTrayWnd = NULL;

		UnregisterClass(trayWndName.c_str(),hInstance);
		for(unsigned int i=0;i<childClasses.size();i++)
			UnregisterClass(childClasses[i].c_str(),hInstance);
	}
}

/** @brief Register a child window class and creates an instance attached to the specified parent
  *
  *
  */
void clsSystemTray::createTrayChild(const tstring pParentClass, const tstring pChildClass)
{
	HWND parent = FindWindow(pParentClass.c_str(),NULL);

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass,sizeof(wndClass));
	wndClass.cbSize=sizeof(wndClass);
	wndClass.hInstance=hInstance;
	wndClass.lpfnWndProc=trayChildWndProc;
	wndClass.lpszClassName=pChildClass.c_str();
	RegisterClassEx(&wndClass);

	CreateWindowEx( 0, pChildClass.c_str(), NULL,
		WS_CHILD|WS_DISABLED, 0, 0, 0, 0, parent, NULL, hInstance, NULL);
	childClasses.push_back(pChildClass);
}


/** @brief announceSystemTray
  *
  * @todo: document this function
  */
void clsSystemTray::announceSystemTray()
{
	SendNotifyMessage(HWND_BROADCAST, trayCreatedMessage, 0, 0);
}

/** @brief Window procedure for the children of the tray
  *
  * Currently just calls DefWindowProc, additional functionality will be implemented if neccesary.
  */
LRESULT CALLBACK clsSystemTray::trayChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/** @brief Window procedure for the system tray window
  *
  * Calls DefWindowProc to handle everything except WM_COPYDATA.
  * @todo: document this function
  */
LRESULT CALLBACK clsSystemTray::trayWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message != WM_COPYDATA)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

