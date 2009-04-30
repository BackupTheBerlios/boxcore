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

	UINT GetTraySize();
	bool GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], ATOM p_info[], UINT p_count);
	NotificationIcon *LookupIcon(HWND p_hWnd, UINT p_uID);
	NotificationIcon *LookupIcon(UINT p_index);

	NotifyIconHandler *m_imp;
protected:
	virtual bool _Start();
	virtual bool _Stop();
	virtual bool _SetProperty(ATOM p_property, PVOID p_value);
	virtual bool _Exec(ATOM p_command);
private:
	//NotifyIconHandler *m_imp;

	PVOID m_iconFactory;
	bool m_useProxy;

	ATOM m_iconFactoryProp;
	ATOM m_useProxyProp;
	ATOM m_callbackAddProp;
	ATOM m_callbackModProp;
	ATOM m_callbackDelProp;
	ATOM m_cleanTrayCommand;
};

}

#endif /* CLSNOTIFYICONSRV_H_ */
