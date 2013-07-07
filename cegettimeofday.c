// cegettimeofday.c
//
// Time-stamp: <23/12/01 12:24:16 keuchel@netwave.de>

#include "celib.h"
#include "time.h"
#include "sys/timeb.h"

struct timeval {
  long    tv_sec; 
  long    tv_usec;
};

struct timezone {
  int	tz_minuteswest;
  int	tz_dsttime;
};

int
xcegettimeofday (struct timeval *tv, struct timezone *tz)
{
  struct _timeb tb;

  xceftime (&tb);

  tv->tv_sec = tb.time;
  tv->tv_usec = tb.millitm * 1000;

#if 0
  // why not?
  if(tz) 
    {
      tz->tz_minuteswest = tb.timezone;
      tz->tz_dsttime = tb.dstflag;
    }
#endif

  return 0;
}

// special version for timers, where seconds is incorrect...

int
xcegettimeofday_timer(struct timeval *tv, struct timezone *tz)
{
  DWORD ticks;

  ticks = GetTickCount();

  tv->tv_sec = ticks / 1000;
  tv->tv_usec = ticks % 1000;

  return 0;
}
