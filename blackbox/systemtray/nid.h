/*
 * nid.h
 *
 *  Created on: 28 Dec 2008
 *      Author: shaun
 */

#ifndef NID_H_
#define NID_H_

namespace ShellServices
{

struct NID_ANSI
{
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	CHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	CHAR szInfo[256];
	union
	{
		UINT uTimeout;
		UINT uVersion;
	};
	CHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
	UINT hBalloonIcon;
};

struct NID_UNICODE
{
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union
	{
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
	UINT hBalloonIcon;
};

struct NID_INTERNAL
{
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union
	{
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
	HICON hBalloonIcon;
};

#ifndef offsetof
#define offsetof(type, member) __builtin_offsetof(type, member)
#endif

#define NID_WIN95_SIZE offsetof(NID_ANSI, szTip[64])
#define NID_2KA_SIZE offsetof(NID_ANSI, guidItem)
#define NID_XPA_SIZE offsetof(NID_ANSI, hBalloonIcon)
#define NID_VISTAA_SIZE sizeof(NID_ANSI)

#define NID_NT_SIZE offsetof(NID_UNICODE, szTip[64])
#define NID_2KW_SIZE offsetof(NID_UNICODE, guidItem)
#define NID_XPW_SIZE offsetof(NID_UNICODE, hBalloonIcon)
#define NID_VISTAW_SIZE sizeof(NID_UNICODE)

#ifndef NIF_INFO
#define NIF_INFO 0x00000010
#endif
#ifndef NIF_GUID
#define NIF_GUID 0x00000020
#endif
#ifndef NIF_REALTIME
#define NIF_REALTIME 0x00000040
#endif
#ifndef NIF_SHOWTIP
#define NIF_SHOWTIP 0x00000080
#endif

#ifndef NIIF_USER
#define NIIF_USER 0x00000004
#endif
#ifndef NIIF_LARGE_ICON
#define NIIF_LARGE_ICON 0x00000020
#endif

}

#endif /* NID_H_ */
