/*
 * clsService.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSSERVICE_H_
#define CLSSERVICE_H_

#include <windows.h>

#include "../../debug/debug.h"
#include "../clsSystemInfo.h"

namespace ShellServices
{

class ServiceManager;

class Service
{
public:
	Service(LPCSTR p_serviceName);
	virtual ~Service();

	bool Start(ATOM p_serviceID);
	bool Stop(ATOM p_serviceID);
	int Running(ATOM p_serviceID);
	bool IsService(ATOM p_serviceID);
	bool Exec(ATOM p_serviceID, ATOM p_command);

	bool SetProperty(ATOM p_serviceID, ATOM p_property, PVOID p_value);
protected:
	virtual bool _Start() = 0;
	virtual bool _Stop() = 0;
	virtual bool _Exec(ATOM p_command);
	virtual bool _SetProperty(ATOM p_property, PVOID p_value);

	bool m_running;
	static ServiceManager *s_serviceManager;
	static clsSystemInfo s_systemInfo;
private:
	ATOM m_serviceID;

	friend class ServiceManager;
};



}

#endif /* CLSSERVICE_H_ */
