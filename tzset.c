// tzset.c - adapted from Microsoft C Runtime
//
// Time-stamp: <19/02/01 20:07:11 keuchel@w2k>

#include "celib.h"
#include <ctype.h>
#include <time.h>

/*
 * Number of milliseconds in one day
 */
#define DAY_MILLISEC    (24L * 60L * 60L * 1000L)

/*
 * The macro below is valid for years between 1901 and 2099, which easily
 * includes all years representable by the current implementation of time_t.
 */
#define IS_LEAP_YEAR(year)  ( (year & 3) == 0 )

/*
 * Pointer to a saved copy of the TZ value obtained in the previous call
 * to tzset() set (if any).
 */
static char * lastTZ = NULL;

/*
 * Flag indicating that time zone information came from GetTimeZoneInformation
 * API call.
 */
static int tzapiused;

static TIME_ZONE_INFORMATION tzinfo;

/*
 * Structure used to represent DST transition date/times.
 */
typedef struct {
        int  yr;        /* year of interest */
        int  yd;        /* day of year */
        long ms;        /* milli-seconds in the day */
        } transitiondate;

/*
 * DST start and end structs.
 */
static transitiondate dststart = { -1, 0, 0L };
static transitiondate dstend   = { -1, 0, 0L };

static int __cdecl _isindst_lk(struct tm *);

void 
__tzset(void)
{
  static int first_time = 0;

  if ( !first_time ) {

    if ( !first_time ) {
      _xcetzset();
      first_time++;
    }
  }
}

