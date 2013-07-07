// ceexec.c
//
// Time-stamp: <10/02/02 12:30:43 keuchel@netwave.de>

#include "celib.h"
#include "cecrt.h"

int
xceexecv(const char *cmdname, const char *const *argv)
{
  XCEShowMessageA("xceexecv");
  xceabort();
  return -1;
}

int 
xceexecvp(const char *cmdname, const char *const *argv)
{
  XCEShowMessageA("xceexecvp");
  xceabort();
  return -1;
}


int
xceexecl(char *arg0, ...)
{
  XCEShowMessageA("xceexecl");
  xceabort();
  return -1;
}

int
xceexeclp(char *arg0, ...)
{
  XCEShowMessageA("xceexeclp");
  xceabort();
  return -1;
}


