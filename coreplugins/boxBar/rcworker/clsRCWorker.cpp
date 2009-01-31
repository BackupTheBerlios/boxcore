/*
 * clsRCWorker.cpp
 *
 *  Created on: 18 Jan 2009
 *      Author: shaun
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
