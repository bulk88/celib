// cert0.c - runtime init
//
// Time-stamp: <28/07/01 07:53:42 keuchel@w2k>

#include "celib.h"

int
wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t *lpCommand, int nShow)
{
  xceinit(lpCommand);

  return main(__xceargc, __xceargv, environ);
}
