/*
 * clsAppbarSrv.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSAPPBARSRV_H_
#define CLSAPPBARSRV_H_

#include "clsService.h"

namespace ShellServices
{

class AppbarHandler;

class AppbarSrv: public ShellServices::Service
{
public:
	AppbarSrv();
	virtual ~AppbarSrv();

	void SetTaskbarPos(int p_left, int p_top, int p_right, int p_bottom, UINT p_edge);
protected:
	virtual bool _Start();
	virtual bool _Stop();
private:
	AppbarHandler *m_imp;

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSAPPBARSRV_H_ */
