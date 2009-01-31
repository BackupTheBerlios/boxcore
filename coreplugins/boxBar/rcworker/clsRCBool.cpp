/** @internal
  * @file
  * @brief This file contains the implementation of RCBool
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

#include "clsRCBool.h"
#include "BBApi.h"

namespace RCWorkers
{

/** @brief Constructor
  *
  * @param[in] p_configFile RC file path. Passed directly to RCWorker constructor
  * @param[in] p_RCKey Key in the RC file to read. Passed directly to RCWorker constructor
  * @param[in,out] p_variable A reference to the variable you want linked to the RC entry
  * @param[in] p_default The default value to use if not found
  */
RCBool::RCBool(LPCSTR p_configFile, LPCSTR p_RCKey, bool &p_variable, bool p_default):RCWorker(p_configFile, p_RCKey), m_variable(p_variable), m_default(p_default)
{
}

/** @brief Uses the ReadBool() function to get the value of the entry in the RC file */
void RCBool::ReadValue()
{
	m_variable = ReadBool(m_configFile, m_RCKey, m_default);
}

/** @brief Uses the WriteBool() function to write the variable value into the RC file */
void RCBool::WriteValue()
{
	WriteBool(m_configFile, m_RCKey, m_variable);
}

}
