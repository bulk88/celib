// ceio.h
//
// Time-stamp: <26/04/02 18:51:03 keuchel@netwave.de>

#if UNDER_CE <= 201
#include <windows.h>
#endif
#include <winsock.h>
#include "celib.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "ioctl.h"
#include "ceshared.h"
#include "xcetrace.h"

// unused
//int _fmode;

_fdent_t _fdtab[MAXFDS];

HWND console_hwnd;
CONSOLE_READ_FUNC  console_read_func;
CONSOLE_WRITE_FUNC console_write_func;
CONSOLE_IOCTL_FUNC console_ioctl_func;

extern int __xcetrace;

void _initfds();

#define FDCHECK(F) \
  if(F < 0 || F >= MAXFDS || _fdtab[F].fd == -1) \
    { \
      XCETrace("Invalid file handle: %d", F); \
      errno = EBADF; \
      return -1; \
    }

void
_init_io()
{
  HANDLE hFile;
  char stdinpath[126];
  char stdoutpath[126];
  char stderrpath[126];
  char modname[126];
  char path[126];
  char *p;
  char *dirname;

  _initfds();

  XCEGetModuleFileNameA(NULL, path, sizeof(path));

  if((p = xcestrrchr(path, '.')) != NULL)
    *p = 0;
  if((p = xcestrrchr(path, '\\')) != NULL)
    p++;
  else
    p = path;

  strcpy(modname, p);

  if((dirname = xcegetenv("CELIBSTDLOGDIR")) == NULL)
    dirname = "";
  else if(strcmp(dirname, "\\") == 0 || strcmp(dirname, "/") == 0)
    dirname = "";

  sprintf(stdinpath, "%s\\%s-stdin.txt", dirname, modname);
  sprintf(stdoutpath, "%s\\%s-stdout.txt", dirname, modname);
  sprintf(stderrpath, "%s\\%s-stderr.txt", dirname, modname);

  // open the first three file descriptors...
  if((hFile = xceshared_getstdhandle(STD_INPUT_HANDLE)) == NULL)
    {
      xceopen(stdinpath, O_RDONLY|O_CREAT|O_TRUNC, 0666);
      xcesetfiletype(0, XCE_FILE_TYPE_CONSOLE);
    }
  else if(hFile == INVALID_HANDLE_VALUE)
    {
      xceopen("/dev/null", O_RDONLY, 0666);
    }
  else
    {
      XCESetStdHandle(STD_INPUT_HANDLE, hFile);
      xceopen_osfilehandle(hFile, O_RDONLY);
    }

  if((hFile = xceshared_getstdhandle(STD_OUTPUT_HANDLE)) == NULL)
    {
      xceopen(stdoutpath, O_WRONLY|O_CREAT|O_TRUNC, 0666);
      xcesetfiletype(1, XCE_FILE_TYPE_CONSOLE);
    }
  else if(hFile == INVALID_HANDLE_VALUE)
    {
      xceopen("/dev/null", O_WRONLY, 0666);
    }
  else
    {
      XCESetStdHandle(STD_OUTPUT_HANDLE, hFile);
      xceopen_osfilehandle(hFile, O_WRONLY);
    }

  if((hFile = xceshared_getstdhandle(STD_ERROR_HANDLE)) == NULL)
    {
      xceopen(stderrpath, O_WRONLY|O_CREAT|O_TRUNC, 0666);
      xcesetfiletype(2, XCE_FILE_TYPE_CONSOLE);
    }
  else if(hFile == INVALID_HANDLE_VALUE)
    {
      xceopen("/dev/null", O_WRONLY, 0666);
    }
  else
    {
      XCESetStdHandle(STD_ERROR_HANDLE, hFile);
      xceopen_osfilehandle(hFile, O_WRONLY);
    }
}

void
_initfds()
{
  int i;

  static int fdsinitialized = 0;

  if(fdsinitialized)
    return;

  for(i = 0; i < MAXFDS; i++)
    _fdtab[i].fd = -1;

  fdsinitialized = 1;
}

