// wmain.c - non-console sample

#include "celib.h"

int
wWinMain(HINSTANCE hInstance,
	 HINSTANCE hPrevInstance, 
	 LPWSTR    lpCmdLine, 
	 int       nCmdShow)
{
  xceinit(lpCmdLine);

  // You can get argc and argv by using __xceargc and __xceargv.
  // These symbols are exported by the dll.

  // ...

  return 0;
}
