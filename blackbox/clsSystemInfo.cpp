#include "clsSystemInfo.h"

clsSystemInfo::clsSystemInfo()
{
	ZeroMemory(&osInfo, sizeof(osInfo));
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	GetVersionEx(&osInfo);
	typeNT = osInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
	osVista = typeNT && osInfo.dwMajorVersion >= 6;
	osWin7 = osVista && osInfo.dwMinorVersion == 1;
	os2kXP    = typeNT && osInfo.dwMajorVersion == 5;

	BOOL bIs64BitOS= FALSE;
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"),"IsWow64Process");
	if (NULL != fnIsWow64Process)
		fnIsWow64Process(GetCurrentProcess(), &bIs64BitOS);
	arch64 = bIs64BitOS;

	prog64 = (sizeof(int)!=sizeof(void *));
	if (prog64) arch64=true;

}

clsSystemInfo::~clsSystemInfo()
{
	//dtor
}