void 
_xcetzset (void)
{
  char *TZ;
  int defused;
  int negdiff = 0;

  /*
   * Clear the flag indicated whether GetTimeZoneInformation was used.
   */
  tzapiused = 0;

  /*
   * Set year fields of dststart and dstend structures to -1 to ensure
   * they are recomputed as after this
   */
  dststart.yr = dstend.yr = -1;

  /*
   * Fetch the value of the TZ environment variable.
   */
  if ( (TZ = xcegetenv("TZ")) == NULL ) {

    /*
     * There is no TZ environment variable, try to use the time zone
     * information from the system.
     */

    if ( GetTimeZoneInformation( &tzinfo ) != 0xFFFFFFFF ) {
      /*
       * Note that the API was used.
       */
      tzapiused = 1;

      /*
       * Derive _timezone value from Bias and StandardBias fields.
       */
      _timezone = tzinfo.Bias * 60L;

      if ( tzinfo.StandardDate.wMonth != 0 )
	_timezone += (tzinfo.StandardBias * 60L);

      /*
       * Check to see if there is a daylight time bias. Since the
       * StandardBias has been added into _timezone, it must be
       * compensated for in the value computed for _dstbias.
       */
      if ( (tzinfo.DaylightDate.wMonth != 0) &&
	   (tzinfo.DaylightBias != 0) )
	{
	  _daylight = 1;
	  _dstbias = (tzinfo.DaylightBias - tzinfo.StandardBias) *
	    60L;
	}
      else {
	_daylight = 0;

	/*
	 * Set daylight bias to 0 because GetTimeZoneInformation
	 * may return TIME_ZONE_ID_DAYLIGHT even though there is
	 * no DST (in NT 3.51, just turn off the automatic DST
	 * adjust in the control panel)!
	 */
	_dstbias = 0;
      }

      /*
       * Try to grab the name strings for both the time zone and the
       * daylight zone. Note the wide character strings in tzinfo
       * must be converted to multibyte characters strings. The
       * locale codepage, __lc_codepage, is used for this. Note that
       * if setlocale() with LC_ALL or LC_CTYPE has not been called,
       * then __lc_codepage will be 0 (_CLOCALECP), which is CP_ACP
       * (which means use the host's default ANSI codepage).
       */
      if ( (WideCharToMultiByte( CP_ACP,
				 WC_COMPOSITECHECK |
				 WC_SEPCHARS,
				 tzinfo.StandardName,
				 -1,
				 _tzname[0],
				 63,
				 NULL,
				 &defused ) != 0) &&
	   (!defused) )
	_tzname[0][63] = '\0';
      else
	_tzname[0][0] = '\0';

      if ( (WideCharToMultiByte( CP_ACP,
				 WC_COMPOSITECHECK |
				 WC_SEPCHARS,
				 tzinfo.DaylightName,
				 -1,
				 _tzname[1],
				 63,
				 NULL,
				 &defused ) != 0) &&
	   (!defused) )
	_tzname[1][63] = '\0';
      else
	_tzname[1][0] = '\0';

    }

    /*
     * Time zone information is unavailable, just return.
     */
    return;
  }


  if ( (*TZ == '\0') || ((lastTZ != NULL) && (strcmp(TZ, lastTZ) == 0)) )
    {
      /*
       * Either TZ is NULL, pointing to '\0', or is the unchanged
       * from a earlier call (to this function). In any case, there
       * is no work to do, so just return
       */

      return;
    }

  /*
   * Update lastTZ
   */

  free(lastTZ);

  if ((lastTZ = malloc(strlen(TZ)+1)) == NULL)
    {
      return;
    }
  strcpy(lastTZ, TZ);

  /*
   * Process TZ value and update _tzname, _timezone and _daylight.
   */

  strncpy(_tzname[0], TZ, 3);
  _tzname[0][3] = '\0';

  /*
   * time difference is of the form:
   *
   *      [+|-]hh[:mm[:ss]]
   *
   * check minus sign first.
   */
  if ( *(TZ += 3) == '-' ) {
    negdiff++;
    TZ++;
  }

  /*
   * process, then skip over, the hours
   */
  _timezone = atol(TZ) * 3600L;

  while ( (*TZ == '+') || ((*TZ >= '0') && (*TZ <= '9')) ) TZ++;

  /*
   * check if minutes were specified
   */
  if ( *TZ == ':' ) {
    /*
     * process, then skip over, the minutes
     */
    _timezone += atol(++TZ) * 60L;
    while ( (*TZ >= '0') && (*TZ <= '9') ) TZ++;

    /*
     * check if seconds were specified
     */
    if ( *TZ == ':' ) {
      /*
       * process, then skip over, the seconds
       */
      _timezone += atol(++TZ);
      while ( (*TZ >= '0') && (*TZ <= '9') ) TZ++;
    }
  }

  if ( negdiff )
    _timezone = -_timezone;

  /*
   * finally, check for a DST zone suffix
   */
  if ( _daylight = *TZ ) {
    strncpy(_tzname[1], TZ, 3);
    _tzname[1][3] = '\0';
  }
  else
    *_tzname[1] = '\0';

}

/***
*static void cvtdate( trantype, datetype, year, month, week, dayofweek,
*                     date, hour, min, second, millisec ) - convert
*       transition date format
*
*Purpose:
*       Convert the format of a transition date specification to a value of
*       a transitiondate structure.
*
*Entry:
*       int trantype    - 1, if it is the start of DST
*                         0, if is the end of DST (in which case the date is
*                            is a DST date)
*       int datetype    - 1, if a day-in-month format is specified.
*                         0, if an absolute date is specified.
*       int year        - year for which the date is being converted (70 ==
*                         1970)
*       int month       - month (0 == January)
*       int week        - week of month, if datetype == 1 (note that 5== last
*                         week of month),
*                         0, otherwise.
*       int dayofweek   - day of week (0 == Sunday), if datetype == 1.
*                         0, otherwise.
*       int date        - date of month (1 - 31)
*       int hour        - hours (0 - 23)
*       int min         - minutes (0 - 59)
*       int sec         - seconds (0 - 59)
*       int msec        - milliseconds (0 - 999)
*
*Exit:
*       dststart or dstend is filled in with the converted date.
*
*******************************************************************************/

