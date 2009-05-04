/*
 * clsService.cpp
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#include "clsService.h"

namespace ShellServices
{

Service::Service(LPCSTR p_serviceName):
		m_running(false),
		m_atomList(),
		m_serviceID(RegisterAtom(p_serviceName))

{
}

Service::~Service()
{
	for (std::list<ATOM>::iterator i = m_atomList.begin(); i != m_atomList.end(); ++i)
	{
		DeleteAtom(*i);
	}
}

bool Service::Start(ATOM p_serviceID)
{
	if (m_serviceID == p_serviceID)
	{
		return _Start();
	}
	else
	{
		return false;
	}
}

bool Service::Stop(ATOM p_serviceID)
{
	if (m_serviceID == p_serviceID)
	{
		return _Stop();
	}
	else
	{
		return false;
	}
}

bool Service::Exec(ATOM p_serviceID, ATOM p_command)
{
	if (m_serviceID == p_serviceID)
	{
		return _Exec(p_command);
	}
	else
	{
		return false;
	}
}

int Service::Running(ATOM p_serviceID)
{
	if (m_serviceID == p_serviceID)
	{
		return m_running;
	}
	else
	{
		return -1;
	}
}

bool Service::IsService(ATOM p_serviceID)
{
	return m_serviceID == p_serviceID;
}

bool Service::SetProperty(ATOM p_serviceID, ATOM p_property, PVOID p_value)
{
	if (m_serviceID == p_serviceID)
	{
		return _SetProperty(p_property, p_value);
	}
	else
	{
		return false;
	}
}

bool Service::_Exec(ATOM p_command)
{
	return false;
}

bool Service::_SetProperty(ATOM p_property, PVOID p_value)
{
	return false;
}

ATOM Service::RegisterAtom(LPCSTR p_identifier)
{
	ATOM temp = AddAtomA(p_identifier);
	m_atomList.push_back(temp);
	return temp;
}

ServiceManager *Service::s_serviceManager = NULL;
clsSystemInfo Service::s_systemInfo;

}
