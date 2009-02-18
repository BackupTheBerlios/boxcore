/*
 * clsCriticalSection.h
 *
 *  Created on: 17 Feb 2009
 *      Author: shaun
 */

#ifndef CLSCRITICALSECTION_H_
#define CLSCRITICALSECTION_H_

#include <windows.h>

class CriticalSection
{
public:
	CriticalSection(CRITICAL_SECTION &p_critSection);
	virtual ~CriticalSection();
private:
	CRITICAL_SECTION &m_critSection;
};

#endif /* CLSCRITICALSECTION_H_ */
