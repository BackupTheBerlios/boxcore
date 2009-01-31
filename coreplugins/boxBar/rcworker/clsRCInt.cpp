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

/** @brief Constructor for the signed version
  *
  * @param[in] p_configFile RC file path. Passed directly to RCWorker constructor
  * @param[in] p_RCKey Key in the RC file to read. Passed directly to RCWorker constructor
  * @param[in,out] p_variable A reference to the variable you want linked to the RC entry
  * @param[in] p_default The default value to use if not found
  */
RCInt::RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, INT &p_variable, INT p_default):RCWorker(p_configFile, p_RCKey), m_variable(p_variable), m_unsignedVariable(m_unsignedDefault)
{
	m_default = p_default;
	m_signed = true;
}

/** @brief Constructor for the unsigned version
  *
  * @param[in] p_configFile RC file path. Passed directly to RCWorker constructor
  * @param[in] p_RCKey Key in the RC file to read. Passed directly to RCWorker constructor
  * @param[in,out] p_variable A reference to the variable you want linked to the RC entry
  * @param[in] p_default The default value to use if not found
  */
RCInt::RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, UINT &p_variable, UINT p_default):RCWorker(p_configFile, p_RCKey), m_variable(m_default), m_unsignedVariable(p_variable)
{
	m_unsignedDefault = p_default;
	m_signed = false;
}

/** @brief Uses the ReadInt() function to get the value of the entry in the RC file */
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

/** @brief Uses the WriteInt() function to write the variable value into the RC file */
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
