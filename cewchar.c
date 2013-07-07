// cewchar.c
//
// Time-stamp: <13/05/01 10:57:48 keuchel@w2k>

#include "celib.h"
#include "cewchar.h"

int 
xcewctomb(char *mbchar, wchar_t wchar)
{
  *mbchar = (char) wchar;

  return 1;
}

int 
xcembtowc(wchar_t *wchar, const char *mbchar, size_t count)
{
  if(*mbchar == 0)
    return 0;

  *wchar = *mbchar;

  return 1;
}

int 
xcemblen(const char *mbstr, size_t count)
{
  if(*mbstr == 0)
    return 0;

  return 1;
}


