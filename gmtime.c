// cegmtime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <12/02/01 15:59:33 keuchel@keuchelnt>

#include "celib.h"
#include <time.h>

static struct tm tb = { 0 };

struct tm * 
xcegmtime(const time_t *timp)
{
  long caltim = *timp;  
  int islpyr = 0;       
  int tmptim;
  int *mdays;           

  struct tm *ptb = &tb;

  if ( caltim < 0L )
    return(NULL);

  tmptim = (int)(caltim / _FOUR_YEAR_SEC);
  caltim -= ((long)tmptim * _FOUR_YEAR_SEC);

  tmptim = (tmptim * 4) + 70; 

  if ( caltim >= _YEAR_SEC ) 
    {
      tmptim++;                 
      caltim -= _YEAR_SEC;
      
      if ( caltim >= _YEAR_SEC ) 
	{
	  tmptim++;               
	  caltim -= _YEAR_SEC;

	  if ( caltim >= (_YEAR_SEC + _DAY_SEC) ) 
	    {
	      tmptim++;
	      caltim -= (_YEAR_SEC + _DAY_SEC);
	    }
	  else 
	    {
	      islpyr++;
	    }
	}
    }

  ptb->tm_year = tmptim;

  ptb->tm_yday = (int)(caltim / _DAY_SEC);
  caltim -= (long)(ptb->tm_yday) * _DAY_SEC;

  if ( islpyr )
    mdays = _lpdays;
  else
    mdays = _days;

  for ( tmptim = 1 ; mdays[tmptim] < ptb->tm_yday ; tmptim++ ) ;

  ptb->tm_mon = --tmptim;

  ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

  ptb->tm_wday = ((int)(*timp / _DAY_SEC) + _BASE_DOW) % 7;

  ptb->tm_hour = (int)(caltim / 3600);
  caltim -= (long)ptb->tm_hour * 3600L;

  ptb->tm_min = (int)(caltim / 60);
  ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

  ptb->tm_isdst = 0;
  return( (struct tm *)ptb );
}
