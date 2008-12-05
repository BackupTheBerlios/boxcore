#ifndef CLSUSER32_H
#define CLSUSER32_H

#include <windows.h>

typedef BOOL (*fnAllowSetForegroundWindow)(DWORD dwProcessId);
typedef BOOL (*fnTrackMouseEvent)(LPTRACKMOUSEEVENT lpEventTrack);


class clsUser32
{
	public:
		clsUser32();
		~clsUser32();

	fnAllowSetForegroundWindow AllowSetForegroundWindow;
	fnTrackMouseEvent TrackMouseEvent;

	protected:
		HMODULE hModule;

	private:
};

#endif // CLSUSER32_H
