/*
 * clsRCBool.h
 *
 *  Created on: 19 Jan 2009
 *      Author: shaun
 */

#ifndef CLSRCBOOL_H_
#define CLSRCBOOL_H_

#include "clsRCWorker.h"

namespace RCWorkers
{

class RCBool: public RCWorkers::RCWorker
{
public:
	RCBool(LPCSTR p_configFile, LPCSTR p_RCKey, bool &p_variable, bool p_default);
	virtual ~RCBool();

	virtual void ReadValue();
	virtual void WriteValue();
private:
	bool &m_variable;
	bool m_default;
};

}

#endif /* CLSRCBOOL_H_ */
