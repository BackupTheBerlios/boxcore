#ifndef CLSMSIMG32_H
#define CLSMSIMG32_H
#include <windows.h>

typedef WINAPI BOOL (*fnAlphaBlend)(HDC hdcDest,int xoriginDest,int yoriginDest,int wDest,int hDest,HDC hdcSrc,int xoriginSrc,int yoriginSrc,int wSrc,int hSrc,BLENDFUNCTION ftn);

class clsMsimg32
{
public:
	clsMsimg32();
	~clsMsimg32();

	fnAlphaBlend AlphaBlend;
protected:
	HMODULE hModule;
private:
};

extern clsMsimg32 msimg32;

#endif // CLSMSIMG32_H
