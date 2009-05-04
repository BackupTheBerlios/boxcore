/*
 * clsServiceRegistrar.cpp
 *
 *  Created on: 04 May 2009
 *      Author: shaun
 */

#include "clsServiceRegistrar.h"

namespace ShellServices
{

ServiceRegistrar::ServiceRegistrar(LPCSTR p_serviceName, fnServiceCreator p_creator)
{
	m_serviceAtom = AddAtomA(p_serviceName);
	ServiceManager::KnownService(m_serviceAtom,  p_creator);

}

ServiceRegistrar::~ServiceRegistrar()
{
	DeleteAtom(m_serviceAtom);
}

}
