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

#define SERVICE_NAME "SRV_ShellTrayWnd"

namespace ShellServices
{

ShellTrayWndSrv::ShellTrayWndSrv():
		Service(SERVICE_NAME),
		m_imp(NULL),
		m_hInstance(NULL),
		m_topMost(false)
{
	m_hInstanceProp = RegisterAtom("STW_hInstance");
	m_topMostProp = RegisterAtom("STW_topMost");
	m_handlerProp = RegisterAtom("STW_handler");
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
	else if (p_property == m_handlerProp)
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

void ShellTrayWndSrv::SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge)
{
	if (m_imp)
	{
		m_imp->SetTaskbarPos(pLeft, pTop, pRight, pBottom, pEdge);
	}
	AppbarSrv *appbarService;
	s_serviceManager->CastService("SRV_Appbar", appbarService);
	if (appbarService)
	{
		appbarService->SetTaskbarPos(pLeft, pTop, pRight, pBottom, pEdge);
	}
}

REGISTER_SERVICE(ShellTrayWndSrv)

}
