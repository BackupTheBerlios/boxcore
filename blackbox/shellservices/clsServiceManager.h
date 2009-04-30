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

namespace ShellServices
{

class Service;

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

	bool RemoveService(LPCSTR p_serviceID);

	bool ExecServiceCommand(LPCSTR p_serviceID, LPCSTR p_command);

	bool SetServiceProperty(LPCSTR p_serviceID, LPCSTR p_property, PVOID p_value);

	template<typename T>
	void CastService(LPCSTR p_serviceID, T&p_destination)
	{
		Service *service = GetService(p_serviceID);
		p_destination = static_cast<T>(service);
	}
private:
	t_serviceList m_serviceList;

	static DWORD WINAPI ServiceThread(LPVOID lpData);

};



}

#endif /* CLSSERVICEMANAGER_H_ */
