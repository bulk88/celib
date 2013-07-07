#ifndef CESHARED_H
#define CESHARED_H 1

#include "celib_defs.h"

XCE_BEGIN_DECLS

#define MAXPIPES        10
#define MAXCONSOLES     10
#define PIPEBUFSIZE     1024

// macros for pipes
#define MAKEPIPEREADHANDLE(I)   ((HANDLE)((0xABCD0000 | 0x1000 | I)))
#define MAKEPIPEWRITEHANDLE(I)  ((HANDLE)((0xABCD0000 | 0x2000 | I)))
#define ISPIPEHANDLE(H)         (((DWORD) H & 0xFFFF0000) == 0xABCD0000)
#define ISPIPEREADHANDLE(H)     (((DWORD) H & 0xFFFFF000) == 0xABCD1000)
#define ISPIPEWRITEHANDLE(H)    (((DWORD) H & 0xFFFFF000) == 0xABCD2000)
#define DECODEPIPEHANDLE(H)     (((DWORD) H & 0x000000FF)) 

#define MAKECONSREADHANDLE(I)   ((HANDLE)((0xBBCD0000 | 0x1000 | I)))
#define MAKECONSWRITEHANDLE(I)  ((HANDLE)((0xBBCD0000 | 0x2000 | I)))
#define ISCONSHANDLE(H)         (((DWORD) H & 0xFFFF0000) == 0xBBCD0000)
#define ISCONSREADHANDLE(H)     (((DWORD) H & 0xFFFFF000) == 0xBBCD1000)
#define ISCONSWRITEHANDLE(H)    (((DWORD) H & 0xFFFFF000) == 0xBBCD2000)
#define DECODECONSHANDLE(H)     (((DWORD) H & 0x000000FF)) 

#define MAKEINHERIT(H)          ((HANDLE)(((DWORD) H)| 0x00000100))
#define ISINHERIT(H)            (((DWORD) H)& 0x00000100)

// Shared data MUST NOT be malloced!
// Pointers MUST NOT be passed to processes!

struct pipe_entry {
  char event_name_canread[20];
  char event_name_canwrite[20];
  DWORD used;
  HANDLE hread;
  HANDLE hwrite;
  BYTE buf[PIPEBUFSIZE];
  DWORD size;
  BYTE *readp;
  DWORD avail;
  DWORD opencntread;
  DWORD opencntwrite;
};

// this entry is copied to shared seg 
// before creating a new process. the
// child will read it.

struct procstart_entry {
  BOOL showwindow;
  char cwd[126];
  char environ[MAX_ENVIRONBLOCKSIZE];
  HANDLE hstdin;
  HANDLE hstdout;
  HANDLE hstderr;
};

XCE_EXPORT BOOL
xceshared_getshowwindow();

XCE_EXPORT HANDLE
xceshared_getstdhandle(DWORD dwId);

XCE_EXPORT BOOL
XCECreatePipe(PHANDLE hRead, PHANDLE hWrite, LPSECURITY_ATTRIBUTES lpsa,
	     DWORD size);

XCE_EXPORT BOOL
XCEReadFile(HANDLE hFile, LPVOID buf, DWORD dwSize, LPDWORD lpdwRead,
	   LPOVERLAPPED lpOverlapped);

XCE_EXPORT BOOL
XCEWriteFile(HANDLE hFile, LPCVOID buf, DWORD dwSize, LPDWORD lpdwWritten,
	    LPOVERLAPPED lpOverlapped);

XCE_EXPORT BOOL 
XCECloseHandle(HANDLE handle);

XCE_EXPORT BOOL
XXDuplicateHandle(HANDLE hProc1, HANDLE hFile1, 
		  HANDLE hProc2, HANDLE *phFile2,
		  DWORD dwAccess, BOOL bInherit, 
		  DWORD dwOpts
		  );

XCE_EXPORT BOOL
XCESetStdHandle(DWORD dwId, HANDLE hFile);

XCE_EXPORT HANDLE
XCEGetStdHandle(DWORD dwId);

XCE_END_DECLS

#endif
