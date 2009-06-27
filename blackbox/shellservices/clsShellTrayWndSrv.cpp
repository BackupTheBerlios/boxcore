/*
 * clsShellTrayWndSrv.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsShellTrayWndSrv.h"
#include "clsAppbarSrv.h"
#include "clsServiceManager.h"
#include "clsServiceRegistrar.h"

#include <typeinfo>

#define SERVICE_NAME "SRV_ShellTrayWnd"

namespace ShellServices
{

ShellTrayWndSrv::ShellTrayWndSrv():
		Service(SERVICE_NAME),
		m_imp(NULL),
		m_hInstance(NULL),
		m_topMost(false),
		m_fnSetTaskbarPos(RegisterAtom("FN_SetTaskbarPos"))
{
	m_hInstanceProp = RegisterAtom("PROP_hInstance");
	m_topMostProp = RegisterAtom("PROP_topMost");
	m_fnSetHandler = RegisterAtom("FN_SetHandler");
}

ShellTrayWndSrv::~ShellTrayWndSrv()
{
}

bool ShellTrayWndSrv::_SetProperty(ATOM p_property, PVOID p_value)
{
	if (p_property == m_hInstanceProp)
	{
		m_hInstance = reinterpret_cast<HINSTANCE>(p_value);
		return true;
	}
	else if (p_property == m_topMostProp)
	{
		m_topMost = p_value;
		return true;
	}
	else if (p_property == m_fnSetHandler)
	{
		if (m_imp)
		{
			std::pair<UINT, ShellServiceHandler *> &handler = *reinterpret_cast<std::pair<UINT, ShellServiceHandler *> *>(p_value);
			m_imp->RegisterHandler(handler.first, handler.second);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ShellTrayWndSrv::_Start()
{
	if (!m_imp)
	{
		m_imp = new ShellServiceWindow(m_hInstance, m_topMost);
		return true;
	}
	else
	{
		return false;
	}
}

bool ShellTrayWndSrv::_Stop()
{
	if (m_imp)
	{
		delete m_imp;
		m_imp = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool ShellTrayWndSrv::Call(ATOM p_function, const ServiceArg &p_arg1, const ServiceArg &p_arg2)
{
	if (p_function == m_fnSetHandler)
	{
		typedef Arg<UINT> type1;
		typedef Arg<ShellServiceHandler *> type2;
		if (CHECK_ARG(1) && CHECK_ARG(2))
		{
			m_imp->RegisterHandler(MAKE_ARG(1), MAKE_ARG(2));
			return true;
		}
		PRINT("Argument check failed in " SERVICE_NAME " for 2 args");
	}
	else if (p_function == m_fnSetTaskbarPos)
	{
		typedef Arg<RECT &> type1;
		typedef Arg<UINT> type2;
		if (CHECK_ARG(1) && CHECK_ARG(2))
		{
			RECT &arg1 = MAKE_ARG(1);
			m_imp->SetTaskbarPos(arg1.left, arg1.top, arg1.right, arg1.bottom, MAKE_ARG(2));
			return true;
		}
		PRINT("Argument check failed in " SERVICE_NAME " for 2 args");
	}
	else if (p_function == m_SetPropertyFn)
	{
		typedef Arg<ATOM> type1;
		if (CHECK_ARG(1))
		{
			ATOM prop = MAKE_ARG(1);
			if (prop == m_hInstanceProp)
			{
				typedef Arg<HINSTANCE> type2;
				if (CHECK_ARG(2))
				{
					m_hInstance = MAKE_ARG(2);
					return true;
				}
			}
			else if (prop == m_topMostProp)
			{
				typedef Arg<bool> type2;
				if (CHECK_ARG(2))
				{
					m_topMost = MAKE_ARG(2);
					return true;
				}
			}
		}
	}
	PRINT("2 Arg call not found in " SERVICE_NAME);
	return false;
}

REGISTER_SERVICE(ShellTrayWndSrv)

}
