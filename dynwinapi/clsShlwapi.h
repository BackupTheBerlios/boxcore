#ifndef CLSSHLWAPI_H
#define CLSSHLWAPI_H

#include <windows.h>

typedef WINAPI LPVOID (*fnSHLockShared)(HANDLE hData, DWORD dwOtherProcId);
typedef WINAPI BOOL (*fnSHUnlockShared)(void *pvData);


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

#endif // CLSSHLWAPI_H
