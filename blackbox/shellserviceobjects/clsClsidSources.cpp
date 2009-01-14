/** @internal
  * @file clsClsidSources.cpp
  * @brief This file contains the implementation of clsClsidSource, clsClsidRegValues,
  * clsClsidRegKeys and clsClsidInjected
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2008 Carsomyr
  * @par Links:
  * http://developer.berlios.de/projects/boxcore @n
  * @par License:
  * boxCore is free software, released under the GNU General
  * Public License (GPL version 2 or later), with an extension that allows
  * linking of proprietary modules under a controlled interface. This means
  * that plugins etc. are allowed to be released under any license the author
  * wishes. For details see:
  * @par
  * http://www.fsf.org/licenses/gpl.html @n
  * http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface
  * @par
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  */

#include "clsClsidSources.h"

/** @brief Opens a registry key for later use
  *
  * Attempts to open the specified registry key with read permissions. If succesful
  * a handle to the key is stored, as well as the key name. The flag keyValid is
  * set to true on suceesfully opening a key.
  * @param[in] pRegKeyName The registry key to open under HKEY_LOCAL_MACHINE
  */
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

/** @brief Closes the registry key
  */
clsClsidRegValues::~clsClsidRegValues()
{
	if (keyValid)
		RegCloseKey(regKey);
}

/** @brief Fetch the next CLSID from the list
  * @return A CLSID stored under the registry key, or CLSID_NULL once all the values have been used
  */
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

/** @brief Opens a registry key for later use
  * @param[in] pRegKeyName The name of the registry key to open
  */
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

/** @brief Closes the registry key
  */
clsClsidRegKeys::~clsClsidRegKeys()
{
	if (keyValid)
		RegCloseKey(regKey);
}

/** @brief Fetch the next CLSID from the list
  * @return A CLSID stored under the registry key, or CLSID_NULL once all the keys have been used
  */
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

/** @brief Add a CLSID to the whitelist
  * @param[in] pWhitelist A list of CLSID to add to the whitelist in string form
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

/** @brief Tests if a CLSID is in the whitelist
  * @param[in] pClsid The CLSID to test
  * @return true if in whitelist or whitelist is empty. False otherwise
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

/** @brief Converts a string form CLSID into an actual CLSID for later output
  */
clsClsidInjected::clsClsidInjected(const CLSID p_inject)
{
	inject = p_inject;
	used = false;
}

/** @brief Get the next CLSID in the list
  * @return The stored CLSID on the first call, CLSID_NULL on subsequent calls
  */
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


