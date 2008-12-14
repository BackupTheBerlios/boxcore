/** @internal
  * @file clsClsidSources.h
  * @brief This file contains the definitions of clsClsidSource, clsClsidRegValues,
  * clsClsidRegKeys and clsClsidInjected
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2008 Carsomyr
  * @par Links:
  * http://developer.berlios.de/projects/boxcore @n
  * @par License:
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

#ifndef CLSCLSIDSOURCES_H
#define CLSCLSIDSOURCES_H

#include <string>
#include <vector>

#include <shlobj.h>
#include <shellapi.h>
#ifdef __GNUC__
#include <shlwapi.h>
#endif
#include <docobj.h>

using std::wstring;
using std::vector;


/** @internal
  * @class clsClsidSource
  *
  * @brief Base class for CLSID sources
  *
  * Abstract base class with a single virtual method for retrieving CLSID's.
  * Also implements a whitelist to limit which CLSID's are returned.
  * The whitelist is deactivated unless it has at least one entry.
  */
class clsClsidSource
{
public:
	/** @brief Dummy virtual destructor, in case child classes need it.
	  */
	virtual ~clsClsidSource() {}

	/** @brief Fetch the CLSID in the list
	  *
	  * Virtual method for implementation by child classes.
	  * Should return CLSID_NULL once all CLSID's have been returned,
	  * the source of CLSID's is dependant on the child implementation
	  */
	virtual CLSID getNextCLSID() = 0;

	void setWhitelist(vector<wstring> &pWhitelist);
protected:
	bool inWhitelist(CLSID pClsid);
private:
	///Stores the CLSID's of a whitelist. Whitelist is disabled when this is empty
	vector<CLSID> whiteList;
};

/** @internal
  * @class clsClsidRegValues
  *
  * @brief Reads CLSID's from a registry key
  *
  * This class reads CLSID's stored as registry values under a given key. The CLSID's
  * are read from the data of each value (the value name is not used).
  * This is the style used in the Windows 2000/XP ShellServiceObjectsDelayLoad key
  */
class clsClsidRegValues : public clsClsidSource
{
public:
	clsClsidRegValues(const wstring &pRegKeyName);
	~clsClsidRegValues();
	virtual CLSID getNextCLSID();
private:
	///Stores the opened registry key
	HKEY regKey;
	///The name of the key that was opened
	wstring keyName;
	///If the key was succesfully opened or not
	bool keyValid;
	///Index of the next CLSID to return
	int index;
};

/** @internal
  * @class clsClsidRegKeys
  *
  * @brief Reads CLSID's from a registry key
  *
  * This class reads CLSID's stored as registry keys under a given key. The CLSID's
  * are read from the names of the subkeys.
  * This is the style used in the Windows Vista ShellServiceObjects key
  *
  * @note Currently not used, due to double icons when combined with the injected key
  */
class clsClsidRegKeys : public clsClsidSource
{
public:
	clsClsidRegKeys(const wstring &);
	~clsClsidRegKeys();
	virtual CLSID getNextCLSID();
private:
	///Stores the opened registry key
	HKEY regKey;
	///The name of the key that was opened
	wstring keyName;
	///If the key was succesfully opened or not
	bool keyValid;
	///Index of the next CLSID to return
	int index;
};


/** @internal
  * @class clsClsidInjected
  *
  * @brief Produces an arbitrary CLSID
  *
  * Creates a CLSID form a string and then returns it once.
  * Used when an object needs to be started, but is not present in the registry (Windows Vista)
  */
class clsClsidInjected : public clsClsidSource
{
public:
	clsClsidInjected(const wstring &);
	virtual CLSID getNextCLSID();
private:
	/// The CLSID to output
	CLSID inject;
	/// True once the CLSID has been used
	bool used;
};

#endif
