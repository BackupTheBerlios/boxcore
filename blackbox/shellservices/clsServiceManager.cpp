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
#include <typeinfo>

namespace ShellServices
{

ServiceManager::ServiceManager():
		m_serviceList()
{
	Service::s_serviceManager = this;
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
	Service *service = GetService(p_serviceID);
	if (service)
	{
		TRACE("Starting %s", p_serviceID);
		return service->Start(serviceID);
	}
	else
	{
		return false;
	}
}

bool ServiceManager::StartServiceThreaded(LPCSTR p_serviceID)
{
	Service *service = GetService(p_serviceID);
	if (service)
	{
		TRACE("Starting %s threaded", p_serviceID);
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
	TRACE("Stopping %s", p_serviceID);
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
		Service *newService = KnownService(serviceID);
		if (newService)
		{
			AddService(newService);
			return newService;
		}
		else
		{
			TRACE("WARNING: Could not create service %s", p_serviceID);
			return NULL;
		}
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

bool ServiceManager::SetServiceProperty(LPCSTR p_serviceID, LPCSTR p_property, const ServiceArg &p_value)
{
	Service *service = GetService(p_serviceID);
	if (service)
	{
		//ATOM serviceID = FindAtomA(p_serviceID);
		ATOM propertyFn = FindAtom(TEXT("FN_SetProperty"));
		ATOM property = FindAtomA(p_property);
		return service->Call(propertyFn, Arg<ATOM>(property), p_value);
		//return service->SetProperty(serviceID, property, p_value);
	}
	else
	{
		return false;
	}
}

DWORD WINAPI ServiceManager::ServiceThread(LPVOID lpData)
{
	Service *service = reinterpret_cast<Service *>(lpData);
	return service->Start(service->m_serviceID);
}

void ServiceManager::AddService(Service *p_service)
{
	m_serviceList.push_back(p_service);
}

bool ServiceManager::Call(LPCSTR p_serviceID, LPCSTR p_function, const ServiceArg &p_arg1, const ServiceArg &p_arg2, const ServiceArg &p_arg3, const ServiceArg &p_arg4)
{
	Service *service = GetService(p_serviceID);
	if (service)
	{
		ATOM function = FindAtomA(p_function);
		if (function)
		{
			if (typeid(p_arg4) != typeid(ServiceArg))
			{
				return service->Call(function, p_arg1, p_arg2, p_arg3, p_arg4);
			}
			if (typeid(p_arg3) != typeid(ServiceArg))
			{
				return service->Call(function, p_arg1, p_arg2, p_arg3);
			}
			if (typeid(p_arg2) != typeid(ServiceArg))
			{
				return service->Call(function, p_arg1, p_arg2);
			}
			if (typeid(p_arg1) != typeid(ServiceArg))
			{
				return service->Call(function, p_arg1);
			}
			return service->Call(function);
		}
	}
	return false;
}

Service *ServiceManager::KnownService(ATOM p_id, fnServiceCreator p_creator)
{
	static std::map<ATOM, fnServiceCreator> knownServices;
	if (p_creator)
	{
		knownServices[p_id] = p_creator;
		return NULL;
	}
	else
	{
		if (knownServices[p_id])
		{
			return knownServices[p_id]();
		}
		else
		{
			return NULL;
		}
	}
}

}
