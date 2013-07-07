// ceerrno.c
//
// Time-stamp: <18/02/01 19:33:25 keuchel@w2k>

#include "celib.h"
#include "errno.h"

#ifndef _MT
int errno;
#endif

char *_error_list[] = {
  "No error", // 0
  "EPERM",
  "No such file or directory", // ENOENT
  "ESRCH",
  "EINTR",
  "EIO",
  "ENXIO",
  "E2BIG",
  "ENOEXEC",
  "Bad file descriptor", // EBADF
  "ECHILD",
  "EAGAIN",
  "ENOMEM",
  "Permission denied", // EACCESS
  "EFAULT",
  "NOSUCHERROR",
  "EBUSY",
  "File exists", // EEXISTS
  "EXDEV",
  "ENODEV",
  "ENOTDIR",
  "EISDIR",
  "Invalid argument", // EINVAL
  "ENFILE",
  "EMFILE",
  "ENOTTY",
  "NOSUCHERROR",
  "EFBIG",
  "Disk full", // ENOSPC
  "ESPIPE",
  "EROFS",
  "EMLINK",
  "Broken pipe", // EPIPE
  "EDOM",
  "ERANGE",
  "NOSUCHERROR",
  "EDEADLK",
  "Unimplemented system call", // ENOSYS
  NULL
};

int
_winerror2errno(DWORD werror)
{
  switch(werror)
    {
    case 0:
      return 0;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
      return ENOENT;
    case ERROR_ACCESS_DENIED:
      return EACCES;
    case ERROR_DEV_NOT_EXIST:
      return ENODEV;
    case ERROR_ALREADY_EXISTS:
      return EEXIST;

    default:
      return EOSERR;
    }
  return EOSERR;
}

void
xceperror(const char *s)
{
  char buf[126];

  xceprinterror("%s: %s\n", s, xcestrerror(errno));
}

char *
xcestrerror(int n)
{
  static char buf[125];

  if(n < 0)
    {
      sprintf(buf, "Strange Error %d", n);
    }
  else if(n == EOSERR)
    {
      sprintf(buf, "OsError: %d", GetLastError());
    }
  else if(n > ENOSYS)
    {
      // maybe its a socket error/windows error
      strcpy(buf, xcewinstrerror(n));
    }
  else
    {
      strcpy(buf, _error_list[n]);
    }

  return buf;
}
