// readwrite.c - obsolete
//
// Time-stamp: <10/02/02 12:49:11 keuchel@netwave.de>

#include "celib.h"

#if 0
BOOL XCEAPI
XCEWriteFile(
	     HANDLE hFile,
	     LPCVOID lpBuffer,
	     DWORD nNumberOfBytesToWrite,
	     LPDWORD lpNumberOfBytesWritten,
	     LPOVERLAPPED lpOverlapped
	     )
{
  return WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite,
		   lpNumberOfBytesWritten, lpOverlapped);
}

BOOL XCEAPI
XCEReadFile(
	    HANDLE hFile,
	    LPVOID lpBuffer,
	    DWORD nNumberOfBytesToRead,
	    LPDWORD lpNumberOfBytesRead,
	    LPOVERLAPPED lpOverlapped
	    )
{
  return ReadFile(hFile, lpBuffer, nNumberOfBytesToRead,
		  lpNumberOfBytesRead, lpOverlapped);
}

BOOL
XCECloseHandle(HANDLE h)
{
  return CloseHandle(h);
}

BOOL
XCEDuplicateHandle(HANDLE hProc1, HANDLE hFile1, 
		HANDLE hProc2, HANDLE *phFile2,
		DWORD dwAccess, BOOL bInherit, 
		DWORD dwOpts
		)
{
  SetLastError(ERROR_INVALID_FUNCTION);
  return FALSE;
}

BOOL
XCECreatePipe(PHANDLE hRead, PHANDLE hWrite, LPSECURITY_ATTRIBUTES lpsa,
	      DWORD nSize)
{
  SetLastError(ERROR_INVALID_FUNCTION);
  return FALSE;
}

HANDLE
XCEGetStdHandle(DWORD type)
{
#if 1
  return _fdtab[type].hFile;
#else
  return INVALID_HANDLE_VALUE;
#endif
}

BOOL
XCESetStdHandle(DWORD type, HANDLE handle)
{
  return FALSE;
}

#endif
