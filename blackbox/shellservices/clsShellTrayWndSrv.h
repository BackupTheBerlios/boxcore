/*
 * clsShellTrayWndSrv.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSSHELLTRAYWNDSRV_H_
#define CLSSHELLTRAYWNDSRV_H_

#include "clsService.h"
#include "clsShellServiceWindow.h"

namespace ShellServices
{

class ShellTrayWndSrv: public ShellServices::Service
{
public:
	ShellTrayWndSrv();
	virtual ~ShellTrayWndSrv();

	virtual bool Call(ATOM p_command, const ServiceArg &p_arg1, const ServiceArg &p_arg2);
	void SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge);
protected:
	virtual bool _Start();
	virtual bool _Stop();
	virtual bool _SetProperty(ATOM p_property, PVOID p_value);
private:
	ShellServiceWindow *m_imp;
	HINSTANCE m_hInstance;
	bool m_topMost;

	ATOM m_hInstanceProp;
	ATOM m_topMostProp;
	ATOM m_handlerProp;

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSSHELLTRAYWNDSRV_H_ */
