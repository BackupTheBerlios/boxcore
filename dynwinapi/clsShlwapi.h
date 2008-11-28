#ifndef CLSSHLWAPI_H
#define CLSSHLWAPI_H

#include <windows.h>

typedef LPVOID (*fnSHLockShared)(HANDLE hData, DWORD dwOtherProcId);
typedef BOOL (*fnSHUnlockShared)(void *pvData);


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
