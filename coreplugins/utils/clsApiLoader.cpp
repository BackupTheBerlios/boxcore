#include "clsApiLoader.h"

clsApiLoader::clsApiLoader()
{
	hBlackbox = NULL;
}

clsApiLoader::~clsApiLoader()
{

}

bool clsApiLoader::requestApiPresence(CONST TCHAR *pIdentifier)
{
	return GlobalFindAtom(pIdentifier);
}

FARPROC clsApiLoader::requestApiPointer(CONST CHAR *pApiName)
{
	if (!hBlackbox)
	{
		hBlackbox = GetModuleHandle(NULL);
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

