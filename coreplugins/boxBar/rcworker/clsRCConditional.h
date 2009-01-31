/*
 * clsRCConditional.h
 *
 *  Created on: 19 Jan 2009
 *      Author: shaun
 */

#ifndef CLSRCCONDITIONAL_H_
#define CLSRCCONDITIONAL_H_

#include "clsRCInt.h"

namespace RCWorkers
{

class RCConditional: public RCWorker
{
public:
	RCConditional(RCWorker *p_worker, bool &p_condition, bool p_conditionTest);
	virtual ~RCConditional();

	virtual void ReadValue();
	virtual void WriteValue();
private:
	RCWorker *m_worker;
	bool &m_condition;
	bool m_conditionTest;
	bool m_valid;
};

}

#endif /* CLSRCCONDITIONALINT_H_ */
