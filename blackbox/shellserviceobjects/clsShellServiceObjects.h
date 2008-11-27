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

/**
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
