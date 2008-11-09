#include "clsShellServiceObjects.h"

clsShellServiceObjects::clsShellServiceObjects()
{
}

clsShellServiceObjects::~clsShellServiceObjects()
{
	stopServiceObjects();
}

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
