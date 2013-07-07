// testfloat.c - test floats under MIPS/WCE200
//
// Time-stamp: <03/01/02 23:10:56 keuchel@netwave.de>

// With MIPS/WCE200/HPC the compiler generates wrong float compare
// calls when -MD is not specified!!!!

// Same is true for MIPS/WCE201/PSPC

#include <windows.h>

int
wWinMain()
{
  double x, y, z;
  WCHAR buf[126];

  x = 10.0;
  y = 3.0;

  // mul works with FPE
  z = x * y;

  // div works with FPE...
  z = x / y;

#if 1
  // Generates __eqd() call under MIPS/WCE200, which is not in any
  // library.  Compiling without FPE (-QMFPE-), generates an
  // exception. R4000 does not seem to support hard floats.

  if(z == 0.0) 
    MessageBoxW(NULL, L"Z==0", L"Z==0", MB_OK);
#endif

#if 0
  // generates ltd call
  if(z < 0.0)
    MessageBoxW(NULL, L"Z<0", L"Z<0", MB_OK);
#endif

#if 0
  // generates gtd call
  if(z > 0.0)
    MessageBoxW(NULL, L"Z>0", L"Z>0", MB_OK);
#endif

#if 0
  // generates ned call
  if(z != 0.0) 
    MessageBoxW(NULL, L"Z!=0", L"Z!=0", MB_OK);
#endif
    
  swprintf(buf, L"%f", z);

  MessageBoxW(NULL, buf, buf, MB_OK);

  return 0;
}
