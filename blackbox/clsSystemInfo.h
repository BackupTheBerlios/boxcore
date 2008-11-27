#ifndef CLSSYSTEMINFO_H
#define CLSSYSTEMINFO_H

#include "windows.h"

class clsSystemInfo
{
	public:
		clsSystemInfo();
		virtual ~clsSystemInfo();

		bool isOs64Bits() {return arch64;}

		bool isOsNT() {return typeNT;}

		bool isOsVista() {return osVista;}
		bool isOs2kXP() {return os2kXP;}

		bool isBlackbox64Bits() {return prog64;}
		bool isBlackboxNativeBits() {return arch64==prog64;}
	protected:
	private:
		OSVERSIONINFO osInfo;

		bool typeNT;
		bool osVista;
		bool os2kXP;

		bool arch64;

		bool prog64;
};

#endif // CLSSYSTEMINFO_H
