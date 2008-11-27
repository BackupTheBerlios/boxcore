#include "clsClsidSources.h"

clsClsidRegValues::clsClsidRegValues(const wstring &pRegKeyName)
{
	if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, pRegKeyName.c_str(), 0, KEY_READ, &regKey))
	{
		keyValid = false;
	}
	else
	{
		keyValid = true;
		keyName = pRegKeyName;
	}
	index = 0;
}

clsClsidRegValues::~clsClsidRegValues()
{
	if (keyValid)
		RegCloseKey(regKey);
}

CLSID clsClsidRegValues::getNextCLSID()
{
	WCHAR szValueName[100];
	WCHAR szClsid[200];
	DWORD cbValueName   = sizeof szValueName;
	DWORD cbClsid        = sizeof szClsid;
	DWORD dwDataType;

	CLSID clsid;
	do
	{
		if (ERROR_SUCCESS != RegEnumValueW(regKey, index, szValueName, &cbValueName, 0,
										   &dwDataType, (LPBYTE) szClsid, &cbClsid))
			return CLSID_NULL;

		CLSIDFromString(szClsid, &clsid);
		index++;
	}
	while (!inWhitelist(clsid));

	return clsid;
}

clsClsidRegKeys::clsClsidRegKeys(const wstring &pRegKeyName)
{
	if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, pRegKeyName.c_str(), 0, KEY_READ, &regKey))
	{
		keyValid = false;
	}
	else
	{
		keyValid = true;
		keyName = pRegKeyName;
	}
	index = 0;
}

clsClsidRegKeys::~clsClsidRegKeys()
{
	if (keyValid)
		RegCloseKey(regKey);
}

CLSID clsClsidRegKeys::getNextCLSID()
{
	WCHAR szCLSID[100];
	DWORD cbCLSID   = sizeof szCLSID;
	CLSID clsid;

	do
	{
	if (ERROR_SUCCESS != RegEnumKeyExW(regKey, index, szCLSID, &cbCLSID, NULL, NULL, NULL, NULL))
		return CLSID_NULL;

	CLSIDFromString(szCLSID, &clsid);
	index++;
	}
	while (!inWhitelist(clsid));

	return clsid;
}

/** @brief setWhitelist
  *
  * @todo: document this function
  */
void clsClsidSource::setWhitelist(vector<wstring> &pWhitelist)
{
	WCHAR clsidString[50];
	CLSID clsid;
	for (unsigned int i = 0;i < pWhitelist.size();++i)
	{
		pWhitelist[i].copy(clsidString, pWhitelist[i].size(), 0);
		CLSIDFromString(clsidString, &clsid);
		whiteList.push_back(clsid);
	}
}

/** @brief inWhitelist
  *
  * @todo: document this function
  */
bool clsClsidSource::inWhitelist(CLSID pClsid)
{
	if (whiteList.size() == 0)
		return true;
	else
	{
		for (unsigned int i = 0;i < whiteList.size();++i)
		{
			if (pClsid == whiteList[i])
				return true;
		}
		return false;
	}
}



clsClsidInjected::clsClsidInjected(const wstring &pClsidString)
{
	WCHAR clsidString[pClsidString.size()+1];
	pClsidString.copy(clsidString, pClsidString.size(), 0);
	CLSIDFromString(clsidString, &inject);

	used = false;
}

CLSID clsClsidInjected::getNextCLSID()
{
	if (used)
		return CLSID_NULL;
	else
	{
		used = true;
		return inject;
	}
}


