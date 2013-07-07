// utime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <03/01/02 22:22:22 keuchel@netwave.de>

#if UNDER_CE > 201
#include <stdio.h>
#endif
#include <sys/types.h>
#include <sys/utime.h>
#include <time.h>

#include "celib.h"
#include "fcntl.h"

XCE_EXPORT int 
xceutime (const char *fname, struct _utimbuf *times)
{
  int fh;
  int retval;

  if ((fh = xceopen(fname, _O_RDWR | _O_BINARY)) < 0)
    return(-1);

  retval = xcefutime(fh, times);

  xceclose(fh);
  return(retval);
}

XCE_EXPORT int 
xcefutime (int fh, struct _utimbuf *times)
{
  struct tm *tmb;

  SYSTEMTIME SystemTime;
  FILETIME LocalFileTime;
  FILETIME LastWriteTime;
  FILETIME LastAccessTime;

  struct _utimbuf deftimes;

  if (times == NULL) {
    xcetime(&deftimes.modtime);
    deftimes.actime = deftimes.modtime;
    times = &deftimes;
  }

  if ((tmb = xcelocaltime(&times->modtime)) == NULL) {
    errno = EINVAL;
    return(-1);
  }

  SystemTime.wYear   = (WORD)(tmb->tm_year + 1900);
  SystemTime.wMonth  = (WORD)(tmb->tm_mon + 1);
  SystemTime.wDay    = (WORD)(tmb->tm_mday);
  SystemTime.wHour   = (WORD)(tmb->tm_hour);
  SystemTime.wMinute = (WORD)(tmb->tm_min);
  SystemTime.wSecond = (WORD)(tmb->tm_sec);
  SystemTime.wMilliseconds = 0;

  if ( !SystemTimeToFileTime( &SystemTime, &LocalFileTime ) ||
       !LocalFileTimeToFileTime( &LocalFileTime, &LastWriteTime ) )
    {
      errno = EINVAL;
      return(-1);
    }

  if ((tmb = xcelocaltime(&times->actime)) == NULL) {
    errno = EINVAL;
    return(-1);
  }

  SystemTime.wYear   = (WORD)(tmb->tm_year + 1900);
  SystemTime.wMonth  = (WORD)(tmb->tm_mon + 1);
  SystemTime.wDay    = (WORD)(tmb->tm_mday);
  SystemTime.wHour   = (WORD)(tmb->tm_hour);
  SystemTime.wMinute = (WORD)(tmb->tm_min);
  SystemTime.wSecond = (WORD)(tmb->tm_sec);
  SystemTime.wMilliseconds = 0;

  if ( !SystemTimeToFileTime( &SystemTime, &LocalFileTime ) ||
       !LocalFileTimeToFileTime( &LocalFileTime, &LastAccessTime ) )
    {
      errno = EINVAL;
      return(-1);
    }

  if (!SetFileTime((HANDLE) xceget_osfhandle(fh),
		   NULL,
		   &LastAccessTime,
		   &LastWriteTime
		   ))
    {
      errno = EINVAL;
      return(-1);
    }

  return(0);
}
