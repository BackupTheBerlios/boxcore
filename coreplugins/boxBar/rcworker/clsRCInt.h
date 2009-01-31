/*
 * clsRCInt.h
 *
 *  Created on: 18 Jan 2009
 *      Author: shaun
 */

#ifndef CLSRCINT_H_
#define CLSRCINT_H_

#include "clsRCWorker.h"

namespace RCWorkers
{

class RCInt: public RCWorkers::RCWorker
{
public:
	RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, INT &p_variable, INT p_default);
	RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, UINT &p_variable, UINT p_default);
	virtual ~RCInt();

	virtual void ReadValue();
	virtual void WriteValue();
private:
	bool m_signed;
	INT &m_variable;
	UINT &m_unsignedVariable;
	INT m_default;
	UINT m_unsignedDefault;

};

}

#endif /* CLSRCINT_H_ */