static void __cdecl cvtdate (
        int trantype,
        int datetype,
        int year,
        int month,
        int week,
        int dayofweek,
        int date,
        int hour,
        int min,
        int sec,
        int msec
        )
{
        int yearday;
        int monthdow;

        if ( datetype == 1 ) {

            /*
             * Transition day specified in day-in-month format.
             */

            /*
             * Figure the year-day of the start of the month.
             */
            yearday = 1 + (IS_LEAP_YEAR(year) ? _lpdays[month - 1] :
                      _days[month - 1]);

            /*
             * Figure the day of the week of the start of the month.
             */
            monthdow = (yearday + ((year - 70) * 365) + ((year - 1) >> 2) -
                        _LEAP_YEAR_ADJUST + _BASE_DOW) % 7;

            /*
             * Figure the year-day of the transition date
             */
            if ( monthdow < dayofweek )
                yearday += (dayofweek - monthdow) + (week - 1) * 7;
            else
                yearday += (dayofweek - monthdow) + week * 7;

            /*
             * May have to adjust the calculation above if week == 5 (meaning
             * the last instance of the day in the month). Check if year falls
             * beyond after month and adjust accordingly.
             */
            if ( (week == 5) &&
                 (yearday > (IS_LEAP_YEAR(year) ? _lpdays[month] :
                             _days[month])) )
            {
                yearday -= 7;
            }
        }
        else {
            /*
             * Transition day specified as an absolute day
             */
            yearday = IS_LEAP_YEAR(year) ? _lpdays[month - 1] :
                      _days[month - 1];

            yearday += date;
        }

        if ( trantype == 1 ) {
            /*
             * Converted date was for the start of DST
             */
            dststart.yd = yearday;
            dststart.ms = (long)msec +
                          (1000L * (sec + 60L * (min + 60L * hour)));
            /*
             * Set year field of dststart so that unnecessary calls to
             * cvtdate() may be avoided.
             */
            dststart.yr = year;
        }
        else {
            /*
             * Converted date was for the end of DST
             */
            dstend.yd = yearday;
            dstend.ms = (long)msec +
                              (1000L * (sec + 60L * (min + 60L * hour)));
            /*
             * The converted date is still a DST date. Must convert to a
             * standard (local) date while being careful the millisecond field
             * does not overflow or underflow.
             */
            if ( (dstend.ms += (_dstbias * 1000L)) < 0 ) {
                dstend.ms += DAY_MILLISEC;
                dstend.yd--;
            }
            else if ( dstend.ms >= DAY_MILLISEC ) {
                dstend.ms -= DAY_MILLISEC;
                dstend.yd++;
            }

            /*
             * Set year field of dstend so that unnecessary calls to cvtdate()
             * may be avoided.
             */
            dstend.yr = year;
        }

        return;
}

/***
*int _isindst(tb) - determine if broken-down time falls in DST
*
*Purpose:
*       Determine if the given broken-down time falls within daylight saving
*       time (DST). The DST rules are either obtained from Win32 (tzapiused !=
*       TRUE) or assumed to be USA rules, post 1986.
*
*       If the DST rules are obtained from Win32's GetTimeZoneInformation API,
*       the transition dates to/from DST can be specified in either of two
*       formats. First, a day-in-month format, similar to the way USA rules
*       are specified, can be used. The transition date is given as the n-th
*       occurence of a specified day of the week in a specified month. Second,
*       an absolute date can be specified. The two cases are distinguished by
*       the value of wYear field in the SYSTEMTIME structure (0 denotes a
*       day-in-month format).
*
*       USA rules for DST are that a time is in DST iff it is on or after
*       02:00 on the first Sunday in April, and before 01:00 on the last
*       Sunday in October.
*
*Entry:
*       struct tm *tb - structure holding broken-down time value
*
*Exit:
*       1, if time represented is in DST
*       0, otherwise
*
*******************************************************************************/

