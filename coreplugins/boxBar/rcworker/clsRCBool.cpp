/*
 * clsRCBool.cpp
 *
 *  Created on: 19 Jan 2009
 *      Author: shaun
 */

#include "clsRCBool.h"
#include "BBApi.h"

namespace RCWorkers
{

RCBool::RCBool(LPCSTR p_configFile, LPCSTR p_RCKey, bool &p_variable, bool p_default):RCWorker(p_configFile, p_RCKey), m_variable(p_variable), m_default(p_default)
{
	// TODO Auto-generated constructor stub

}

RCBool::~RCBool()
{
	// TODO Auto-generated destructor stub
}

void RCBool::ReadValue()
{
	m_variable = ReadBool(m_configFile, m_RCKey, m_default);
}

void RCBool::WriteValue()
{
	WriteBool(m_configFile, m_RCKey, m_variable);
}

}
