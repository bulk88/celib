// dtoxtime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <19/02/01 20:06:48 keuchel@w2k>

#include "celib.h"
#include <time.h>

time_t 
__loctotime_t(int yr, int mo, int dy, int hr, int mn, int sc, int dstflag)
{
  int tmpdays;
  long tmptim;
  struct tm tb;

  if ( ((long)(yr -= 1900) < _BASE_YEAR) || ((long)yr > _MAX_YEAR) )
    return (time_t)(-1);

  tmpdays = dy + _days[mo - 1];

  if ( !(yr & 3) && (mo > 2) )
    {
      tmpdays++;
    }

  tmptim = (((long)yr - _BASE_YEAR) * 365L +
	    (long)((yr - 1) >> 2) - _LEAP_YEAR_ADJUST + tmpdays) * 24L + hr;

  tmptim = (tmptim * 60L + mn) * 60L + sc;

  __tzset();

  tmptim += _timezone;

  tb.tm_yday = tmpdays;
  tb.tm_year = yr;
  tb.tm_mon  = mo - 1;
  tb.tm_hour = hr;

  if ((dstflag == 1) || ((dstflag == -1) && _daylight &&
			  _isindst(&tb)))
    {
      tmptim += _dstbias;
    }

  return(tmptim);
}
