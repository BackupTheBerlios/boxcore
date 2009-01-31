/** @internal
  * @file
  * @brief This file contains the implementation of RCConditional
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

#include "clsRCConditional.h"

namespace RCWorkers
{

/** @brief Constructor for a conditional worker
  *
  * @param p_worker A pointer to another worker, which must be dynamically allocated.
  * This class takes responsibility or later deletion
  * @param p_condition A reference to the variable which should be checked
  * @param p_conditionTest The value to check the reference variable for
  */
RCConditional::RCConditional(RCWorker *p_worker, bool &p_condition, bool p_conditionTest):RCWorker(NULL, NULL), m_condition(p_condition), m_conditionTest(p_conditionTest)
{
	m_worker = p_worker;
	m_valid = false;
}

/** @brief Destructor
  *
  * Deletes the worker that was passed in
  */
RCConditional::~RCConditional()
{
	delete m_worker;
}

/** @brief Performs a conditional read using the given worker
  *
  * If the reference variable is equal to the required condition, the RCWorker::ReadValue()
  * method is called on the wrapped worker and the contained data is set as valid. Otherwise the
  * contained data is set to be invalid.
  */
void RCConditional::ReadValue()
{
	if (m_condition == m_conditionTest)
	{
		m_worker->ReadValue();
		m_valid = true;
	}
	else
	{
		m_valid = false;
	}
}

/** @brief Performs a conditional write using the given worker
  *
  * If the reference variable is equal to the required condition and the data is marked as valid
  * the RCWorker::WriteValue() method is called on the wrapped worker.
  */
void RCConditional::WriteValue()
{
	if (m_valid && m_condition == m_conditionTest)
	{
		m_worker->WriteValue();
	}
}

}
