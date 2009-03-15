/*
 * clsCriticalSection.cpp
 *
 *  Created on: 17 Feb 2009
 *      Author: shaun
 */

#include "clsCriticalSection.h"

CriticalSection::CriticalSection(CRITICAL_SECTION &p_critSection):m_critSection(p_critSection)
{
	EnterCriticalSection(&m_critSection);
}

CriticalSection::~CriticalSection()
{
	LeaveCriticalSection(&m_critSection);
}
