// testfloatio.c - test floats under MIPS/WCE200
//
// Time-stamp: <08/05/01 23:04:56 keuchel@w2k>

// With MIPS/WCE200 the compiler generates wrong float compare
// calls when -MD is not specified!!!!

#include <windows.h>
#include "celib.h"

int
wWinMain()
{
  char buf[126];
  double x, y, z;
  int n;
  int i, j, k;
  HMODULE hMod;

  OSVERSIONINFO osinfo;

  hMod = XCEGetModuleHandleA(NULL);
  XCEShowMessageA("Mod: %x\n", hMod);
  return 0;

  if(!XCEGetVersionExA(&osinfo))
    XCEShowMessageA("Cannot get version info");
  else
    XCEShowMessageA("%d.%d 0x%x\r\n%s", 
		    osinfo.dwMajorVersion, // 2
		    osinfo.dwMinorVersion, // 11
		    osinfo.dwPlatformId,   // 3 = VER_PLATFORM_WIN32_CE
		    osinfo.szCSDVersion);

  x = 10.0;
  y = 3.0;
  z = x / y;

#if 0  
  sprintf(buf, "%f", z);
  XCEShowMessageA("%s", buf);
  n = sscanf(buf, "%lf", &x);
  XCEShowMessageA("%d: %f", n, x);
#endif

  n = sscanf("1 -2 3 1.23 4.66 -10.99", "%d %d %d %lf %lf %lf",
	     &i, &j, &k, &x, &y, &z);

  XCEShowMessageA("%d: %d %d %d %f %f %f", n, i, j, k, x, y, z);

  return 0;
}
