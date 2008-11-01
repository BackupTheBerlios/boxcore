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