int
_getnewfd()
{
  int i;

  for(i = 0; i < MAXFDS; i++)
    {
      if(_fdtab[i].fd == -1)
	{
	  _fdtab[i].flags = 0;

	  return i;
	}
    }

  XCEShowMessageA("Out of filedescriptors!");

  return -1;
}

HANDLE
xceget_osfhandle(int fd)
{
  if(fd < 0 || fd > MAXFDS || _fdtab[fd].fd == -1)
    return INVALID_HANDLE_VALUE;

  return _fdtab[fd].hFile;
}

int
xcecreat(const char *filename, int pmode)
{
  int fd;

  XCETrace("creat(%s, %d)", filename, pmode);

  fd = xceopen(filename, _O_CREAT|_O_TRUNC|_O_RDWR, pmode);

  XCETrace("creat returns %d", fd);
  
  return fd;
}

int
xceopen(const char *path, int oflag, ...)
{
  va_list ap;
  int pmode = 0;
  wchar_t wfname[MAX_PATH];
  char newpath[MAX_PATH];
  int fd;

  va_start(ap, oflag);
  pmode = va_arg(ap, int);
  va_end(ap);

  XCETrace("open(%s, %x, %o)", path, oflag, pmode);

  // Seems this does not work when the process has pipes as std handles...
  if(xcestricmp(path, "con") == 0 ||
     xcestricmp(path, "con:") == 0 ||
     xcestricmp(path, "/dev/tty") == 0)
    {
      fd = _getnewfd();

      if(fd >= 0)
	{
	  _fdtab[fd].fd = fd;
	  _fdtab[fd].type = XCE_FILE_TYPE_CONSOLE;
	  _fdtab[fd].hFile = (HANDLE) -1;
	}
    }

  else if(xcestricmp(path, "nul") == 0 ||
	  xcestricmp(path, "nul:") == 0 ||
	  xcestricmp(path, "null:") == 0 ||
	  xcestricmp(path, "/dev/nul") == 0 ||
	  xcestricmp(path, "/dev/null") == 0)
    {
      fd = _getnewfd();

      if(fd >= 0)
	{
	  _fdtab[fd].fd = fd;
	  _fdtab[fd].type = XCE_FILE_TYPE_NULL;
	  _fdtab[fd].hFile = (HANDLE) -1;
	}
    }
  else
    {
      strcpy(newpath, path);
      MultiByteToWideChar(CP_ACP, 0, newpath, -1, wfname, COUNTOF(wfname));

      fd = xceopenw(wfname, oflag, pmode);
    }

  XCETrace("open returns %d", fd);

  return fd;
}

int
xceopenw(const wchar_t *fname, int oflag, ...)
{
  HANDLE hFile;
  DWORD fileaccess;
  DWORD fileshare; 
  DWORD filecreate;
  DWORD fileattrib;
  int pmode;
  va_list ap;
  int fd;

  va_start(ap, oflag);
  pmode = va_arg(ap, int);
  va_end(ap);

  _initfds();

  fileshare = FILE_SHARE_READ|FILE_SHARE_WRITE;
  fileattrib = FILE_ATTRIBUTE_NORMAL;

#if 0
  // only if creating new file...
  if(!(pmode & _S_IWRITE))
    fileattrib = FILE_ATTRIBUTE_READONLY;
#endif

  switch( oflag & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) 
    {
    case _O_RDONLY:         /* read access */
      fileaccess = GENERIC_READ;
      break;
    case _O_WRONLY:         /* write access */
      fileaccess = GENERIC_WRITE;
      break;
    case _O_RDWR:           /* read and write access */
      fileaccess = GENERIC_READ | GENERIC_WRITE;
      break;
    default:                /* error, bad oflag */
      errno = EINVAL;
      return -1;
    }

  switch ( oflag & (_O_CREAT | _O_EXCL | _O_TRUNC) ) 
    {
    case 0:
    case _O_EXCL:                   // ignore EXCL w/o CREAT
      filecreate = OPEN_EXISTING;
      break;

    case _O_CREAT:
      filecreate = OPEN_ALWAYS;
      break;

    case _O_CREAT | _O_EXCL:
    case _O_CREAT | _O_TRUNC | _O_EXCL:
      filecreate = CREATE_NEW;
      break;

    case _O_TRUNC:
    case _O_TRUNC | _O_EXCL:        // ignore EXCL w/o CREAT
      filecreate = TRUNCATE_EXISTING;
      break;

    case _O_CREAT | _O_TRUNC:
      filecreate = CREATE_ALWAYS;
      break;

    default:
      // this can't happen ... all cases are covered
      errno = EINVAL;
      return -1;
    }

  if((hFile = XCECreateFileW(fname,
			     fileaccess,
			     fileshare,
			     NULL,
			     filecreate,
			     fileattrib,
			     NULL)) == INVALID_HANDLE_VALUE)
    {
      errno = _winerror2errno(GetLastError());

      XCETRACEW((L"CreateFile(%s): errno = %d oserr = %d",
		 fname, errno, GetLastError()));

      return -1;
    }

  fd = _getnewfd();

  _fdtab[fd].fd = fd;
  _fdtab[fd].type = XCE_FILE_TYPE_FILE;
  _fdtab[fd].hFile = hFile;

  if(oflag & _O_APPEND)
    {
      SetFilePointer(hFile, 0, NULL, FILE_END);
    }

  return fd;
}

