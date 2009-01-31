/** @internal
  * @file
  * @brief This file contains the base constructor and destructor for RCWorker
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

#include "clsRCWorker.h"

namespace RCWorkers
{

/** @brief Sets up variables for the filename and RC key ot be used
  * by derived classes.
  *
  * @param[in] p_configFile The RC file name. The string passed in here should be preserved
  * for the lifetime of the worker
  * @param[in] p_RCKey The key to read in the RC file. A copy is made, as this is likely to be
  * constructed dynamically
  */
RCWorker::RCWorker(LPCSTR p_configFile, LPCSTR p_RCKey)
{
	m_configFile = p_configFile;
	if (p_RCKey)
	{
		m_RCKey = new CHAR[strlen(p_RCKey)+1];
		strcpy(m_RCKey, p_RCKey);
	}
	else
	{
		m_RCKey = NULL;
	}
}

/** @brief Base destructor for RCWorkers
  *
  * Cleans up the allocated string used for the RC key
  */
RCWorker::~RCWorker()
{
	delete[] m_RCKey;
}

}
