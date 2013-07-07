// dllmain.c
//
// Time-stamp: <27/08/01 09:47:39 keuchel@netwave.de>

#include <windows.h>
#include "celib.h"

BOOL APIENTRY 
DllMain( HANDLE hModule, 
	 DWORD  ul_reason_for_call, 
	 LPVOID lpReserved
	 )
{
  switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
      xcedllinit();
      xceshared_attach();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      // Maybe close pipes here... Is it called when
      // process is killed? Yes!
      xceshared_detach();
      break;
    default:
      break;
    }
  return TRUE;
}
