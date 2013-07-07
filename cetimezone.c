// cetimezone.c
//
// Time-stamp: <18/08/01 10:04:30 keuchel@w2k>

#include "celib.h"

char *
xcegettimezonename(char *buf, int size)
{
  // there is no W-Version...
  TIME_ZONE_INFORMATION tzi;
  char *p;

  if((p = xcegetenv("TZ")) != NULL)
    {
      strcpy(buf, p);
      return buf;
    }

  // tzi.StandardName = "Mitteleurop. Normalzeit"
  // tzi.DaylightName = "(MEZ) - Mitteleurop. Sommerzeit"

  GetTimeZoneInformation(&tzi);
  WideCharToMultiByte(CP_ACP, 0, tzi.DaylightName + 1, 3,
		      buf, size, 0, 0);
  buf[3] = 0;

  return buf;
}
