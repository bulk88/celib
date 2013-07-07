// time.c - adapted from Microsoft C Runtime

#include "celib.h"
#include "time.h"

long _timezone = 8 * 3600L; /* Pacific Time Zone */
int _daylight = 1;          /* Daylight Saving Time (DST) in timezone */
long _dstbias = -3600L;     /* DST offset in seconds */

/* note that NT Posix's TZNAME_MAX is only 10 */

static char tzstd[64] = { "PST" };
static char tzdst[64] = { "PDT" };

int _lpdays[] = {
  -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

int _days[] = {
  -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
};

char *_tzname[2] = { tzstd, tzdst };

/*  Day names must be Three character abbreviations strung together */

const char __dnames[] = {
  "SunMonTueWedThuFriSat"
};

/*  Month names must be Three character abbreviations strung together */

const char __mnames[] = {
  "JanFebMarAprMayJunJulAugSepOctNovDec"
};

/*
 * Cache holding the last time (GMT) for which the Daylight time status was
 * determined by an API call.
 */
static SYSTEMTIME gmt_cache;

/*
 * Three values of dstflag_cache and dstflag (local variable in code
 * below)
 */
#define DAYLIGHT_TIME   1
#define STANDARD_TIME   0
#define UNKNOWN_TIME    -1

static int dstflag_cache;

time_t 
xcetime(time_t *timeptr)
{
  time_t tim;

  SYSTEMTIME loct, gmt;
  TIME_ZONE_INFORMATION tzinfo;
  DWORD tzstate;
  int dstflag;

  /*
   * Get local time from Win32
   */
  GetLocalTime( &loct );

  /*
   * Determine whether or not the local time is a Daylight Saving
   * Time. On Windows NT, the GetTimeZoneInformation API is *VERY*
   * expensive. The scheme below is intended to avoid this API call in
   * many important case by caching the GMT value and dstflag.In a
   * subsequent call to time(), the cached value of dstflag is used
   * unless the new GMT differs from the cached value at least in the
   * minutes place.
   */
  GetSystemTime( &gmt );

  if ( (gmt.wMinute == gmt_cache.wMinute) &&
       (gmt.wHour == gmt_cache.wHour) &&
       (gmt.wDay == gmt_cache.wDay) &&
       (gmt.wMonth == gmt_cache.wMonth) &&
       (gmt.wYear == gmt_cache.wYear) )
    {
      dstflag = dstflag_cache;
    }
  else
    {
      if ( (tzstate = GetTimeZoneInformation( &tzinfo )) != 0xFFFFFFFF )
	{
	  /*
	   * Must be very careful in determining whether or not DST is
	   * really in effect.
	   */
	  if ( (tzstate == TIME_ZONE_ID_DAYLIGHT) &&
	       (tzinfo.DaylightDate.wMonth != 0) &&
	       (tzinfo.DaylightBias != 0) )
	    dstflag = DAYLIGHT_TIME;
	  else
	    /*
	     * When in doubt, assume standard time
	     */
	    dstflag = STANDARD_TIME;
	}
      else
	dstflag = UNKNOWN_TIME;

      dstflag_cache = dstflag;
      gmt_cache = gmt;
    }

  /* convert using our private routine */

  tim = __loctotime_t( (int)loct.wYear,
		       (int)loct.wMonth,
		       (int)loct.wDay,
		       (int)loct.wHour,
		       (int)loct.wMinute,
		       (int)loct.wSecond,
		       dstflag );

  if (timeptr)
    *timeptr = tim;         /* store time if requested */

  return tim;
}
