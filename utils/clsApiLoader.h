#ifndef CLSAPILOADER_H
#define CLSAPILOADER_H
#include <windows.h>
#include <string>

using std::string;

typedef BOOL (*fnGetApiExtension)(UINT64, UINT64, char *);

class clsApiLoader
{
	public:
		clsApiLoader();
		virtual ~clsApiLoader();
		const char *requestApiName(UINT64 pBranch, UINT64 pApi);
		FARPROC requestApiPointer(string);
	protected:
		fnGetApiExtension GetApiExtension;
	private:
		HMODULE hBlackbox;
		char apiName[256];
};

#endif // CLSAPILOADER_H
