// bsd.c - misc bsd funcs, should be macros...
//
// Time-stamp: <08/05/01 21:17:45 keuchel@w2k>

#include <stdlib.h>
#include <string.h>
#include "celib.h"

void *
xcebcopy(void *from, void *to, size_t size)
{
  // handles overlapping
  return memmove(to, from, size);
}

int
xcebcmp(const void *a1, const void *a2, size_t size)
{
  return memcmp(a1, a2, size);
}

void *
xcebzero(void *block, size_t size)
{
  return memset(block, 0, size);
}

char *
xceindex(const char *s, int c)
{
  return strchr(s, c);
}

char *
xcerindex(const char *s, int c)
{
  return xcestrrchr(s, c);
}


