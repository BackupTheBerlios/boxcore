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
#include <typeinfo>

#define SERVICE_NAME "SRV_Appbar"

namespace ShellServices
{

AppbarSrv::AppbarSrv():
		Service(SERVICE_NAME),
		m_imp(NULL),
		m_SetTaskbarPosFn(RegisterAtom("FN_SetTaskbarPos"))
{
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
		s_serviceManager->Call("SRV_ShellTrayWnd", "STW_handler", Arg<UINT>(0), Arg<ShellServiceHandler *>(m_imp));
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

bool AppbarSrv::Call(ATOM p_function, const ServiceArg &p_arg1,
					  const ServiceArg &p_arg2)
{
	if (p_function == m_SetTaskbarPosFn)
	{
		typedef Arg<RECT &> type1;
		typedef Arg<UINT> type2;
		if (CHECK_ARG(1) && CHECK_ARG(2))
		{
			RECT &arg1 = MAKE_ARG(1);
			m_imp->SetTaskbarPos(arg1.left, arg1.top, arg1.right, arg1.bottom, MAKE_ARG(2));
			s_serviceManager->Call("SRV_ShellTrayWnd", "FN_SetTaskbarPos", p_arg1, p_arg2);
			return true;
		}
		PRINT("Argument check failed in " SERVICE_NAME " for 2 args");
	}
	PRINT("2 Arg call not found in " SERVICE_NAME);
	return false;
}

REGISTER_SERVICE(AppbarSrv)

}
