/** @internal
  * @file
  * @brief This file contains the definition of RCBool
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

#ifndef CLSRCBOOL_H_
#define CLSRCBOOL_H_

#include "clsRCWorker.h"

namespace RCWorkers
{

/** @internal
  * @brief This class provides a connector between a boolean and an RC entry
  */
class RCBool: public RCWorker
{
public:
	RCBool(LPCSTR p_configFile, LPCSTR p_RCKey, bool &p_variable, bool p_default);

	virtual void ReadValue();
	virtual void WriteValue();
private:
	/** @brief A reference to the boolean varaible that is linked */
	bool &m_variable;

	/** @brief The default value to return if not found */
	bool m_default;
};

}

#endif /* CLSRCBOOL_H_ */
