// cestdio.c
//
// Time-stamp: <13/02/02 07:36:29 keuchel@netwave.de>

#include "celib.h"
#include "sys/stat.h"
#include "fcntl.h"

// we need to fix path...

#ifndef PALM_SIZE
FILE *
xcefopen(const char *path, const char *mode)
{
  char newpath[MAX_PATH];
  wchar_t wpath[MAX_PATH];
  wchar_t wpathnew[MAX_PATH];
  wchar_t wmode[10];
  FILE *f;
  int fd;

  _initfds();

  // WinCE has no NUL!
  if(xcestricmp(path, "nul") == 0 ||
     xcestricmp(path, "nul:") == 0 ||
     xcestricmp(path, "null") == 0 ||
     xcestricmp(path, "null:") == 0 ||
     xcestricmp(path, "/dev/nul") == 0 ||
     xcestricmp(path, "/dev/null") == 0)
    {
      strcpy(newpath, "/dev/null");
    }
  else
    {
      strcpy(newpath, path);
    }

  MultiByteToWideChar(CP_ACP, 0, newpath, -1, wpath, COUNTOF(wpath));
  MultiByteToWideChar(CP_ACP, 0, mode, -1, wmode, COUNTOF(wmode));

  XCEFixPathW(wpath, wpathnew);

  //wprintf(L"xcefopen(%s)\n", wpathnew);

  if((f = _wfopen(wpathnew, wmode)) == NULL)
    {
      XCETRACEW((L"xcefopen(%s): oserr = %d\n", wpathnew, GetLastError()));

      return NULL;
    }

  // needed for xcefileno...
  fd = _getnewfd();

  _fdtab[fd].fd = fd;
  _fdtab[fd].type = XCE_FILE_TYPE_FILE;
  // seems to be os handle...
  _fdtab[fd].hFile = fileno(f);

  return f;
}
#endif

// there is only a wide version!

#ifndef PALM_SIZE
FILE *
xcefreopen(const char *path, const char *mode, FILE *stream)
{
  wchar_t wpath[MAX_PATH];
  wchar_t wpathnew[MAX_PATH];
  wchar_t wmode[10];

  MultiByteToWideChar(CP_ACP, 0, path, -1, wpath, COUNTOF(wpath));
  MultiByteToWideChar(CP_ACP, 0, mode, -1, wmode, COUNTOF(wmode));

  XCEFixPathW(wpath, wpathnew);

  //wprintf(L"xcefopen(%s)\n", wpathnew);

  return _wfreopen(wpathnew, wmode, stream);
}
#endif

#ifndef PALM_SIZE
void
xcefclose(FILE *f)
{
  int fd;

  for(fd = 0; fd < MAXFDS; fd++)
    {
      if(_fdtab[fd].hFile == fileno(f))
	{
	  _fdtab[fd].fd = -1;
	  break;
	}
    }
  fclose(f);
}
#endif

#ifndef PALM_SIZE
int
xcefileno(FILE *f)
{
  int fd;

  for(fd = 0; fd < MAXFDS; fd++)
    {
      if(_fdtab[fd].hFile == fileno(f))
	{
	  return _fdtab[fd].fd;
	}
    }

  XCETRACE(("Cannot get fileno for file %x\n", f));

  return -1;
}
#endif

#ifndef PALM_SIZE
void
xcerewind(FILE *f)
{
  fseek(f, 0L, SEEK_SET);
}
#endif

#ifndef PALM_SIZE
FILE *
xcefdopen(int fd, const char *mode)
{
  wchar_t wmode[10];

  if(fd < 0 || _fdtab[fd].fd == -1)
    return NULL;

  if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    {
      errno = EINVAL;
      return NULL;
    }

  MultiByteToWideChar(CP_ACP, 0, mode, -1, wmode, COUNTOF(wmode));

  return _wfdopen(_fdtab[fd].hFile, wmode);
}
#endif

#if 0
FILE *
xcepopen(const char *command, const char *mode)
{
  errno = ENOSYS;
  return NULL;
}

