// ctime.c
// 
// Time-stamp: <12/02/01 16:12:08 keuchel@keuchelnt>

#include "celib.h"
#include <time.h>

char * 
xcectime(const time_t *timp)
{
  struct tm *tmtemp;

  if ((tmtemp = xcelocaltime(timp)) != NULL)
    return(xceasctime((const struct tm *)tmtemp));
  else
    return(NULL);
}
