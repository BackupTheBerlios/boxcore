/*
 * clsAppbarSrv.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSAPPBARSRV_H_
#define CLSAPPBARSRV_H_

#include "clsService.h"
#include "clsShellServiceHandler.h"

namespace ShellServices
{

class AppbarHandler;

class AppbarSrv: public ShellServices::Service, ShellServiceHandler
{
public:
	AppbarSrv();
	virtual ~AppbarSrv();

	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);

	virtual bool Call(ATOM p_function, const ServiceArg &p_arg1,
						  const ServiceArg &p_arg2);
protected:
	virtual bool _Start();
	virtual bool _Stop();
private:
	RECT m_taskbarRect;
	UINT m_taskbarEdge;

	ATOM m_SetTaskbarPosFn;

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSAPPBARSRV_H_ */
