/*
 * clsServiceManager.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSSERVICEMANAGER_H_
#define CLSSERVICEMANAGER_H_

#include <windows.h>
#include <list>

#include "ServiceArgs.h"

namespace ShellServices
{

class Service;
typedef Service *(*fnServiceCreator)();

typedef std::list<Service *> t_serviceList;

class ServiceManager
{
public:
	ServiceManager();
	virtual ~ServiceManager();

	bool StartService(LPCSTR p_serviceID);
	bool StartServiceThreaded(LPCSTR p_serviceID);

	bool StopService(LPCSTR p_serviceID);
	bool ServiceRunning(LPCSTR p_serviceID);
	Service *GetService(LPCSTR p_serviceID);

	void AddService(Service *p_service);
	bool RemoveService(LPCSTR p_serviceID);

	bool SetServiceProperty(LPCSTR p_serviceID, LPCSTR p_property, const ServiceArg &p_value);

	bool Call(LPCSTR p_serviceID, LPCSTR p_function, const ServiceArg &p_arg1 = ServiceArg(),
			  const ServiceArg &p_arg2 = ServiceArg(), const ServiceArg &p_arg3 = ServiceArg(),
			  const ServiceArg &p_arg4 = ServiceArg());
private:
	t_serviceList m_serviceList;

	static Service *KnownService(ATOM p_id, fnServiceCreator p_creator = NULL);
	static DWORD WINAPI ServiceThread(LPVOID lpData);

	friend class ServiceRegistrar;
};



}

#endif /* CLSSERVICEMANAGER_H_ */
