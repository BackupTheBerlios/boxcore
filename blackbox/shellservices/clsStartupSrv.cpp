/*
 * clsStartupSrv.cpp
 *
 *  Created on: 20 Apr 2009
 *      Author: shaun
 */

#include "clsStartupSrv.h"
#include <shlobj.h>
#include <regstr.h>
#include <tchar.h>
#include <cstdio>

namespace ShellServices
{

StartupSrv::StartupSrv():
		Service("SRV_Startup"),
		m_vistaOrGreater(true),
		m_isNT(true)
{
	// TODO Auto-generated constructor stub

}

StartupSrv::~StartupSrv()
{
	// TODO Auto-generated destructor stub
}

bool StartupSrv::_Start()
{
	if (IsFirstRunThisSession(TEXT("StartupHasBeenRun")))
	{
		RunShellFolder(CSIDL_COMMON_STARTUP);
		RunShellFolder(CSIDL_STARTUP);
		if (!m_vistaOrGreater)
		{
			RunShellFolder(CSIDL_COMMON_ALTSTARTUP);
			RunShellFolder(CSIDL_ALTSTARTUP);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool StartupSrv::_Stop()
{
	return false;
}

HKEY StartupSrv::_CreateSessionInfoKey()
{
	HKEY hkSessionInfo = NULL;
	HANDLE hToken = NULL;
	PRINT("Creating session key");
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		PRINT("Have token");
		HRESULT hr = E_FAIL;

		TCHAR tzSessionInfo[128] = { 0 };
		DWORD dwOutSize = 0;

		if (m_vistaOrGreater)
		{
			DWORD dwSessionId = 0;

			// On Vista the subkey's name is the Session ID
			if (GetTokenInformation(hToken, TokenSessionId,
									&dwSessionId, sizeof(dwSessionId), &dwOutSize))
			{
				hr = ERROR_SUCCESS;
				PRINT("Got token info");
				_stprintf(tzSessionInfo, REGSTR_PATH_EXPLORER TEXT("\\SessionInfo\\%u"), (UINT)dwSessionId);
				PRINT(tzSessionInfo);
			}
		}
		else
		{
			TOKEN_STATISTICS tsStats = { {0} };

			// Prior to Vista the subkey's name is the AuthenticationId
			if (GetTokenInformation(hToken, TokenStatistics,
									&tsStats, sizeof(tsStats), &dwOutSize))
			{
				hr = ERROR_SUCCESS;
				_stprintf(tzSessionInfo, REGSTR_PATH_EXPLORER TEXT("\\SessionInfo\\%08x%08x"),
						  (UINT)tsStats.AuthenticationId.HighPart,
						  (UINT)tsStats.AuthenticationId.LowPart);
			}
		}

		if (SUCCEEDED(hr))
		{
			// Finally open the SessionInfo key
			LONG lResult = RegCreateKeyEx(
							   HKEY_CURRENT_USER, tzSessionInfo, 0, NULL,
							   REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkSessionInfo, NULL);

			if (lResult != ERROR_SUCCESS)
			{
				hkSessionInfo = NULL;
			}
		}

		CloseHandle(hToken);
	}

	return hkSessionInfo;
}

bool StartupSrv::IsFirstRunThisSession(LPCTSTR pszSubkey)
{
	bool bReturn = false;

	if (m_isNT)
	{
		HKEY hkSessionInfo = _CreateSessionInfoKey();

		if (hkSessionInfo != NULL)
		{
			DWORD dwDisposition;
			HKEY hkStartup;

			LONG lResult = RegCreateKeyEx(
							   hkSessionInfo, pszSubkey, 0, NULL, REG_OPTION_VOLATILE,
							   KEY_WRITE, NULL, &hkStartup, &dwDisposition);

			RegCloseKey(hkStartup);

			if (lResult == ERROR_SUCCESS &&
					dwDisposition == REG_CREATED_NEW_KEY)
			{
				bReturn = true;
			}
		}

		RegCloseKey(hkSessionInfo);
	}
	else
	{
		bReturn = true;
	}

	return bReturn;
}

void StartupSrv::RunShellFolder(UINT p_csidl)
{
	LPITEMIDLIST item;
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, p_csidl, &item)))
	{
		if (SUCCEEDED(SHGetPathFromIDList(item, szPath)))
		{
			if (szPath[_tcslen(szPath)] != TEXT('\\'))
			{
				strcat(szPath, TEXT("\\"));
			}
			TCHAR *pathEnd = szPath + _tcslen(szPath);
			_tcscpy(pathEnd, TEXT("*.*"));
			WIN32_FIND_DATA findData;
			HANDLE hSearch = FindFirstFile(szPath, &findData);
			if (hSearch == INVALID_HANDLE_VALUE) return;
			do
			{
				if (0 == (findData.dwFileAttributes & (
							  FILE_ATTRIBUTE_SYSTEM
							  |FILE_ATTRIBUTE_DIRECTORY
							  |FILE_ATTRIBUTE_HIDDEN
						  )))
				{
					_tcscpy(pathEnd, findData.cFileName);
					ShellExecute(NULL, NULL, szPath, NULL, NULL, SW_SHOWNORMAL);
				}
			}
			while (FindNextFile(hSearch, &findData));
			FindClose(hSearch);
		}
		IMalloc *pMalloc;
		if (item && SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free(item);
			pMalloc->Release();
		}
	}
}

}
