#ifndef CLSSHLWAPI_H
#define CLSSHLWAPI_H

#include <windows.h>

typedef LPVOID (WINAPI *fnSHLockShared)(HANDLE hData, DWORD dwOtherProcId);
typedef BOOL (WINAPI *fnSHUnlockShared)(void *pvData);


class clsShlwapi
{
public:
	clsShlwapi();
	virtual ~clsShlwapi();

	fnSHLockShared SHLockShared;
	fnSHUnlockShared SHUnlockShared;
protected:
	HMODULE hModule;

private:
};

extern clsShlwapi shlwapi;

#endif // CLSSHLWAPI_H
