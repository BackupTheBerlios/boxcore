#ifndef CLSAPILOADER_H
#define CLSAPILOADER_H
#include <windows.h>

class clsApiLoader
{
public:
	clsApiLoader();
	virtual ~clsApiLoader();

	void freeLibrary();
	bool requestApiPresence(CONST TCHAR *pIdentifier);
	FARPROC requestApiPointer(CONST CHAR *pApiName);
protected:
private:
	HMODULE hBlackbox;
};

#endif // CLSAPILOADER_H
