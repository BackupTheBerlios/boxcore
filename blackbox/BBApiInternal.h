#ifndef BBAPIINTERNAL_H
#define BBAPIINTERNAL_H

#include "BBApi.h"

//Branch codes
const UINT64 branchCarsomyr = (1<<10);

//Api codes for branch 1<<10
const UINT64 apiTrayIconEvent = 1;

extern "C" API_EXPORT BOOL TrayIconEvent(HWND, UINT, UINT, WPARAM, LPARAM);
extern "C" API_EXPORT void SetTaskbarPos(int, int, int, int, UINT);

#endif //BBAPIINTERNAL_H