int
xceopen_osfilehandle(HANDLE hFile, int oflag)
{
  int fd;

  XCETrace("open_osfilehandle(%x)", hFile);

  if((fd = _getnewfd()) >= 0)
    {
      _fdtab[fd].fd = fd;
      _fdtab[fd].type = XCE_FILE_TYPE_FILE;
      _fdtab[fd].hFile = hFile;
    }

  XCETrace("open_osfilehandle returns %d", fd);

  return fd;
}

int
xceclose(int fd)
{
  XCETrace("close(%d)", fd);

  FDCHECK(fd)

  if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    {
      XCECloseHandle(_fdtab[fd].hFile);
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    {
      closesocket((SOCKET) _fdtab[fd].hFile);
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_CONSOLE)
    {
      XCETrace("Warning: closing console");
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_NULL)
    {
    }

  _fdtab[fd].fd = -1;

  return 0;
}

int
xceread(int fd, void *buf, int count)
{
  DWORD dwRead;

  if(__xcetrace & XCE_TRACE_READWRITE)
    XCETrace("read(fd = %d, size = %d)", fd, count);

  FDCHECK(fd)

  // XServer closes fd 0. I wonder why this shows up so late...
  if(_fdtab[fd].type == XCE_FILE_TYPE_CONSOLE && console_read_func)
    {
      if(__xcetrace & XCE_TRACE_READWRITE)
	XCETrace("Reading from console");

      dwRead = (*console_read_func)(fd, buf, count);
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_CONSOLE && !console_read_func)
    {
      if(__xcetrace & XCE_TRACE_READWRITE)
	XCETrace("Reading from console, which is not set");

      dwRead = 0;
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    {
      if(XCEReadFile(_fdtab[fd].hFile, buf, count, &dwRead, NULL) == FALSE)
	{
	  if(__xcetrace & XCE_TRACE_READWRITE)
	    XCETrace("ReadFile: %d", GetLastError());

	  DebugBreak();

	  errno = EIO;
	  return -1;
	}
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    {
      if((dwRead = recv((SOCKET) _fdtab[fd].hFile, 
			buf, count, 0)) == SOCKET_ERROR)
	{
	  DWORD dwError = WSAGetLastError();

	  XCETrace("recv: %d", dwError);

	  if(dwError == WSAEWOULDBLOCK)
	    {
	      errno = EAGAIN;
	      return -1;
	    }

	  //DebugBreak();

	  errno = EIO;
	  return -1;
	}
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_NULL)
    {
      dwRead = 0;
    }

  if(__xcetrace & XCE_TRACE_READWRITE)
    XCETrace("read returns %d", dwRead);

  return dwRead;
}

int
xcewrite(int fd, void *buf, int size)
{
  DWORD dwWritten;

  if(__xcetrace & XCE_TRACE_READWRITE)
    XCETrace("write(%d, %d)", fd, size);

  FDCHECK(fd)

  if(size == 0)
    return 0;

  if(_fdtab[fd].type == XCE_FILE_TYPE_CONSOLE && console_write_func)
    {
      if(__xcetrace & XCE_TRACE_READWRITE)
	XCETrace("writing to console");
      dwWritten = (*console_write_func)(fd, buf, size);

      if(_fdtab[fd].hFile == (HANDLE) -1)
	{
	  // con does not have a real file handle yet...
	  goto done;
	}
    }
  
  if(_fdtab[fd].type == XCE_FILE_TYPE_FILE ||
     _fdtab[fd].type == XCE_FILE_TYPE_CONSOLE)
    {
      if(XCEWriteFile(_fdtab[fd].hFile, buf, size, &dwWritten, NULL) == FALSE)
	{
	  XCETrace("WriteFile: handle %x: error %d", 
		   _fdtab[fd].hFile, GetLastError());

	  errno = EIO;
	  return -1;
	}
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_SOCKET)
    {
      if((dwWritten = send((SOCKET) _fdtab[fd].hFile, 
			   buf, size, 0)) == SOCKET_ERROR)
	{
	  XCETrace("send: sock %d: error %d", 
		   _fdtab[fd].hFile, GetLastError());

	  errno = EIO;
	  return -1;
	}
    }
  else if(_fdtab[fd].type == XCE_FILE_TYPE_NULL)
    {
      dwWritten = size;
    }

 done:
  if(__xcetrace & XCE_TRACE_READWRITE)
    XCETrace("write returns %d", dwWritten);

  return dwWritten;
}

off_t
xcelseek(int fd, int off, int whence)
{
  off_t newpos;

  if(__xcetrace & XCE_TRACE_READWRITE)
    XCETrace("lseek(%d, %d, %d)", fd, off, whence);

  FDCHECK(fd);

  if(_fdtab[fd].type == XCE_FILE_TYPE_FILE)
    {
      if ((newpos = SetFilePointer(_fdtab[fd].hFile, off, NULL, whence)) == -1)
	{
	  XCETrace("SetFilePointer(%x): error %d", 
		   _fdtab[fd].hFile, GetLastError());
	  
	  errno = EIO;
	  newpos = -1;
	}
    }
  else
    {
      errno = EINVAL;
      newpos = -1;
    }

  if(__xcetrace & XCE_TRACE_READWRITE)
    XCETrace("lseek returns %d", newpos);

  return newpos;
}

int
xceisatty(int fd)
{
  XCETrace("isatty(%d)", fd);

  FDCHECK(fd)

  if(_fdtab[fd].type == XCE_FILE_TYPE_CONSOLE && console_read_func)
    {
      XCETrace("isatty(%d): yes", fd);
      return 1;
    }

  XCETrace("isatty(%d): no", fd);
  return 0;
}

int
xcesetmode(int fd, int mode)
{
  FDCHECK(fd)

  return 0;
}

#ifndef PALM_SIZE
int
xcesetmodefile(FILE *f, int mode)
{
  return _setmode(f, mode);
}
#endif

int
xceinitconsole(CONSOLE_WRITE_FUNC wf, CONSOLE_READ_FUNC rf)
{
  console_write_func = wf;
  console_read_func = rf;

  return 0;
}

int
xceinitconsole2(HWND hwnd, 
		CONSOLE_WRITE_FUNC wf, 
		CONSOLE_READ_FUNC rf,
		CONSOLE_IOCTL_FUNC cf)
{
  console_hwnd = hwnd;
  console_write_func = wf;
  console_read_func = rf;
  console_ioctl_func = cf;

  return 0;
}

int 
xcegetfiletype(int fd)
{
  FDCHECK(fd)
  
  return _fdtab[fd].type;
}

int
xcesetfiletype(int fd, int type)
{
  FDCHECK(fd)

  _fdtab[fd].type = type;
  return 0;
}

// TODO: The fd entry is simply copied. When one of the files
// is closed, the handle is also closed! There should be a 
// refcount on the handles! That would require another table.

int
xcedup(int fd)
{
  int newfd;

  XCETrace("dup(%d)", fd);

  FDCHECK(fd)

  newfd = _getnewfd();

  if(newfd >= 0)
    memcpy(&_fdtab[newfd], &_fdtab[fd], sizeof(_fdent_t));

  XCETrace("dup returns: %d", newfd);

  return newfd;
}

int
xcedup2(int fd1, int fd2)
{
  XCETrace("dup2(%d, %d)", fd1, fd2);

  FDCHECK(fd1)

  if(fd2 < 0 || fd2 >= MAXFDS);
    {
      errno = EBADF;
      return -1;
    }

  if(_fdtab[fd2].fd != -1)
    xceclose(fd2);

  memcpy(&_fdtab[fd2], &_fdtab[fd1], sizeof(_fdent_t));

  XCETrace("dup2 returns: %d", fd2);

  return fd2;
}

int
xcepipe(int fds[2])
{
  HANDLE hread, hwrite;

  if(!XCECreatePipe(&hread, &hwrite, NULL, 0))
    {
      XCETrace("CreatePipe failed");
      errno = EIO;
      return -1;
    }

  fds[0] = xceopen_osfilehandle(hread, O_RDONLY);
  fds[1] = xceopen_osfilehandle(hwrite, O_WRONLY);

  if(fds[0] < 0 || fds[1] < 0)
    {
      return -1;
    }
  return 0;
}

int
xcefcntl(int fd, int mode, int flag)
{
  XCETrace("fcntl(%d, %d, %d)", fd, mode, flag);

  FDCHECK(fd)

  if(ISPIPEHANDLE(xceget_osfhandle(fd)))
    {
      if(mode == F_SETFL)
	{
	  _fdtab[fd].flags = flag;
	  return 0;
	}
      else if(mode == F_GETFL)
	{
	  return _fdtab[fd].flags;
	}
    }
  else if(xcegetfiletype(fd) == XCE_FILE_TYPE_SOCKET)
    {
      int one = 1;
      int zero = 0;

      if(mode == F_SETFL)
	{
	  if(flag == O_NONBLOCK)
	    {
	      // This is done in xceioctlsocket...
	      //_fdtab[fd].flags |= O_NONBLOCK;
	      return xceioctlsocket(fd, FIONBIO, &one);
	    }
	  else if(flag == 0)
	    {
	      //_fdtab[fd].flags &= ~(O_NONBLOCK);
	      return xceioctlsocket(fd, FIONBIO, &zero);
	    }

	  XCEShowMessageA("xcefcntl(): Unhandled flag 0x%x", flag);
	  xceabort();
	}
      else if(mode == F_GETFL)
	{
	  int flag = _fdtab[fd].flags;

	  XCETrace("WARNING: xcefcntl(%d, F_GETFL): 0x%x", fd, flag);

	  return flag;
	}

      XCEShowMessageA("xcefcntl");
      xceabort();

      errno = EINVAL;
      return -1;
    }
  else
    {
      XCETrace("WARNING: fcntl() on non-socket");
    }

  errno = EINVAL;
  return -1;
}

int
xceioctl(int fd, int flag, void *arg)
{
  XCETrace("ioctl(%d, 0x%x)", fd, flag);

  FDCHECK(fd)

  if(xcegetfiletype(fd) == XCE_FILE_TYPE_CONSOLE)
    {
      if(console_ioctl_func)
	return console_ioctl_func(fd, flag, arg);

      errno = EBADF;
      return -1;
    }

#if 0
  XCEShowMessageA("xceioctl");
  xceabort();
#else
  return 0;
#endif

  errno = EINVAL;
  return -1;
}

// called from xceexit()

int
close_all_fds()
{
  int i;

  for(i = 0; i < MAXFDS; i++)
    {
      if(_fdtab[i].fd != -1)
	{
	  if(ISPIPEHANDLE(_fdtab[i].hFile))
	    XCECloseHandle(_fdtab[i].hFile);
	}
    }
  return 0;
}
