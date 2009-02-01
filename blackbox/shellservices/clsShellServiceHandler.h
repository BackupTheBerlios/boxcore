/** @internal
  * @file
  * @brief This file contains the definition the ShellServiceHandler class.
  *
  * ShellServiceHandler is part of the Shell Services implementation used by boxCore.
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008-2009 Carsomyr -->
  * Copyright &copy; 2008-2009 Carsomyr
  * @par links
  * http://developer.berlios.de/projects/boxcore @n
  * @par License
  * boxCore is free software, released under the GNU General
  * Public License (GPL version 2 or later), with an extension that allows
  * linking of proprietary modules under a controlled interface. This means
  * that plugins etc. are allowed to be released under any license the author
  * wishes. For details see:
  * @par
  * http://www.fsf.org/licenses/gpl.html @n
  * http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface
  * @par
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * The basic operations of this class is based on the functions in Tray.cpp in
  * the bbLean source code.
  */
#include <windows.h>

#ifndef CLSSHELLSERVICEHANDLER_H
#define CLSSHELLSERVICEHANDLER_H

/** @internal
  * @brief Contains classes and functions which provide some sort of service to the shell
  */
namespace ShellServices
{

/** @internal
  * @brief This class represents an interface for use with ShellServiceWindow
  *
  * Classes implementing this interface will be able to register themselves with an instance
  * of ShellServiceWindow to allow them to process WM_COPYDATA messages sent to Shell_TrayWnd.
  * Only one type of message can be handled by a derived class, as the dwData member is not passed on
  */
class ShellServiceHandler
{
public:
	/** @brief Empty virtual destructor */
	virtual ~ShellServiceHandler() {}

	/**@brief Function called by ShellServiceWindow for a WM_COPYDATA message
	  *
	  * This function must be implemented by derived classes and is the sole purpose of this class
	  */
	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData) = 0;
};

}

#endif // CLSSHELLSERVICEHANDLER_H
