#include "clsUser32.h"

clsUser32::clsUser32()
{
	hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule)
	{
		AllowSetForegroundWindow = (fnAllowSetForegroundWindow)GetProcAddress(hModule, "AllowSetForegroundWindow");
	}
	else
	{
		AllowSetForegroundWindow = NULL;
	}
}

clsUser32::~clsUser32()
{
	if (hModule)
		FreeLibrary(hModule);
}
