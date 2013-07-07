// createfilemap.c
//
// Time-stamp: <03/01/02 22:34:54 keuchel@netwave.de>

#include "celib.h"

#if UNDER_CE <= 201
// function is there...
#define CreateFileForMappingW CreateFileForMapping
#endif

#if 1 /* UNDER_CE > 201 */

HANDLE XCEAPI
XCECreateFileForMappingA(
	       LPCSTR fname,
	       DWORD dwDesiredAccess,
	       DWORD dwShareMode,
	       LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	       DWORD dwCreationDisposition,
	       DWORD dwFlagsAndAttributes,
	       HANDLE hTemplateFile
	       )
{
  wchar_t wfname[MAX_PATH];
  HANDLE hFile;

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  hFile = XCECreateFileForMappingW(wfname, dwDesiredAccess, dwShareMode,
				   lpSecurityAttributes, dwCreationDisposition,
				   dwFlagsAndAttributes, hTemplateFile);

  return hFile;
}

HANDLE XCEAPI
XCECreateFileForMappingW(
    LPCWSTR wfname,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    )
{
  HANDLE hFile;
  wchar_t wpath[MAX_PATH];
  wchar_t *p;
  DWORD dwError = 0;

  XCEFixPathW(wfname, wpath);

  SetLastError(0);

  hFile = CreateFileForMappingW(wpath, dwDesiredAccess, dwShareMode,
		      lpSecurityAttributes, dwCreationDisposition,
		      dwFlagsAndAttributes, hTemplateFile);

  if(hFile == INVALID_HANDLE_VALUE)
    {
      dwError = GetLastError();

      XCEShowMessageA("CreateFileForMapping: %d", dwError);

      errno = _winerror2errno(dwError);
    }

  return hFile;
}

#endif // UNDER_CE > 201
