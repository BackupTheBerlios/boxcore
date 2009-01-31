/*
 * clsRCInt.cpp
 *
 *  Created on: 18 Jan 2009
 *      Author: shaun
 */

#include "clsRCInt.h"
#include "BBApi.h"

namespace RCWorkers
{

RCInt::RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, INT &p_variable, INT p_default):RCWorker(p_configFile, p_RCKey), m_variable(p_variable), m_unsignedVariable(m_unsignedDefault)
{
	m_default = p_default;
	m_signed = true;
}

RCInt::RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, UINT &p_variable, UINT p_default):RCWorker(p_configFile, p_RCKey), m_variable(m_default), m_unsignedVariable(p_variable)
{
	m_unsignedDefault = p_default;
	m_signed = false;
}

RCInt::~RCInt()
{
}

void RCInt::ReadValue()
{
	if (m_signed)
	{
	m_variable = ReadInt(m_configFile, m_RCKey, m_default);
	}
	else
	{
		m_unsignedVariable = ReadInt(m_configFile, m_RCKey, m_unsignedDefault);
	}
}

void RCInt::WriteValue()
{
	if (m_signed)
	{
	WriteInt(m_configFile, m_RCKey, m_variable);
	}
	else
	{
		WriteInt(m_configFile, m_RCKey, m_unsignedVariable);
	}
}

}
