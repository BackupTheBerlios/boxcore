#ifndef CLSSYSTEMTRAY_H
#define CLSSYSTEMTRAY_H
#include "windows.h"
#include <string>
#include <vector>

using std::vector;

#ifdef UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

/**
 * @class clsSystemTray
 *
 * @brief Implementes system tray functionality (icon tracking etc)
 */
class clsSystemTray
{
	public:
		clsSystemTray(HINSTANCE &phInstance);
		void initialize();
		void terminate();
		virtual ~clsSystemTray();
		void announceSystemTray();
	protected:
	private:
		static LRESULT CALLBACK trayWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK trayChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		HINSTANCE &hInstance;
		const tstring trayWndName;
		HWND hTrayWnd;
		void createTrayChild(const tstring pParentClass, const tstring pChildClass);

		UINT trayCreatedMessage;

		vector<tstring> childClasses;

		HMODULE hUser32;
		BOOL (*ChangeWindowMessageFilter)(UINT message, DWORD dwFlag);
};

#endif // CLSSYSTEMTRAY_H
