/*
 * clsAppbarSrv.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsAppbarSrv.h"
#include "clsServiceManager.h"
#include "clsServiceRegistrar.h"
#include "clsAppbarHandler.h"

#include "../../debug/debug.h"

#include <utility>

#define SERVICE_NAME "SRV_Appbar"

namespace ShellServices
{

AppbarSrv::AppbarSrv():
		Service(SERVICE_NAME),
		m_imp(NULL)
{
	// TODO Auto-generated constructor stub

}

AppbarSrv::~AppbarSrv()
{
	// TODO Auto-generated destructor stub
}

void AppbarSrv::SetTaskbarPos(int p_left, int p_top, int p_right, int p_bottom, UINT p_edge)
{
	if (m_imp)
	{
		m_imp->SetTaskbarPos(p_left, p_top, p_right, p_bottom, p_edge);
	}
}

bool AppbarSrv::_Start()
{
	if (!m_imp)
	{
		PRINT("Starting Appbar Service");
		m_imp = new AppbarHandler();
		std::pair<UINT, PVOID> handler(0, m_imp);
		s_serviceManager->SetServiceProperty("SRV_ShellTrayWnd", "STW_handler", &handler);
		return true;
	}
	else
	{
		return false;
	}
}

bool AppbarSrv::_Stop()
{
	if (m_imp)
	{
		PRINT("Stopping Appbar Service");
		delete m_imp;
		m_imp = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

REGISTER_SERVICE(AppbarSrv)

}
