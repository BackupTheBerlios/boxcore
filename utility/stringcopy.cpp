/*
 * stringcopy.cpp
 *
 *  Created on: 12 Jan 2009
 *      Author: shaun
 */

#include "stringcopy.h"
#include <cstdio>

bool CopyString(LPSTR p_dest, LPCSTR p_src, UINT p_max)
{
	strncpy(p_dest, p_src, p_max);
	p_dest[p_max-1] = '\0';
	return true;
}

bool CopyString(LPSTR p_dest, LPCWSTR p_src, UINT p_max)
{
	if (WideCharToMultiByte(CP_ACP, 0 , p_src, -1, p_dest, p_max, NULL, NULL))
	{
		p_dest[p_max-1] = '\0';
		return true;
	}
	else
	{
		return false;
	}
}

bool CopyString(LPWSTR p_dest, LPCSTR p_src, UINT p_max)
{
	if (MultiByteToWideChar(CP_ACP, 0, p_src, -1, p_dest, p_max))
	{
		p_dest[p_max-1]= L'\0';
		return true;
	}
	else
	{
		return false;
	}
}

bool CopyString(LPWSTR p_dest, LPCWSTR p_src, UINT p_max)
{
	wcsncpy(p_dest, p_src, p_max);
	p_dest[p_max-1] = L'\0';
	return true;
}
