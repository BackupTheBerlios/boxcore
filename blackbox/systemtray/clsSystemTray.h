#ifndef CLSSYSTEMTRAY_H
#define CLSSYSTEMTRAY_H
#include "windows.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

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
		const string trayWndName;
		HWND hTrayWnd;
		void createTrayChild(const string pParentClass, const string pChildClass);

		UINT trayCreatedMessage;

		vector<string> childClasses;

		HMODULE hUser32;
		BOOL (*ChangeWindowMessageFilter)(UINT message, DWORD dwFlag);
};

#endif // CLSSYSTEMTRAY_H
