#include "clsShellServiceObjects.h"

/**
  * @brief Make sure that shell service objects where stopped
  */
clsShellServiceObjects::~clsShellServiceObjects()
{
	stopServiceObjects();
}

/**
  * @brief Starts Shell Service Objects from a list
  *
  * Iterates through the list of CLSID's which is provided by the supplied
  * clsClsidSource. Createa an instance of the OleCommandTarget interface
  * for each object, and then sends the object a start command via this interface.
  * The interfaces are then stored to allow the objects to be stopped again later.
  */
int clsShellServiceObjects::startServiceObjects(clsClsidSource &pSource)
{
	int numStarted=0;
	CLSID objectClsid;
	while(1)
	{
		objectClsid=pSource.getNextCLSID();
		if (objectClsid!=CLSID_NULL)
		{
			IOleCommandTarget *pOCT;
			HRESULT hr = CoCreateInstance(objectClsid, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IOleCommandTarget, (void **) &pOCT);
			if (SUCCEEDED(hr))
			{
				pOCT->Exec(&CGID_ShellServiceObject, OLECMDID_NEW, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
				serviceObjects.push_back(pOCT);
				numStarted++;
			}
			//Sleep(1000);
		}
		else
			break;
	}
	return numStarted;
}

/**
  * @brief Stops any running Shell Service Objects
  *
  * Iterates through the list of stored interfaces to Shell Service Objects.
  * Stops eachone through the stored interface and tehn releases the interface.
  * Empties the list of interfaces on completion, so starting services again
  * is acceptable.
  */
int clsShellServiceObjects::stopServiceObjects()
{
	int numStopped=0;
	for(unsigned int i=0;i<serviceObjects.size();i++)
	{
		serviceObjects[i]->Exec(&CGID_ShellServiceObject, 3, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
		serviceObjects[i]->Release();
	}
	serviceObjects.clear();
	return numStopped;
}
