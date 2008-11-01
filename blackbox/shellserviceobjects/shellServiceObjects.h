#ifndef SHELLSERVICEOBJECTS_H
#define SHELLSERVICEOBJECTS_H

#include "guidSources.h"
#include <vector>

using std::vector;

#include <shlobj.h>
#include <shellapi.h>
#ifdef __GNUC__
#include <shlwapi.h>
#endif
#include <docobj.h>

/**
 * @class shellServiceObjects
 *
 * @brief Start and stops ShellServiceObjects
 *
 * Starts ShellServiceObjects based on CLSID. CLSID's are retrieved from any
 * number of objects of clsidSource. All ShellServiceObjects are stopped on destruction
 * , but the entire list can also be stopped on demand. There is currently no support for
 * stopping individual ShellServiceObjects.
 */
class shellServiceObjects
{
public:
	shellServiceObjects();
	~shellServiceObjects();
	int startServiceObjects(clsidSource &);
	int stopServiceObjects();
protected:
private:
	vector<IOleCommandTarget *> serviceObjects;
};

#endif
