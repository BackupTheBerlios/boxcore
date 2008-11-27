#include "clsApiLoader.h"

clsApiLoader::clsApiLoader()
{
	hBlackbox = NULL;
}

clsApiLoader::~clsApiLoader()
{

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

/** @brief freeLibrary
  *
  * @todo: document this function
  */
void clsApiLoader::freeLibrary()
{
	FreeLibrary(hBlackbox);
	hBlackbox = NULL;
}

