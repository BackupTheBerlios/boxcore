/**
  * debug.h
  *
  *  Created on: 25 Dec 2008
  *      Author: shaun
  */

#ifndef DEBUG_H

void trace_printf(const char *pFileName, unsigned int pLine, const char *pFunction, const char *pFormat, ...);

#ifndef NDEBUG
#define PRINT(A) OutputDebugString(A)
#define TRACE(...) trace_printf(__FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#else
#define PRINT(A)
#define TRACE(...)
#endif //NDEBUG
#define DEBUG_H


#endif /* DEBUG_H */
