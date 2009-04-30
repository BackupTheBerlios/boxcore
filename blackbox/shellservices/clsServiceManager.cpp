/*
 * clsServiceManager.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsServiceManager.h"
#include "clsService.h"

#include "clsShellTrayWndSrv.h"
#include "clsAppbarSrv.h"
#include "clsNotifyIconRectSrv.h"
#include "clsNotifyIconSrv.h"
#include "clsStartupSrv.h"

#include <functional>
#include <algorithm>

namespace ShellServices
{

ServiceManager::ServiceManager()
{
	Service::s_serviceManager = this;
	m_serviceList.push_back(new ShellTrayWndSrv());
	m_serviceList.push_back(new AppbarSrv());
	m_serviceList.push_back(new NotifyIconRectSrv());
	m_serviceList.push_back(new NotifyIconSrv());
	m_serviceList.push_back(new StartupSrv());
}

ServiceManager::~ServiceManager()
{
	for (t_serviceList::iterator i = m_serviceList.begin(); i != m_serviceList.end(); ++i)
	{
		delete *i;
	}
}

bool ServiceManager::StartService(LPCSTR p_serviceID)
{
	ATOM serviceID = FindAtomA(p_serviceID);
	return m_serviceList.end() != std::find_if(m_serviceList.begin(), m_serviceList.end(), std::bind2nd(std::mem_fun(&Service::Start), serviceID));
}

bool ServiceManager::StartServiceThreaded(LPCSTR p_serviceID)
{
	Service *service = GetService(p_serviceID);
	if (service)
	{
		CloseHandle(CreateThread(NULL, 0, ServiceThread, service, 0, NULL));
		return true;
	}
	else
	{
		return false;
	}
}

bool ServiceManager::StopService(LPCSTR p_serviceID)
{
	ATOM serviceID = FindAtomA(p_serviceID);
	return m_serviceList.end() != std::find_if(m_serviceList.begin(), m_serviceList.end(), std::bind2nd(std::mem_fun(&Service::Stop), serviceID));
}

Service *ServiceManager::GetService(LPCSTR p_serviceID)
{
	ATOM serviceID = FindAtomA(p_serviceID);
	t_serviceList::iterator service = std::find_if(m_serviceList.begin(), m_serviceList.end(), std::bind2nd(std::mem_fun(&Service::IsService), serviceID));
	if (service != m_serviceList.end())
	{
		return *service;
	}
	else
	{
		return NULL;
	}
}

bool ServiceManager::RemoveService(LPCSTR p_serviceID)
{
	Service *service = GetService(p_serviceID);
	if (service)
	{
		m_serviceList.remove(service);
		delete service;
		return true;
	}
	else
	{
		return false;
	}
}

bool ServiceManager::ExecServiceCommand(LPCSTR p_serviceID, LPCSTR p_command)
{
	ATOM serviceID = FindAtomA(p_serviceID);
	ATOM command = FindAtomA(p_command);
	for (t_serviceList::iterator i = m_serviceList.begin(); i != m_serviceList.end(); ++i)
	{
		if ((*i)->Exec(serviceID, command))
			return true;
	}
	return false;
}

bool ServiceManager::SetServiceProperty(LPCSTR p_serviceID, LPCSTR p_property, PVOID p_value)
{
	ATOM serviceID = FindAtomA(p_serviceID);
	ATOM property = FindAtomA(p_property);
	for (t_serviceList::iterator i = m_serviceList.begin(); i != m_serviceList.end(); ++i)
	{
		if ((*i)->SetProperty(serviceID, property, p_value))
			return true;
	}
	return false;
}

DWORD WINAPI ServiceManager::ServiceThread(LPVOID lpData)
{
	Service *service = reinterpret_cast<Service *>(lpData);
	return service->Start(service->m_serviceID);
}

}
