#include "clsUser32.h"

clsUser32::clsUser32()
{
	hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule)
	{
		AllowSetForegroundWindow = (fnAllowSetForegroundWindow)GetProcAddress(hModule, "AllowSetForegroundWindow");
		TrackMouseEvent = (fnTrackMouseEvent)GetProcAddress(hModule, "TrackMouseEvent");
	}
	else
	{
		AllowSetForegroundWindow = NULL;
		TrackMouseEvent = NULL;
	}
}

clsUser32::~clsUser32()
{
	if (hModule)
		FreeLibrary(hModule);
}
