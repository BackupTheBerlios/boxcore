#ifndef CLSUSER32_H
#define CLSUSER32_H

#include <windows.h>

typedef BOOL (WINAPI *fnAllowSetForegroundWindow)(DWORD dwProcessId);
typedef BOOL (WINAPI *fnTrackMouseEvent)(LPTRACKMOUSEEVENT lpEventTrack);
typedef BOOL (WINAPI *fnUpdateLayeredWindow)(HWND hwnd, HDC hdcDest, POINT *pptDest, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pBlend, DWORD dwFlags);
typedef BOOL (WINAPI *fnGetMonitorInfo)(HMONITOR hMonitor, LPMONITORINFO lpmi);
typedef HMONITOR (WINAPI *fnMonitorFromWindow)(HWND hwnd, DWORD dwFlags);

class clsUser32
{
public:
	clsUser32();
	~clsUser32();

	fnAllowSetForegroundWindow AllowSetForegroundWindow;
	fnTrackMouseEvent TrackMouseEvent;
	fnUpdateLayeredWindow UpdateLayeredWindow;
	fnGetMonitorInfo GetMonitorInfoA;
	fnMonitorFromWindow MonitorFromWindow;

protected:
	HMODULE hModule;

private:
};

extern clsUser32 user32;

#endif // CLSUSER32_H
