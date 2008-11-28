#ifndef CLSUSER32_H
#define CLSUSER32_H

#include <windows.h>

typedef BOOL (*fnAllowSetForegroundWindow)(DWORD dwProcessId);


class clsUser32
{
	public:
		clsUser32();
		virtual ~clsUser32();

	fnAllowSetForegroundWindow AllowSetForegroundWindow;

	protected:
		HMODULE hModule;

	private:
};

#endif // CLSUSER32_H
