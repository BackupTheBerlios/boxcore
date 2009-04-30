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
protected:
	virtual bool _Start();
	virtual bool _Stop();
private:
	AppbarHandler *m_imp;
};

}

#endif /* CLSAPPBARSRV_H_ */
