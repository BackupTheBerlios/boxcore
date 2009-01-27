#include "clsShlwapi.h"

clsShlwapi shlwapi;

clsShlwapi::clsShlwapi()
{
	hModule = LoadLibrary(TEXT("shlwapi.dll"));
	if (hModule)
	{
		SHLockShared = (fnSHLockShared)GetProcAddress(hModule, reinterpret_cast<LPCSTR>(8));
		SHUnlockShared = (fnSHUnlockShared)GetProcAddress(hModule, reinterpret_cast<LPCSTR>(9));
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
