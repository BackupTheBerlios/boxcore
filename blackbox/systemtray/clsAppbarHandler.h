/*
 * clsAppbarHandler.h
 *
 *  Created on: 05 Jan 2009
 *      Author: shaun
 */

#ifndef CLSAPPBARHANDLER_H_
#define CLSAPPBARHANDLER_H_

#include "clsShellServiceHandler.h"

namespace ShellServices
{

class AppbarHandler: public ShellServices::ShellServiceHandler
{
public:
	AppbarHandler();
	virtual ~AppbarHandler();

	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);
	void SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge);
private:
	int barLeft, barRight, barTop, barBottom;
	UINT barEdge;
};

}

#endif /* CLSAPPBARHANDLER_H_ */
