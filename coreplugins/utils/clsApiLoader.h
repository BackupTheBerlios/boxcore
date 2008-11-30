#ifndef CLSAPILOADER_H
#define CLSAPILOADER_H
#include <windows.h>

class clsApiLoader
{
	public:
		clsApiLoader();
		virtual ~clsApiLoader();

		void freeLibrary();
		bool requestApiPresence(TCHAR *pIdentifier);
		FARPROC requestApiPointer(CHAR *pApiName);
	protected:
	private:
		HMODULE hBlackbox;
};

#endif // CLSAPILOADER_H
