// ftime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <23/12/01 12:14:32 keuchel@netwave.de>

// NOTE: Did not return millis (problem with GetSystemTime())

#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include "celib.h"

#define EPOCH_BIAS  116444736000000000i64

typedef union {
  unsigned __int64 ft_scalar;
  FILETIME ft_struct;
} FT;

static time_t elapsed_minutes_cache = 0;
static unsigned __int64 millis_at_system_start = 0;

#define DAYLIGHT_TIME   1
#define STANDARD_TIME   0
#define UNKNOWN_TIME    -1

static int dstflag_cache = UNKNOWN_TIME;

#if 0
void 
xceftime_old(struct _timeb *tp)
{
  FT nt_time;
  time_t t;
  TIME_ZONE_INFORMATION tzinfo;
  DWORD tzstate;

  __tzset();

  XCEGetSystemTimeAsFileTime(&(nt_time.ft_struct));

  if((t = (time_t)(nt_time.ft_scalar / 600000000i64))
       != elapsed_minutes_cache)
    {
      if((tzstate = GetTimeZoneInformation(&tzinfo)) != 0xFFFFFFFF)
	{
	  if ( (tzstate == TIME_ZONE_ID_DAYLIGHT) &&
	       (tzinfo.DaylightDate.wMonth != 0) &&
	       (tzinfo.DaylightBias != 0) )
	    dstflag_cache = DAYLIGHT_TIME;
	  else
	    dstflag_cache = STANDARD_TIME;
	}
      else
	dstflag_cache = UNKNOWN_TIME;

      elapsed_minutes_cache = t;
    }

  tp->time = (time_t)((nt_time.ft_scalar - EPOCH_BIAS) / 10000000i64);
  tp->millitm = 0;
  tp->timezone = (short)(_timezone / 60);
  tp->dstflag = (short)dstflag_cache;
}
#endif

// This new version with milliseconds seems to work well...

void 
xceftime(struct _timeb *tp)
{
  FT nt_time;
  time_t t;
  TIME_ZONE_INFORMATION tzinfo;
  DWORD tzstate;
  __int64 current_millis;

  if(millis_at_system_start == 0)
    {
      __tzset();

      XCEGetSystemTimeAsFileTime(&(nt_time.ft_struct));

      if((t = (time_t)(nt_time.ft_scalar / 600000000i64))
	 != elapsed_minutes_cache)
	{
	  if((tzstate = GetTimeZoneInformation(&tzinfo)) != 0xFFFFFFFF)
	    {
	      if ( (tzstate == TIME_ZONE_ID_DAYLIGHT) &&
		   (tzinfo.DaylightDate.wMonth != 0) &&
		   (tzinfo.DaylightBias != 0) )
		dstflag_cache = DAYLIGHT_TIME;
	      else
		dstflag_cache = STANDARD_TIME;
	    }
	  else
	    {
	      dstflag_cache = UNKNOWN_TIME;
	    }

	  elapsed_minutes_cache = t;
	}

      // current millisecs
      millis_at_system_start = ((nt_time.ft_scalar - EPOCH_BIAS) / 10000i64);
      // subtract ticks
      millis_at_system_start -= GetTickCount();
    }

  current_millis = millis_at_system_start + GetTickCount();

  tp->time = (time_t)(current_millis / 1000i64);
  tp->millitm = current_millis % 1000i64;
  tp->timezone = (short)(_timezone / 60);
  tp->dstflag = (short)dstflag_cache;
}
