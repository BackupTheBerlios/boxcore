/*
 * clsRCConditionalInt.cpp
 *
 *  Created on: 19 Jan 2009
 *      Author: shaun
 */

#include "clsRCConditional.h"

namespace RCWorkers
{

RCConditional::RCConditional(RCWorker *p_worker, bool &p_condition, bool p_conditionTest):RCWorker(NULL, NULL), m_condition(p_condition), m_conditionTest(p_conditionTest)
{
	m_worker = p_worker;
	m_valid = false;
}

RCConditional::~RCConditional()
{
	delete m_worker;
}

void RCConditional::ReadValue()
{
	if (m_condition == m_conditionTest)
	{
		m_worker->ReadValue();
		m_valid = true;
	}
	else
	{
		m_valid = false;
	}
}

void RCConditional::WriteValue()
{
	if (m_valid && m_condition == m_conditionTest)
	{
		m_worker->WriteValue();
	}
}



}
