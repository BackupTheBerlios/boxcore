#include "guidSources.h"

clsidRegValues::clsidRegValues(const string &pRegKeyName)
{
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, pRegKeyName.c_str(), 0, KEY_READ, &regKey))
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
	char szValueName[100]; char szData[200];
	DWORD cbValueName   = sizeof szValueName;
	DWORD cbData        = sizeof szData;
	DWORD dwDataType;

	if (ERROR_SUCCESS != RegEnumValue(regKey, index, szValueName, &cbValueName, 0,
	    &dwDataType, (LPBYTE) szData, &cbData))
		return CLSID_NULL;

	WCHAR wszCLSID[sizeof szData];
	MultiByteToWideChar(CP_ACP, 0, szData, cbData, wszCLSID, sizeof szData);

	CLSID clsid;
	CLSIDFromString(wszCLSID, &clsid);
	index++;
	return clsid;
}

clsidRegKeys::clsidRegKeys(const string &pRegKeyName)
{
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, pRegKeyName.c_str(), 0, KEY_READ, &regKey))
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
	char szValueName[100]; char szData[200];
	DWORD cbValueName   = sizeof szValueName;
	DWORD cbData        = sizeof szData;

	if (ERROR_SUCCESS != RegEnumKeyEx(regKey, index, szValueName, &cbValueName, 0, szData, &cbData,NULL))
		return CLSID_NULL;

	WCHAR wszCLSID[sizeof szValueName];
	MultiByteToWideChar(CP_ACP, 0, szValueName, cbValueName, wszCLSID, sizeof szValueName);

	CLSID clsid;
	CLSIDFromString(wszCLSID, &clsid);
	index++;
	return clsid;
}

clsidInjected::clsidInjected(const string &pClsidString)
{
	DWORD stringSize = pClsidString.size()+1;
	WCHAR wszCLSID[stringSize];
	MultiByteToWideChar(CP_ACP, 0, pClsidString.c_str(), stringSize, wszCLSID, pClsidString.size()+1);

	CLSIDFromString(wszCLSID, &inject);

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
