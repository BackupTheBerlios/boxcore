#include "guidSources.h"

clsidRegValues::clsidRegValues(const wstring &pRegKeyName)
{
	if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, pRegKeyName.c_str(), 0, KEY_READ, &regKey))
	{
		keyValid=false;
	}
	else
	{
		keyValid=true;
		keyName=pRegKeyName;
	}
	index=0;
}

clsidRegValues::~clsidRegValues()
{
	if (keyValid)
		RegCloseKey(regKey);
}

CLSID clsidRegValues::getNextCLSID()
{
	WCHAR szValueName[100]; WCHAR szClsid[200];
	DWORD cbValueName   = sizeof szValueName;
	DWORD cbClsid        = sizeof szClsid;
	DWORD dwDataType;

	if (ERROR_SUCCESS != RegEnumValueW(regKey, index, szValueName, &cbValueName, 0,
	    &dwDataType, (LPBYTE) szClsid, &cbClsid))
		return CLSID_NULL;

	CLSID clsid;
	CLSIDFromString(szClsid, &clsid);
	index++;
	return clsid;
}

clsidRegKeys::clsidRegKeys(const wstring &pRegKeyName)
{
	if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, pRegKeyName.c_str(), 0, KEY_READ, &regKey))
	{
		keyValid=false;
	}
	else
	{
		keyValid=true;
		keyName=pRegKeyName;
	}
	index=0;
}

clsidRegKeys::~clsidRegKeys()
{
	if (keyValid)
		RegCloseKey(regKey);
}

CLSID clsidRegKeys::getNextCLSID()
{
	WCHAR szCLSID[100];
	DWORD cbCLSID   = sizeof szCLSID;

	if (ERROR_SUCCESS != RegEnumKeyExW(regKey, index, szCLSID, &cbCLSID, NULL, NULL, NULL, NULL))
		return CLSID_NULL;

	CLSID clsid;
	CLSIDFromString(szCLSID, &clsid);
	index++;
	return clsid;
}

clsidInjected::clsidInjected(const wstring &pClsidString)
{
	WCHAR clsidString[pClsidString.size()+1];
	pClsidString.copy(clsidString,pClsidString.size(),0);
	CLSIDFromString(clsidString, &inject);

	used=false;
}

CLSID clsidInjected::getNextCLSID()
{
	if (used)
		return CLSID_NULL;
	else
	{
		used=true;
		return inject;
	}
}
