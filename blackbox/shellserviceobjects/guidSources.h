#ifndef GUIDSOURCES_H
#define GUIDSOURCES_H

#include <string>
#include <windows.h>

#include <shlobj.h>
#include <shellapi.h>
#ifdef __GNUC__
#include <shlwapi.h>
#endif
#include <docobj.h>

using std::wstring;


/**
 * @class clsidSource
 *
 * @brief Base class for CLSID sources
 *
 * Abstract base class with a single virtual method for retrieving CLSID's
 */
class clsidSource
{
public:
/**
 * Dummy virtual destructor, in case child classes need it.
 */
	virtual ~clsidSource() {}
/**
 * Virtual method for implementation by child classes.
 * Should return CLSID_NULL once all CLSID's have been returned,
 * the source of CLSID's is dependant on the child implementation
 */
	virtual CLSID getNextCLSID()=0;
protected:
private:
};

/**
 * @class clsidRegValues
 *
 * @brief Reads CLSID's from a registry key
 *
 * This class reads CLSID's stored as registry values under a given key. The CLSID's
 * are read from the data of each value (the value name is not used).
 * This is the style used in the Windows 2000/XP ShellServiceObjectsDelayLoad key
 */
class clsidRegValues : public clsidSource
{
public:
/**
 * Attempts to open the specified registry key with read permissions. If succesful
 * a handle to the key is stored, as well as the key name. The flag keyValid is
 * set to true on suceesfully opening a key.
 * @param[in] pRegKeyName The registry key to open under HKEY_LOCAL_MACHINE
 */
	clsidRegValues(const wstring &pRegKeyName);
/**
 * Closes the handle to the registry key, if it was open.
 */
	~clsidRegValues();
/**
 * Returns the CLSID's in the order they appear in the registry.
 * Returns CLSID_NULL on first calll if the registry key was not succesfully opened.
 * Returns CLSID_NULL on calls after each registry value has been read once.
 */
	virtual CLSID getNextCLSID();
private:
	HKEY regKey;
	wstring keyName;
	bool keyValid;
	int index;
};

/**
 * @class clsidRegKeys
 *
 * @brief Reads CLSID's from a registry key
 *
 * This class reads CLSID's stored as registry keys under a given key. The CLSID's
 * are read from the names of the subkeys.
 * This is the style used in the Windows Vista ShellServiceObjects key
 *
 * @note Currently not used, due to double icons when combined with the injected key
 */
class clsidRegKeys : public clsidSource
{
	public:
		clsidRegKeys(const wstring &);
		~clsidRegKeys();
		virtual CLSID getNextCLSID();
	private:
		HKEY regKey;
		wstring keyName;
		bool keyValid;
		int index;
};


/**
 * @class clsidInjected
 *
 * @brief Produces an arbitrary CLSID
 *
 * Creates a CLSID form a string and then returns it once.
 * Used when an object needs to be started, but is not present in the registry (Windows Vista)
 */
class clsidInjected : public clsidSource
{
	public:
		clsidInjected(const wstring &);
		virtual CLSID getNextCLSID();
	private:
		CLSID inject;
		bool used;
};

#endif
