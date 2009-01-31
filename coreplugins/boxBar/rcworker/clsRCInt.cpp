/** @internal
  * @file
  * @brief This file contains the implementation of RCInt
  *
  * This file is part of the boxBar source code @n
  * <!-- Copyright (C) 2008-2009 Carsomyr -->
  * Copyright &copy; 2008-2009 Carsomyr
  * @par Links:
  * http://developer.berlios.de/projects/boxcore @n
  * @par License:
  * boxBar is free software, released under the GNU General
  * Public License (GPL version 2 or later). For details see:
  * @par
  * http://www.fsf.org/licenses/gpl.html @n
  * @par
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
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
