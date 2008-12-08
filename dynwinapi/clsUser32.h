#ifndef CLSUSER32_H
#define CLSUSER32_H

#include <windows.h>

typedef WINAPI BOOL (*fnAllowSetForegroundWindow)(DWORD dwProcessId);
typedef WINAPI BOOL (*fnTrackMouseEvent)(LPTRACKMOUSEEVENT lpEventTrack);
typedef WINAPI BOOL (*fnUpdateLayeredWindow)(HWND hwnd, HDC hdcDest, POINT *pptDest, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pBlend, DWORD dwFlags);


class clsUser32
{
	public:
		clsUser32();
		~clsUser32();

	fnAllowSetForegroundWindow AllowSetForegroundWindow;
	fnTrackMouseEvent TrackMouseEvent;
	fnUpdateLayeredWindow UpdateLayeredWindow;

	protected:
		HMODULE hModule;

	private:
};

#endif // CLSUSER32_H
