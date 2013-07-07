// Time-stamp: <11/08/01 13:58:49 keuchel@w2k>

#include <windows.h>
#include "celib.h"

#if 0
// Not supported in WinCE < 300
HANDLE
XCECreateSemaphoreA(LPSECURITY_ATTRIBUTES lpsa, LONG initial_count,
		    LONG max_count, LPCTSTR name)
{
  wchar_t *lpnamew = NULL;
  wchar_t namew[126];

  if(name)
    {
      MultiByteToWideChar(CP_ACP, 0, name, -1, namew, sizeof(namew)/2);
      lpnamew = namew;
    }

  return CreateSemaphoreW(lpsa, initial_count, max_count, lpnamew);
}
#endif
