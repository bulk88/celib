#include "celib.h"

unsigned int
xcesleep(unsigned int secs)
{
  Sleep(secs * 1000);
  return 0;
}

