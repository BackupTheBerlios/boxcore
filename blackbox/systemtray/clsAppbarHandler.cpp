/*
 * clsAppbarHandler.cpp
 *
 *  Created on: 05 Jan 2009
 *      Author: shaun
 */

#include "clsAppbarHandler.h"
#include "../../dynwinapi/clsShlwapi.h"
#include "../../debug/debug.h"


#ifndef ABM_SETSTATE
#define ABM_SETSTATE 0x0000000A
#endif

//Received from XP onwards (lParam member is 64 bits)
struct APPBARDATA_64
{
	DWORD cbSize;
	///Really an HWND, but needed for spacing on 64 bit systems
	DWORD hWnd;
	UINT uCallbackMessage;
	UINT uEdge;
	RECT rc;
	LONG64 lParam;
};

//Received from 2k back, lParam member is 32 bits
struct APPBARDATA_32
{
	DWORD cbSize;
	///Really an HWND, but needed for spacing on 64 bit systems
	DWORD hWnd;
	UINT uCallbackMessage;
	UINT uEdge;
	RECT rc;
	LONG32 lParam;
};

/** @internal
  * Used in XP, Vista as well
  */
struct APPBARMSG_64
{
	APPBARDATA_64 abd;

	DWORD dwMessage;
	///Really a handle, but needed for correct spacing on 64 bit hosts
	DWORD hSharedMemory;
	DWORD dwSourceProcessID;
};


/** @internal
  * Used in 2k, and 98se
  */
struct APPBARMSG_32
{
	APPBARDATA_32 abd;

	DWORD dwMessage;
	///Really a handle, but needed for correct spacing on 64 bit hosts
	DWORD hSharedMemory;
	DWORD dwSourceProcessID;
};

/** @internal
  * We use a pointer to the 32 bit version of APPBARDATA, because we only need  the low order bits, and it keeps us safe
  */
struct INTERNAL_APPBARMSG
{
	APPBARDATA_32 *abd;

	DWORD dwMessage;
	HANDLE hSharedMemory;
	DWORD dwSourceProcessID;
};

namespace ShellServices
{

AppbarHandler::AppbarHandler()
{
	// TODO Auto-generated constructor stub

}

AppbarHandler::~AppbarHandler()
{
	// TODO Auto-generated destructor stub
}

HRESULT AppbarHandler::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
{
	INTERNAL_APPBARMSG appbarMsg;
	//Always cast to 32 bit version for this assignment
	appbarMsg.abd = &(((APPBARMSG_32 *)(p_lpData))->abd);
	switch (p_cbData)
	{
	case sizeof(APPBARMSG_64):
		appbarMsg.dwMessage = ((APPBARMSG_64 *)(p_lpData))->dwMessage;
		appbarMsg.hSharedMemory = (HANDLE)((APPBARMSG_64 *)(p_lpData))->hSharedMemory;
		appbarMsg.dwSourceProcessID = ((APPBARMSG_64 *)(p_lpData))->dwSourceProcessID;
		break;
	case sizeof(APPBARMSG_32):
		appbarMsg.dwMessage = ((APPBARMSG_32 *)(p_lpData))->dwMessage;
		appbarMsg.hSharedMemory = (HANDLE)((APPBARMSG_32 *)(p_lpData))->hSharedMemory;
		appbarMsg.dwSourceProcessID = ((APPBARMSG_32 *)(p_lpData))->dwSourceProcessID;
		break;
	default:
		return FALSE;
	}
	switch (appbarMsg.dwMessage)
	{
	case ABM_NEW:
		return 1;
	case ABM_GETTASKBARPOS:
		if (shlwapi.SHLockShared)
			appbarMsg.abd = (APPBARDATA_32 *)shlwapi.SHLockShared(appbarMsg.hSharedMemory, appbarMsg.dwSourceProcessID);
		SetRect(&appbarMsg.abd->rc, barLeft, barTop, barRight, barBottom);
		appbarMsg.abd->uEdge = barEdge;
		if (shlwapi.SHUnlockShared)
			shlwapi.SHUnlockShared(appbarMsg.abd);
		return 1;
	case ABM_REMOVE:
	case ABM_QUERYPOS:
	case ABM_GETSTATE:
	case ABM_ACTIVATE:
	case ABM_GETAUTOHIDEBAR:
	case ABM_SETAUTOHIDEBAR:
	case ABM_WINDOWPOSCHANGED:
	case ABM_SETSTATE:
	default:
		TRACE("Appbar debug :The message is %ld or %ld", ((APPBARMSG_32*)(p_lpData))->dwMessage, ((APPBARMSG_64*)(p_lpData))->dwMessage);
		return FALSE;
	}
}

void AppbarHandler::SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge)
{
	barLeft = pLeft;
	barRight = pRight;
	barTop = pTop;
	barBottom = pBottom;
	barEdge = pEdge;
}

}
