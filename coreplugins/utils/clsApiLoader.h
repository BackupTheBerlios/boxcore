#ifndef CLSAPILOADER_H
#define CLSAPILOADER_H
#include <windows.h>
#include <string>

using std::string;
using std::wstring;

class clsApiLoader
{
	public:
		clsApiLoader();
		virtual ~clsApiLoader();

		void freeLibrary();
		bool requestApiPresence(wstring);
		FARPROC requestApiPointer(string);
	protected:
	private:
		HMODULE hBlackbox;
};

#endif // CLSAPILOADER_H
