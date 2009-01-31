/*
 * clsRCInt.h
 *
 *  Created on: 18 Jan 2009
 *      Author: shaun
 */

#ifndef CLSRCINT_H_
#define CLSRCINT_H_

#include "clsRCWorker.h"

namespace RCWorkers
{

/** @brief This class provides a connector between an integer and an RC entry
  *
  * This class allows the linking of both signed and unsigned integers to RC entries
  * by having two constructors.
  */
class RCInt: public RCWorker
{
public:
	RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, INT &p_variable, INT p_default);
	RCInt(LPCSTR p_configFile, LPCSTR p_RCKey, UINT &p_variable, UINT p_default);

	virtual void ReadValue();
	virtual void WriteValue();
private:
	/** @brief Indicates whether a signed or unsigned instance was created */
	bool m_signed;

	/** @brief The reference to the signed integer which is linked, if applicable */
	INT &m_variable;

	/** @brief The reference to the unsigned integer which is linked, if applicable */
	UINT &m_unsignedVariable;

	/** @brief The signed default value */
	INT m_default;

	/** @brief The unsigned default value */
	UINT m_unsignedDefault;

};

}

#endif /* CLSRCINT_H_ */
