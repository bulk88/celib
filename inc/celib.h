#ifndef CELIB_H
#define CELIB_H 1

#include <stdlib.h>
#include <windows.h>

#include "celib_defs.h"
#include "sys/types.h"
#include "errno.h"
#include "cectype.h"
#include "cestdio.h"

#define MAXFDS   100
#define MAXFILES 100

#define NGROUPS_MAX 10

#define MAX_USERNAME_LENGTH 126
#define MAX_COMPUTERNAME_LENGTH 126
#define MAX_ENVIRONBLOCKSIZE  2048

#ifndef emacs
#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4
#define D_OK 8
#endif

#define XCE_FILE_TYPE_FILE    1
#define XCE_FILE_TYPE_SOCKET  2
#define XCE_FILE_TYPE_CONSOLE 3
#define XCE_FILE_TYPE_PIPE    4
#define XCE_FILE_TYPE_NULL    5

#undef _T
#define _T(X) L##X

#define COUNTOF(X) (sizeof(X)/sizeof(X[0]))

XCE_BEGIN_DECLS

typedef struct _fdent
{
  int fd;
  int type;
  HANDLE hFile;
  int flags;
} _fdent_t;

extern _fdent_t _fdtab[];

//////////////////////////////////////////////////////////////////////
// utils

XCE_EXPORT void xcedllinit();
XCE_EXPORT void xceinit(const wchar_t *commandlinew);

XCE_EXPORT void XCEShowMessageA(const char *fmt, ...);
XCE_EXPORT void XCEShowMessageW(const wchar_t *fmt, ...);

XCE_EXPORT int XCEExecuteProcessA(const char *command, int bWait, LPDWORD lpdwProcId);
XCE_EXPORT int XCEWaitProcess(DWORD dwProcId);

XCE_EXPORT void XCEShowWaitCursor();
XCE_EXPORT void XCEShowNormalCursor();

XCE_EXPORT char *XCEGetUnixPath(const char *path);

XCE_EXPORT int XCESetEnvironmentVariableInRegA(const char *name, const char *value);
XCE_EXPORT int XCEGetEnvironmentVariableFromRegA(const char *name, char *buf, int len);

XCE_EXPORT char *XCEToDosPath(char *s);
XCE_EXPORT char *XCEToUnixPath(char *s);

//////////////////////////////////////////////////////////////////////

#ifdef XCEDEBUG
#define XCETRACE(X) XCETrace X
#define XCETRACEW(X) XCETraceW X
#else
#define XCETRACE(X)
#define XCETRACEW(X)
#endif

XCE_EXPORT void XCETrace(const char *fmt, ...);
XCE_EXPORT void XCETraceW(const wchar_t *fmt, ...);

//////////////////////////////////////////////////////////////////////

XCE_END_DECLS

#include "cecrt.h"
#include "cewin32.h"

#ifndef BUILD_CELIB
#include "cecrt_defs.h"
#include "cewin32_defs.h"
#endif

#ifdef PALM_SIZE
#include "stdio-palmsize.h"
#endif

// new bsd macros...
#include "cectype.h"

#endif
