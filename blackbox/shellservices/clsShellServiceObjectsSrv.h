/*
 * clsShellServiceObjectsSrv.h
 *
 *  Created on: 04 May 2009
 *      Author: shaun
 */

#ifndef CLSSHELLSERVICEOBJECTSSRV_H_
#define CLSSHELLSERVICEOBJECTSSRV_H_

#include "clsService.h"
#include "clsShellServiceHandler.h"
#include <shlobj.h>
#include <shellapi.h>
#ifdef __GNUC__
#include <shlwapi.h>
#endif
#include <docobj.h>

#include <list>
#include <utility>

namespace ShellServices
{

typedef std::list<std::pair<GUID, IOleCommandTarget *> > SSOlist_t;

class ShellServiceObjectsSrv: public ShellServices::Service,
			public ShellServices::ShellServiceHandler
{
public:
	ShellServiceObjectsSrv();
	virtual ~ShellServiceObjectsSrv();

	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);
private:
	static CLSID s_CLSID_ShellTrayObj;

	void StartRegistryValues(LPCTSTR p_regkey);

	void StartGUID(CLSID p_id);
	void StopGUID(CLSID p_id);

	virtual bool _Start();
	virtual bool _Stop();

	SSOlist_t m_SSOlist;

	static ServiceRegistrar s_serviceRegistration;
};

}

#endif /* CLSSHELLSERVICEOBJECTSSRV_H_ */
