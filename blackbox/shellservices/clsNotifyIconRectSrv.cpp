/*
 * clsNotifyIconRectSrv.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsNotifyIconRectSrv.h"

#include "clsServiceManager.h"
#include "clsServiceRegistrar.h"

#include "../../debug/debug.h"

#define SERVICE_NAME "SRV_NotifyIconRect"

namespace ShellServices
{

struct NOTIFYICONIDENTIFIER_V1
{
	DWORD cbSize;
	DWORD dwPadding;
	DWORD hWnd; //HWND hWnd; breaks under 64 bit compilers
	UINT uID;
	GUID guidItem;
};

struct NOTIFYICONIDENTIFIERMSG_V1
{
	DWORD dwMagic;
	DWORD dwMessage;
	NOTIFYICONIDENTIFIER_V1 nii;
};

NotifyIconRectSrv::NotifyIconRectSrv():
		Service(SERVICE_NAME)
{
}

NotifyIconRectSrv::~NotifyIconRectSrv()
{
	// TODO Auto-generated destructor stub
}

bool NotifyIconRectSrv::_Start()
{
	if (!m_running)
	{
		PRINT("Starting NotifyIconRect Service");
		std::pair<UINT, PVOID> handler(3, this);
		s_serviceManager->SetServiceProperty("SRV_ShellTrayWnd", "STW_handler", &handler);
		m_running = true;
	}
	return m_running;
}

bool NotifyIconRectSrv::_Stop()
{
	if (m_running)
	{
		PRINT("Stopping NotifyIconRect Service");
		std::pair<UINT, PVOID> handler(3, NULL);
		s_serviceManager->SetServiceProperty("SRV_ShellTrayWnd", "STW_handler", &handler);
		return true;
	}
	else
	{
		return false;
	}
}

HRESULT NotifyIconRectSrv::ProcessMessage(DWORD p_cbData, PVOID p_lpData)
{
	switch (p_cbData)
	{
	case sizeof(NOTIFYICONIDENTIFIERMSG_V1):
	{
		NOTIFYICONIDENTIFIERMSG_V1 &data = *reinterpret_cast<NOTIFYICONIDENTIFIERMSG_V1 *>(p_lpData);
		if (data.dwMagic != 0x34753423)
			return 0;
		switch (data.dwMessage)
		{
		case 1:
		{
			//Return current cursor position for now, this should be top left corner of icon.
			POINT mousePos;
			GetCursorPos(&mousePos);
			return MAKELONG(mousePos.x, mousePos.y);
		}
		case 2:
			//Return 16x16 as icon size for now
			return 0x00100010;
		default:
			return 0;
		}
	}
	default:
		TRACE("Unexpected NotifyIconGetRect message size: %u", p_cbData);
		return 0;
	}
}

REGISTER_SERVICE(NotifyIconRectSrv)

}
