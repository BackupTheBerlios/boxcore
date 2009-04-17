/*
 * clsNotifyIconRect.h
 *
 *  Created on: 11 Apr 2009
 *      Author: shaun
 */

#ifndef CLSNOTIFYICONRECT_H_
#define CLSNOTIFYICONRECT_H_

#include "clsShellServiceHandler.h"
#include <windows.h>

namespace ShellServices
{

class NotifyIconRectHandler: public ShellServices::ShellServiceHandler
{
public:
	NotifyIconRectHandler();
	virtual ~NotifyIconRectHandler();

	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);
};

}

#endif /* CLSNOTIFYICONRECT_H_ */
