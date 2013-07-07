// ceunicode.c
//
// Time-stamp: <16/04/01 19:16:10 keuchel@w2k>

#include "celib.h"

wchar_t * XCEAPI
XCEMakeUnicode(const char *s, int len)
{
  wchar_t *buf;

  if(len == -1)
    len = strlen(s) + 1;

  buf = malloc(len * sizeof(wchar_t));

  MultiByteToWideChar(CP_ACP, 0, s, len, buf, len);

  return buf;
}

