/** @internal
  * @file
  * @brief This file contains the definition of RCInt
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

#ifndef CLSRCINT_H_
#define CLSRCINT_H_

#include "clsRCWorker.h"

namespace RCWorkers
{

/** @internal
  * @brief This class provides a connector between an integer and an RC entry
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
