// ftruncate.c
//
// Time-stamp: <10/02/02 12:49:05 keuchel@netwave.de>

#include "celib.h"

XCE_EXPORT int
xceftruncate(int fd, unsigned long size)
{
  DWORD newpos;

  if(fd < 0 || _fdtab[fd].fd == -1)
    return -1;

  if ((newpos = SetFilePointer(_fdtab[fd].hFile, size, NULL, 
			       FILE_BEGIN)) == -1)
    {
      return -1;
    }

  if(!SetEndOfFile(_fdtab[fd].hFile))
    {
      return -1;
    }

  return 0;
}

XCE_EXPORT int
xcechsize(int fd, unsigned long size)
{
  return xceftruncate(fd, size);
}
