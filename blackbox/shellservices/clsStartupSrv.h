/*
 * clsStartupSrv.h
 *
 *  Created on: 20 Apr 2009
 *      Author: shaun
 */

#ifndef CLSSTARTUPSRV_H_
#define CLSSTARTUPSRV_H_

#include "clsService.h"

namespace ShellServices
{

class StartupSrv: public ShellServices::Service
{
public:
	StartupSrv();
	virtual ~StartupSrv();
protected:
	bool _Start();
	bool _Stop();

	HKEY _CreateSessionInfoKey();
	bool IsFirstRunThisSession(LPCTSTR pszSubkey);
private:
	IMalloc *m_shellMalloc;

	void RunShellFolder(UINT p_csidl);

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSSTARTUPSRV_H_ */
