// localtime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <21/04/01 16:01:35 keuchel@w2k>

#include "celib.h"
#include <limits.h>
#include <time.h>

XCE_EXPORT struct tm * 
xcelocaltime (const time_t *ptime)
{
  struct tm *ptm;
  long ltime;

  if ( (long)*ptime < 0L )
    return( NULL );

  __tzset();

  if ( (*ptime > 3 * _DAY_SEC) && (*ptime < LONG_MAX - 3 * _DAY_SEC) ) 
    {
      ltime = (long)*ptime - _timezone;
      ptm = xcegmtime( (time_t *)&ltime );

      if ( _daylight && _isindst( ptm ) ) 
	{
	  ltime -= _dstbias;
	  ptm = xcegmtime( (time_t *)&ltime );
	  ptm->tm_isdst = 1;
	}
    }
  else 
    {
      ptm = xcegmtime( ptime );

      if ( _isindst(ptm) )
	ltime = (long)ptm->tm_sec - (_timezone + _dstbias);
      else
	ltime = (long)ptm->tm_sec - _timezone;
      ptm->tm_sec = (int)(ltime % 60);
      if ( ptm->tm_sec < 0 ) {
	ptm->tm_sec += 60;
	ltime -= 60;
      }

      ltime = (long)ptm->tm_min + ltime/60;
      ptm->tm_min = (int)(ltime % 60);
      if ( ptm->tm_min < 0 ) {
	ptm->tm_min += 60;
	ltime -= 60;
      }

      ltime = (long)ptm->tm_hour + ltime/60;
      ptm->tm_hour = (int)(ltime % 24);
      if ( ptm->tm_hour < 0 ) 
	{
	  ptm->tm_hour += 24;
	  ltime -=24;
	}

      ltime /= 24;

      if ( ltime > 0L ) 
	{
	  ptm->tm_wday = (ptm->tm_wday + ltime) % 7;
	  ptm->tm_mday += ltime;
	  ptm->tm_yday += ltime;
	}
      else if ( ltime < 0L ) 
	{
	  ptm->tm_wday = (ptm->tm_wday + 7 + ltime) % 7;
	  if ( (ptm->tm_mday += ltime) <= 0 ) {
	    ptm->tm_mday += 31;
	    ptm->tm_yday = 364;
	    ptm->tm_mon = 11;
	    ptm->tm_year--;
	  }
	  else 
	    {
	      ptm->tm_yday += ltime;
	    }
	}
    }


  return(ptm);
}
