#include "clsApiLoader.h"

clsApiLoader::clsApiLoader()
{
	hBlackbox = NULL;
}

clsApiLoader::~clsApiLoader()
{
	FreeLibrary(hBlackbox);
}

bool clsApiLoader::requestApiPresence(wstring pIdentifier)
{
	return GlobalFindAtomW(pIdentifier.c_str());
}

FARPROC clsApiLoader::requestApiPointer(string pApiName)
{
	if (!hBlackbox)
	{
		hBlackbox = LoadLibrary(TEXT("blackbox.exe"));
		if (hBlackbox == NULL)
			return NULL;
	}
	return GetProcAddress(hBlackbox, pApiName.c_str());
}
