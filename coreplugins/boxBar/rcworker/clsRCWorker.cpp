/*
 * clsRCWorker.cpp
 *
 *  Created on: 18 Jan 2009
 *      Author: shaun
 */

#include "clsRCWorker.h"

namespace RCWorkers
{

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

RCWorker::~RCWorker()
{
	delete[] m_RCKey;
}

}
