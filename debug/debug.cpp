/*
 * debug.cpp
 *
 *  Created on: 25 Dec 2008
 *      Author: shaun
 */

#include "debug.h"
#include <windows.h>
#include <cstdio>

void trace_printf(const char *pFileName, unsigned int pLine, const char *pFunction, const char *pFormat, ...)
{
	char buffer[4096];
	sprintf(buffer, "In file %s at line %u.\nFunction %s", pFileName, pLine, pFunction);
	OutputDebugString(buffer);
	va_list arg;
	va_start(arg, pFormat);
	vsprintf (buffer, pFormat, arg);
	strcat(buffer, "\n");
	OutputDebugString(buffer);
}
