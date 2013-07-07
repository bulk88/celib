// calloc.c
//
// Time-stamp: <12/02/01 16:00:19 keuchel@keuchelnt>

#include "celib.h"

void *
xcecalloc(size_t num, size_t size)
{
  void *p;

  size *= num;

  if((p = malloc(size)) == NULL)
    return NULL;

  memset(p, 0, size);

  return p;
}
