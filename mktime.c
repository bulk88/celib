// mktime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <19/02/01 20:07:03 keuchel@w2k>

#include "celib.h"
#include <time.h>

#define ChkAdd(dest, src1, src2)   ( ((src1 >= 0L) && (src2 >= 0L) \
    && (dest < 0L)) || ((src1 < 0L) && (src2 < 0L) && (dest >= 0L)) )

#define ChkMul(dest, src1, src2)   ( src1 ? (dest/src1 != src2) : 0 )

static time_t _make_time_t( struct tm *, int);

time_t 
xcemktime (struct tm *tb)
{
  return( _make_time_t(tb, 1) );
}

time_t 
_mkgmtime (struct tm *tb)
{
  return( _make_time_t(tb, 0) );
}

static time_t 
_make_time_t (struct tm *tb, int ultflag)
{
  long tmptm1, tmptm2, tmptm3;
  struct tm *tbtemp;

  if ( ((tmptm1 = tb->tm_year) < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
    goto err_mktime;

  if ( (tb->tm_mon < 0) || (tb->tm_mon > 11) ) 
    {
      tmptm1 += (tb->tm_mon / 12);

      if ( (tb->tm_mon %= 12) < 0 ) 
	{
	  tb->tm_mon += 12;
	  tmptm1--;
	}

      if ( (tmptm1 < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
	goto err_mktime;
    }

  tmptm2 = _days[tb->tm_mon];

  if ( !(tmptm1 & 3) && (tb->tm_mon > 1) )
    tmptm2++;

  tmptm3 = (tmptm1 - _BASE_YEAR) * 365L + ((tmptm1 - 1L) >> 2)
    - _LEAP_YEAR_ADJUST;

  tmptm3 += tmptm2;

  tmptm1 = tmptm3 + (tmptm2 = (long)(tb->tm_mday));
  if ( ChkAdd(tmptm1, tmptm3, tmptm2) )
    goto err_mktime;

  tmptm2 = tmptm1 * 24L;
  if ( ChkMul(tmptm2, tmptm1, 24L) )
    goto err_mktime;

  tmptm1 = tmptm2 + (tmptm3 = (long)tb->tm_hour);
  if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
    goto err_mktime;

  tmptm2 = tmptm1 * 60L;
  if ( ChkMul(tmptm2, tmptm1, 60L) )
    goto err_mktime;

  tmptm1 = tmptm2 + (tmptm3 = (long)tb->tm_min);
  if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
    goto err_mktime;

  tmptm2 = tmptm1 * 60L;
  if ( ChkMul(tmptm2, tmptm1, 60L) )
    goto err_mktime;

  tmptm1 = tmptm2 + (tmptm3 = (long)tb->tm_sec);
  if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
    goto err_mktime;

  if  ( ultflag ) 
    {
    __tzset();

    tmptm1 += _timezone;

    if ( (tbtemp = xcelocaltime(&tmptm1)) == NULL )
      goto err_mktime;

    if ( (tb->tm_isdst > 0) || ((tb->tm_isdst < 0) &&
				(tbtemp->tm_isdst > 0)) ) 
      {
	tmptm1 += _dstbias;
	tbtemp = xcelocaltime(&tmptm1);
      }
    }
  else 
    {
      if ( (tbtemp = xcegmtime(&tmptm1)) == NULL )
	goto err_mktime;
    }

  *tb = *tbtemp;
  return (time_t)tmptm1;

 err_mktime:
  return (time_t)(-1);
}
