// cemisc.c
//
// Time-stamp: <12/02/01 14:54:58 keuchel@keuchelnt>

#include "celib.h"

BOOL XCEAPI
XCEGetDiskFreeSpaceExA(const char *path, 
		       ULARGE_INTEGER *pl1, 
		       ULARGE_INTEGER *pl2, 
		       ULARGE_INTEGER *pl3)
{
  wchar_t pathw[MAX_PATH];

#ifdef _WIN32_WCE_EMULATION
  return FALSE;
#else
  MultiByteToWideChar(CP_ACP, 0, path, -1, pathw, COUNTOF(pathw));

  return GetDiskFreeSpaceExW(pathw, pl1, pl2, pl3);
#endif
}
