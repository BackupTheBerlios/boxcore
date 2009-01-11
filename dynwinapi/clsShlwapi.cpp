#include "clsShlwapi.h"

clsShlwapi shlwapi;

clsShlwapi::clsShlwapi()
{
	hModule = LoadLibrary(TEXT("shlwapi.dll"));
	if (hModule)
	{
		SHLockShared = (fnSHLockShared)GetProcAddress(hModule, "SHLockShared");
		SHUnlockShared = (fnSHUnlockShared)GetProcAddress(hModule, "SHUnlockShared");
	}
	else
	{
		SHLockShared = NULL;
		SHUnlockShared = NULL;
	}
}

clsShlwapi::~clsShlwapi()
{
	if (hModule)
		FreeLibrary(hModule);
}
