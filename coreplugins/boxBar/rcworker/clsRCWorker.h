/** @internal
  * @file
  * @brief This file contains the interface definitions for RCWorkers
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

#ifndef CLSRCWORKER_H_
#define CLSRCWORKER_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/** @brief Contains RCWorker and its concrete types */
namespace RCWorkers
{

/** @brief This class presents a basic interface for classes which link an RC file entry with a
  * program variable
  *
  * This class defines the interfaces through which a specific entry in an RC file can be read or written
  * It also provides a basic constructor for keeping track of the file and key, as this is common to all
  * derived classes.
  */
class RCWorker
{
public:
	virtual ~RCWorker();

	/** @brief Called to read in a value from the RC key into the variable */
	virtual void ReadValue() = 0;
	/** @brief Called to write the value of the variable into the RC key */
	virtual void WriteValue() = 0;
protected:
	RCWorker(LPCSTR p_configFile, LPCSTR p_RCKey);

	/** @brief Contains the name of the RC file to work on */
	LPCSTR m_configFile;
	/** @brief Contains the key to read in the RC file */
	LPSTR m_RCKey;
};

}

#endif /* CLSRCWORKER_H_ */
