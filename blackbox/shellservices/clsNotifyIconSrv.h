/*
 * clsNotifyIconSrv.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSNOTIFYICONSRV_H_
#define CLSNOTIFYICONSRV_H_

#include "clsService.h"

#include <windows.h>

namespace ShellServices
{

class NotifyIconHandler;
class NotificationIcon;

//TODO Implement CleanTray service command and figure something out to get rid of the public m_imp
class NotifyIconSrv: public ShellServices::Service
{
public:
	NotifyIconSrv();
	virtual ~NotifyIconSrv();

	virtual bool Call(ATOM p_function);
	virtual bool Call(ATOM p_function, const ServiceArg &p_arg1);
	virtual bool Call(ATOM p_function, const ServiceArg &p_arg1, const ServiceArg &p_arg2);
	virtual bool Call(ATOM p_function, const ServiceArg &p_arg1,
					  const ServiceArg &p_arg2, const ServiceArg &p_arg3);
	virtual bool Call(ATOM p_function, const ServiceArg &p_arg1,
					  const ServiceArg &p_arg2, const ServiceArg &p_arg3,
					  const ServiceArg &p_arg4);
protected:
	virtual bool _Start();
	virtual bool _Stop();
	virtual bool _SetProperty(ATOM p_property, PVOID p_value);
private:
	NotifyIconHandler *m_imp;

	bool GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], ATOM p_info[], UINT p_count);

	PVOID m_iconFactory;
	bool m_useProxy;

	ATOM m_iconFactoryProp;
	ATOM m_useProxyProp;
	ATOM m_callbackAddProp;
	ATOM m_callbackModProp;
	ATOM m_callbackDelProp;
	ATOM m_cleanTrayCommand;
	ATOM m_getNotificationIconInfoFn;
	ATOM m_lookupIconFn;
	ATOM m_GetTraySizeFn;

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSNOTIFYICONSRV_H_ */
