/*
 * clsAppbarSrv.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsAppbarSrv.h"
#include "clsServiceManager.h"
#include "clsAppbarHandler.h"

#include "../../debug/debug.h"

#include <utility>

namespace ShellServices
{

AppbarSrv::AppbarSrv():
		Service("SRV_Appbar"),
		m_imp(NULL)
{
	// TODO Auto-generated constructor stub

}

AppbarSrv::~AppbarSrv()
{
	// TODO Auto-generated destructor stub
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

}
