/*
 * clsNotifyIconRectSrv.h
 *
 *  Created on: 19 Apr 2009
 *      Author: shaun
 */

#ifndef CLSNOTIFYICONRECTSRV_H_
#define CLSNOTIFYICONRECTSRV_H_

#include "clsService.h"
#include "clsShellServiceHandler.h"

namespace ShellServices
{

class NotifyIconRectSrv: public Service, public ShellServiceHandler
{
public:
	NotifyIconRectSrv();
	virtual ~NotifyIconRectSrv();
private:
	virtual bool _Start();
	virtual bool _Stop();

	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSNOTIFYICONRECTSRV_H_ */
