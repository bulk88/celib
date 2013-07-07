// cestdlib.c
//
// Time-stamp: <15/03/02 20:49:53 keuchel@netwave.de>

#include "celib.h"
#include "sys/stat.h"

int __umask;

char *
xcegetcwd(char *buf, int size)
{
  char wd[MAX_PATH];

  XCEGetCurrentDirectoryA(sizeof(wd), wd);

  if(buf)
    {
      strncpy(buf, wd, size);
      return buf;
    }

  return xcestrdup(wd);
}

void
xceabort()
{
  char buf[256];

  XCEGetModuleFileNameA(NULL, buf, sizeof(buf));
  XCEShowMessageA("%s - Abort", buf);

  DebugBreak();

  exit(1);
}

int 
xcesystem(const char *cmd)
{
  return XCEExecuteProcessA(cmd, TRUE, NULL);
}

int
xcelocking(int fd, int mode, int nbytes)
{
  return 0;
}

int
xceumask(int mask)
{
  int oldumask = __umask;

  __umask = mask;

  return oldumask;
}

int
xcemkdir(const char *dirname)
{
  BOOL res = XCECreateDirectoryA(dirname, NULL);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  return 0;
}

int
xcermdir(const char *dirname)
{
  BOOL res = XCERemoveDirectoryA(dirname);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  return 0;
}

int
xcerename(const char *old, const char *new)
{
  struct xcestat st;
  BOOL res;

  if(xcestat(new, &st) == 0)
    {
      if(xceremove(new) < 0)
	{
	  return -1;
	}
    }

  res = XCEMoveFileA(old, new);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());

      return -1;
    }

  return 0;
}

int
xceremove(const char *path)
{
  BOOL res = XCEDeleteFileA(path);

  if(res != TRUE)
    {
      errno =_winerror2errno(GetLastError());

      return -1;
    }

  return 0;
}

int
xcechmod(const char *fname, int mode)
{
  DWORD dwAttr;

  if((dwAttr = XCEGetFileAttributesA(fname)) == 0xFFFFFFFF)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  if(mode & S_IWRITE)
    dwAttr &= ~(FILE_ATTRIBUTE_READONLY);
  else
    dwAttr |= (FILE_ATTRIBUTE_READONLY);

  if(XCESetFileAttributesA(fname, dwAttr) == FALSE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }

  return 0;
}

int
xceaccess(const char *fname, int mode)
{
  struct xcestat st;

  if(xcestat(fname, &st) < 0)
    return -1;

  return 0;
}

int
xceunlink(const char *fname)
{
  BOOL res = XCEDeleteFileA(fname);

  if(res == FALSE)
    {
      errno =_winerror2errno(GetLastError());
      return -1;
    }
  return 0;
}

int
xcegetpid()
{
  DWORD dwProcId = GetCurrentProcessId();

  return dwProcId & 0x7FFFFFFF;
}

int
xcechdir(const char *dirname)
{
  if(XCESetCurrentDirectoryA(dirname) == FALSE)
    return -1;

  return 0;
}

char *
xcemktemp(char *template)
{
  int i;
  struct xcestat st;
  char *pt;

  for(pt = template; *pt && *pt != 'X'; pt++);

  for(i = 1; i < 9999; i++)
    {
      sprintf(pt, "%05d", i);
      if(xcestat(template, &st) < 0 && errno == ENOENT)
	return template;
    }

  return NULL;
}

// there must be other things changed, path search, for example...

int
xcechroot(const char *dirname)
{
#ifdef USE_CHROOT
  // this sets errno already...
  if(XCESetCurrentRootDirectoryA(dirname) == FALSE)
     return -1;

  return 0;
#else
  errno = ENOSYS;
  return -1;
#endif
}

// It seems that this must not be called from a dll, at least under
// emulation...

// FIXME: There seems to be a file close problem, at least in Perl
// under Emulation. Open files are not flushed. Maybe I modified 
// exit() in perl...

void
xceexit(int code)
{
  XCETrace("exit(%d)", code);

#ifdef PALM_SIZE
  palm_fcloseall();
#endif

  close_all_fds();

#ifdef _WIN32_WCE_EMULATION
  TerminateProcess(GetCurrentProcess(), code);
  //exit(code);
#else
  exit(code);
#endif
}

long
xceclock()
{
  return GetTickCount();
}

long
xcetimes(struct tms *buf)
{
  FILETIME tcreat, texit, tkernel, tuser;

  memset(buf, 0, sizeof(struct tms));

#if UNDER_CE <= 201
  errno = ENOSYS;
  return -1;
#else
  GetThreadTimes(GetCurrentThread(), &tcreat, &texit, &tkernel, &tuser);
#endif

  buf->tms_utime = (__int64) *(__int64 *) &tuser / (__int64) 10000;
  buf->tms_stime = (__int64) *(__int64 *) &tkernel / (__int64) 10000;
  buf->tms_cutime = -1;

  //printf("utime: %d stime: %d\n", buf->tms_utime, buf->tms_stime);

  return GetTickCount();
}
