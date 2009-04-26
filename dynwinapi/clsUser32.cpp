#include "clsUser32.h"

clsUser32 user32;

clsUser32::clsUser32()
{
	hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule)
	{
		AllowSetForegroundWindow = reinterpret_cast<fnAllowSetForegroundWindow>(GetProcAddress(hModule, "AllowSetForegroundWindow"));
		TrackMouseEvent = reinterpret_cast<fnTrackMouseEvent>(GetProcAddress(hModule, "TrackMouseEvent"));
		UpdateLayeredWindow = reinterpret_cast<fnUpdateLayeredWindow>(GetProcAddress(hModule, "UpdateLayeredWindow"));
		GetMonitorInfoA = reinterpret_cast<fnGetMonitorInfo>(GetProcAddress(hModule, "GetMonitorInfoA"));
		MonitorFromWindow = reinterpret_cast<fnMonitorFromWindow>(GetProcAddress(hModule, "MonitorFromWindow"));
	}
	else
	{
		AllowSetForegroundWindow = NULL;
		TrackMouseEvent = NULL;
		UpdateLayeredWindow = NULL;
		GetMonitorInfoA = NULL;
		MonitorFromWindow = NULL;
	}
}

clsUser32::~clsUser32()
{
	if (hModule)
		FreeLibrary(hModule);
}
