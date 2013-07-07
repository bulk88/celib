// localhost.c
//
// Time-stamp: <03/01/02 22:20:49 keuchel@netwave.de>

#define WIN32_LEAN_AND_MEAN
#if UNDER_CE <= 201
#include <windows.h>
#endif
#include <winsock.h>
#include <malloc.h>
#include "celib_defs.h"

XCE_EXPORT char *
xcelocalhost()
{
  char *name;

  if((name = malloc(256)) == NULL)
    return name;

  gethostname(name, 256);
  return name;
}
