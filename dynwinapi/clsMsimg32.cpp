#include "clsMsimg32.h"

clsMsimg32::clsMsimg32()
{
	hModule = LoadLibrary(TEXT("msimg32.dll"));
	if (hModule)
	{
		AlphaBlend = (fnAlphaBlend)GetProcAddress(hModule, "AlphaBlend");
	}
	else
	{
		AlphaBlend = NULL;
	}
}

clsMsimg32::~clsMsimg32()
{
	if (hModule)
		FreeLibrary(hModule);
}
