#include "clsApiLoader.h"

clsApiLoader::clsApiLoader()
{
	hBlackbox = NULL;
}

clsApiLoader::~clsApiLoader()
{

}

bool clsApiLoader::requestApiPresence(TCHAR *pIdentifier)
{
	return GlobalFindAtom(pIdentifier);
}

FARPROC clsApiLoader::requestApiPointer(CHAR *pApiName)
{
	if (!hBlackbox)
	{
		hBlackbox = LoadLibrary(TEXT("blackbox.exe"));
		if (hBlackbox == NULL)
			return NULL;
	}
	return GetProcAddress(hBlackbox, pApiName);
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