int
xcepclose(FILE *f)
{
  errno = ENOSYS;
  return -1;
}
#endif

#if 1
FILE *
xcepopen_read(const char *command, const char *mode)
{
  STARTUPINFO startinfo;
  PROCESS_INFORMATION procinfo;
  HANDLE hReadPipe, hWritePipe;
  HANDLE hReadPipeDup;
  SECURITY_ATTRIBUTES sa;
  int fd;
  FILE *f;

  sa.nLength = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

  if(!XCECreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
    {
      return NULL;
    }

  if(!XCEDuplicateHandle(GetCurrentProcess(), hReadPipe,
			 GetCurrentProcess(), &hReadPipeDup,
			 0, FALSE, DUPLICATE_SAME_ACCESS))
    {
      return NULL;
    }

  XCECloseHandle(hReadPipe);

  memset(&startinfo, 0, sizeof(startinfo));
  startinfo.cb = sizeof(startinfo);
  startinfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  startinfo.dwFlags |= STARTF_FORCEOFFFEEDBACK;
  startinfo.wShowWindow = SW_HIDE;

  startinfo.hStdInput = INVALID_HANDLE_VALUE;
  startinfo.hStdOutput = hWritePipe;
  startinfo.hStdError = hWritePipe;

  if(!XCECreateProcessA(NULL, command, NULL, NULL, TRUE, 0, NULL, 
			NULL, &startinfo, &procinfo))
    {
      XCECloseHandle(hWritePipe);
      XCECloseHandle(hReadPipeDup);
      return NULL;
    }

  XCECloseHandle(hWritePipe);

  if((fd = xceopen_osfilehandle(hReadPipeDup, O_RDONLY)) < 0)
    return NULL;

  f = palm_fdopen(fd, "r");

  return f;
}

FILE *
xcepopen_write(const char *command, const char *mode)
{
  STARTUPINFO startinfo;
  PROCESS_INFORMATION procinfo;
  HANDLE hReadPipe, hWritePipe;
  HANDLE hWritePipeDup;
  SECURITY_ATTRIBUTES sa;
  int fd;
  FILE *f;

  sa.nLength = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

  if(!XCECreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
    {
      return NULL;
    }

  if(!XCEDuplicateHandle(GetCurrentProcess(), hWritePipe,
			 GetCurrentProcess(), &hWritePipeDup,
			 0, FALSE, DUPLICATE_SAME_ACCESS))
    {
      return NULL;
    }

  XCECloseHandle(hWritePipe);

  memset(&startinfo, 0, sizeof(startinfo));
  startinfo.cb = sizeof(startinfo);
  startinfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  startinfo.dwFlags |= STARTF_FORCEOFFFEEDBACK;
  startinfo.wShowWindow = SW_HIDE;

  startinfo.hStdInput = hReadPipe;
  startinfo.hStdOutput = NULL;
  startinfo.hStdError = NULL;

  if(!XCECreateProcessA(NULL, command, NULL, NULL, TRUE, 0, NULL, 
			NULL, &startinfo, &procinfo))
    {
      XCECloseHandle(hReadPipe);
      XCECloseHandle(hWritePipeDup);
      return NULL;
    }

  XCECloseHandle(hReadPipe);

  if((fd = xceopen_osfilehandle(hWritePipeDup, O_WRONLY)) < 0)
    return NULL;

  f = palm_fdopen(fd, "w");

  return f;
}

FILE *
xcepopen(const char *command, const char *mode)
{
  if(mode[0] == 'r')
    return xcepopen_read(command, mode);
  else if(mode[0] == 'w')
    return xcepopen_write(command, mode);

  XCETrace("popen: unhandled mode %s", mode);
  errno = EINVAL;

  return NULL;
}

int
xcepclose(FILE *f)
{
  return palm_fclose(f);
}
#endif

int
xceprinterror(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
#ifdef PALM_SIZE
  return palm_vfprintf(palm_stderr, fmt, ap);
#else
  return vfprintf(stderr, fmt, ap);
#endif  
}
