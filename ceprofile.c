#include <windows.h>
#include <celib.h>
#include "ceprofile.h"

XCEPROFILEENTRY __proftab[XCE_MAXPROFILE];

void
XCEProfileDump()
{ 
  int i; 
  __int64 freq; 
  DWORD total; 
   
  QueryPerformanceFrequency((LARGE_INTEGER *) &freq); 
  printf("Freq: %d\n", (DWORD) freq);

  freq /= (__int64) 1000;

  for(i = 0; i < XCE_MAXPROFILE; i++) 
    {
      if(__proftab[i].name) 
	{
	  total = __proftab[i].total / freq;

	  printf("%s: Count: %d Total: %d Average: %d\n", 
		 __proftab[i].name, __proftab[i].count, 
		 total, total / __proftab[i].count
		 ); 
	}
    } 
}
