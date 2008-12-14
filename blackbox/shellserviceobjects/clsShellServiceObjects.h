/** @internal
  * @file clsShellServiceObjects.h
  * @brief This file contains the definition of clsShellServiceObjects
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2008 Carsomyr
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
  */

#ifndef CLSSHELLSERVICEOBJECTS_H
#define CLSSHELLSERVICEOBJECTS_H

#include "clsClsidSources.h"
#include <vector>

using std::vector;

#include <shlobj.h>
#include <shellapi.h>
#ifdef __GNUC__
#include <shlwapi.h>
#endif
#include <docobj.h>

/** @internal
  * @class clsShellServiceObjects
  *
  * @brief Start and stops ShellServiceObjects
  *
  * Starts ShellServiceObjects based on CLSID. CLSID's are retrieved from any
  * number of objects of clsidSource. All ShellServiceObjects are stopped on destruction
  * , but the entire list can also be stopped on demand. There is currently no support for
  * stopping individual ShellServiceObjects.
  */
class clsShellServiceObjects
{
public:
	~clsShellServiceObjects();
	int startServiceObjects(clsClsidSource &);
	int stopServiceObjects();
protected:
private:
	vector<IOleCommandTarget *> serviceObjects;
};

#endif
