/*
 * clsAppbarSrv.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsAppbarSrv.h"
#include "clsServiceManager.h"
#include "clsServiceRegistrar.h"

#include "../../debug/debug.h"
#include "../../dynwinapi/clsShlwapi.h"

#include <utility>
#include <typeinfo>

#define SERVICE_NAME "SRV_Appbar"

namespace ShellServices
{

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

struct APPBARMSG_V7
{
	APPBARDATA_64 abd;

	DWORD dwMessage;
	DWORD dwPadding1;
	///Really a handle, but needed for correct spacing on 32 bit hosts
	UINT64 hSharedMemory;
	DWORD dwSourceProcessID;
	DWORD dwPadding3;
};

AppbarSrv::AppbarSrv():
		Service(SERVICE_NAME),
		m_SetTaskbarPosFn(RegisterAtom("FN_SetTaskbarPos"))
{
}

AppbarSrv::~AppbarSrv()
{
	// TODO Auto-generated destructor stub
}

bool AppbarSrv::_Start()
{
	if (!m_running)
	{
		PRINT("Starting Appbar Service");
		s_serviceManager->Call("SRV_ShellTrayWnd", "STW_handler", Arg<UINT>(0), Arg<ShellServiceHandler *>(this));
		m_running = true;
	}
	return m_running;
}

bool AppbarSrv::_Stop()
{
	if (m_running)
	{
		PRINT("Stopping Appbar Service");
		s_serviceManager->Call("SRV_ShellTrayWnd", "STW_handler", Arg<UINT>(0), Arg<ShellServiceHandler *>(NULL));
		m_running = false;
	}
	return !m_running;
}

bool AppbarSrv::Call(ATOM p_function, const ServiceArg &p_arg1,
					  const ServiceArg &p_arg2)
{
	if (p_function == m_SetTaskbarPosFn)
	{
		typedef Arg<RECT &> type1;
		typedef Arg<UINT> type2;
		if (CHECK_ARG(1) && CHECK_ARG(2))
		{
			m_taskbarRect = MAKE_ARG(1);
			m_taskbarEdge = MAKE_ARG(2);
			s_serviceManager->Call("SRV_ShellTrayWnd", "FN_SetTaskbarPos", p_arg1, p_arg2);
			return true;
		}
		PRINT("Argument check failed in " SERVICE_NAME " for 2 args");
	}
	PRINT("2 Arg call not found in " SERVICE_NAME);
	return false;
}

HRESULT AppbarSrv::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
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
	case sizeof(APPBARMSG_V7):
		appbarMsg.dwMessage = ((APPBARMSG_V7 *)(p_lpData))->dwMessage;
		appbarMsg.hSharedMemory = (HANDLE)((APPBARMSG_V7 *)(p_lpData))->hSharedMemory;
		appbarMsg.dwSourceProcessID = ((APPBARMSG_V7 *)(p_lpData))->dwSourceProcessID;
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
		appbarMsg.abd->rc = m_taskbarRect;
		appbarMsg.abd->uEdge = m_taskbarEdge;
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

REGISTER_SERVICE(AppbarSrv)

}
