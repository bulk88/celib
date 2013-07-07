// bsd-win32.c - wrappers for xcecalls...

#include "celib.h"
#include <float.h>

int
open(const char *path, int mode, int flags)
{
  return xceopen(path, mode, flags);
}

int
read(int fd, void *buf, long size)
{
  return xceread(fd, buf, size);
}

int
write(int fd, void *buf, long size)
{
  return xcewrite(fd, buf, size);
}

int
close(int fd)
{
  return xceclose(fd);
}

int
lseek(int fd, long off, int whence)
{
  return xcelseek(fd, off, whence);
}

int
dup2(int fd1, int fd2)
{
  return -1;
}

// WCE200

int
isinf(double d)
{
  return !_finite(d);
}

int
isnan(double d)
{
  return _isnan(d);
}
