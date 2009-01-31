/** @internal
  * @file
  * @brief This file contains the definition of RCConditional
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

#ifndef CLSRCCONDITIONAL_H_
#define CLSRCCONDITIONAL_H_

#include "clsRCInt.h"

namespace RCWorkers
{

/** @internal
  * @brief This class wraps another RCWorker, to make its read and write actions conditional
  */
class RCConditional: public RCWorker
{
public:
	RCConditional(RCWorker *p_worker, bool &p_condition, bool p_conditionTest);
	virtual ~RCConditional();

	virtual void ReadValue();
	virtual void WriteValue();
private:
	/** @brief The worker that is being wrapped */
	RCWorker *m_worker;

	/** @brief A reference to the variable to check */
	bool &m_condition;

	/** @brief Indicates whether to check for true or false */
	bool m_conditionTest;

	/** @brief Indicates if the wrapped worker has valid data for a write */
	bool m_valid;
};

}

#endif /* CLSRCCONDITIONALINT_H_ */
