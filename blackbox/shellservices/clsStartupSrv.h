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
	bool m_vistaOrGreater;
	bool m_isNT;

	void RunShellFolder(UINT p_csidl);
};

}

#endif /* CLSSTARTUPSRV_H_ */