int 
_isindst (struct tm *tb)
{
  long ms;

  if ( _daylight == 0 )
    return 0;

  /*
   * Compute (recompute) the transition dates for daylight saving time
   * if necessary.The yr (year) fields of dststart and dstend is
   * compared to the year of interest to determine necessity.
   */
  if ( (tb->tm_year != dststart.yr) || (tb->tm_year != dstend.yr) ) {
    if ( tzapiused ) {
      /*
       * Convert the start of daylight saving time to dststart.
       */
      if ( tzinfo.DaylightDate.wYear == 0 )
	cvtdate( 1,
		 1,             /* day-in-month format */
		 tb->tm_year,
		 tzinfo.DaylightDate.wMonth,
		 tzinfo.DaylightDate.wDay,
		 tzinfo.DaylightDate.wDayOfWeek,
		 0,
		 tzinfo.DaylightDate.wHour,
		 tzinfo.DaylightDate.wMinute,
		 tzinfo.DaylightDate.wSecond,
		 tzinfo.DaylightDate.wMilliseconds );
      else
	cvtdate( 1,
		 0,             /* absolute date */
		 tb->tm_year,
		 tzinfo.DaylightDate.wMonth,
		 0,
		 0,
		 tzinfo.DaylightDate.wDay,
		 tzinfo.DaylightDate.wHour,
		 tzinfo.DaylightDate.wMinute,
		 tzinfo.DaylightDate.wSecond,
		 tzinfo.DaylightDate.wMilliseconds );
      /*
       * Convert start of standard time to dstend.
       */
      if ( tzinfo.StandardDate.wYear == 0 )
	cvtdate( 0,
		 1,             /* day-in-month format */
		 tb->tm_year,
		 tzinfo.StandardDate.wMonth,
		 tzinfo.StandardDate.wDay,
		 tzinfo.StandardDate.wDayOfWeek,
		 0,
		 tzinfo.StandardDate.wHour,
		 tzinfo.StandardDate.wMinute,
		 tzinfo.StandardDate.wSecond,
		 tzinfo.StandardDate.wMilliseconds );
      else
	cvtdate( 0,
		 0,             /* absolute date */
		 tb->tm_year,
		 tzinfo.StandardDate.wMonth,
		 0,
		 0,
		 tzinfo.StandardDate.wDay,
		 tzinfo.StandardDate.wHour,
		 tzinfo.StandardDate.wMinute,
		 tzinfo.StandardDate.wSecond,
		 tzinfo.StandardDate.wMilliseconds );

    }
    else {
      /*
       * GetTimeZoneInformation API was NOT used, or failed. USA
       * daylight saving time rules are assumed.
       */
      cvtdate( 1,
	       1,
	       tb->tm_year,
	       4,                 /* April */
	       1,                 /* first... */
	       0,                 /* ...Sunday */
	       0,
	       2,                 /* 02:00 (2 AM) */
	       0,
	       0,
	       0 );

      cvtdate( 0,
	       1,
	       tb->tm_year,
	       10,                /* October */
	       5,                 /* last... */
	       0,                 /* ...Sunday */
	       0,
	       2,                 /* 02:00 (2 AM) */
	       0,
	       0,
	       0 );
    }
  }

  /*
   * Handle simple cases first.
   */
  if ( dststart.yd < dstend.yd ) {
    /*
     * Northern hemisphere ordering
     */
    if ( (tb->tm_yday < dststart.yd) || (tb->tm_yday > dstend.yd) )
      return 0;
    if ( (tb->tm_yday > dststart.yd) && (tb->tm_yday < dstend.yd) )
      return 1;
  }
  else {
    /*
     * Southern hemisphere ordering
     */
    if ( (tb->tm_yday < dstend.yd) || (tb->tm_yday > dststart.yd) )
      return 1;
    if ( (tb->tm_yday > dstend.yd) && (tb->tm_yday < dststart.yd) )
      return 0;
  }

  ms = 1000L * (tb->tm_sec + 60L * tb->tm_min + 3600L * tb->tm_hour);

  if ( tb->tm_yday == dststart.yd ) {
    if ( ms >= dststart.ms )
      return 1;
    else
      return 0;
  }
  else {
    /*
     * tb->tm_yday == dstend.yd
     */
    if ( ms < dstend.ms )
      return 1;
    else
      return 0;
  }
}
