#include "clsApiLoader.h"

clsApiLoader::clsApiLoader()
{
	hBlackbox = NULL;
	GetApiExtension = NULL;
}

clsApiLoader::~clsApiLoader()
{
	//dtor
}

const char *clsApiLoader::requestApiName(UINT64 pBranch, UINT64 pApi)
{
	if (!hBlackbox)
	{
		hBlackbox = GetModuleHandle("blackbox.exe");
		if (hBlackbox == NULL)
			return NULL;
	}
	if (!GetApiExtension)
	{
		GetApiExtension = (fnGetApiExtension)GetProcAddress(hBlackbox, "GetApiExtension");
		if (GetApiExtension == NULL)
			return NULL;
	}
	if ( GetApiExtension(pBranch, pApi, apiName) == TRUE)
		return apiName;
	else
		return NULL;
}

FARPROC clsApiLoader::requestApiPointer(string pApiName)
{
	if (!hBlackbox)
	{
		hBlackbox = GetModuleHandle("blackbox.exe");
		if (hBlackbox == NULL)
			return NULL;
	}
	return GetProcAddress(hBlackbox, pApiName.c_str());
}
