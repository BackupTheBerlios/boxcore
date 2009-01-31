/*
 * clsRCWorker.h
 *
 *  Created on: 18 Jan 2009
 *      Author: shaun
 */

#ifndef CLSRCWORKER_H_
#define CLSRCWORKER_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace RCWorkers
{

class RCWorker
{
public:
	virtual ~RCWorker();

	virtual void ReadValue() = 0;
	virtual void WriteValue() = 0;
protected:
	RCWorker(LPCSTR p_configFile, LPCSTR p_RCKey);

	LPCSTR m_configFile;
	LPSTR m_RCKey;
};

}

#endif /* CLSRCWORKER_H_ */
