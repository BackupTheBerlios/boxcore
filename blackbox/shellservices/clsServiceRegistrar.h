/*
 * clsServiceRegistrar.h
 *
 *  Created on: 04 May 2009
 *      Author: shaun
 */

#ifndef CLSSERVICEREGISTRAR_H_
#define CLSSERVICEREGISTRAR_H_

#include "clsServiceManager.h"

namespace ShellServices
{

class ServiceRegistrar
{
public:
	ServiceRegistrar(LPCSTR p_serviceName, fnServiceCreator p_creator);
	virtual ~ServiceRegistrar();
private:
	ATOM m_serviceAtom;
};

#define REGISTER_SERVICE(className) \
	Service *Make##className() {return new className();} \
	ServiceRegistrar className::s_serviceRegistration(SERVICE_NAME, Make##className);

}

#endif /* CLSSERVICEREGISTRAR_H_ */
