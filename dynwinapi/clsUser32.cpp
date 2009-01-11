#include "clsUser32.h"

clsUser32 user32;

clsUser32::clsUser32()
{
	hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule)
	{
		AllowSetForegroundWindow = (fnAllowSetForegroundWindow)GetProcAddress(hModule, "AllowSetForegroundWindow");
		TrackMouseEvent = (fnTrackMouseEvent)GetProcAddress(hModule, "TrackMouseEvent");
		UpdateLayeredWindow = (fnUpdateLayeredWindow)GetProcAddress(hModule, "UpdateLayeredWindow");
	}
	else
	{
		AllowSetForegroundWindow = NULL;
		TrackMouseEvent = NULL;
		UpdateLayeredWindow = NULL;
	}
}

clsUser32::~clsUser32()
{
	if (hModule)
		FreeLibrary(hModule);
}
