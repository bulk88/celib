// asctime.c - adapted from Microsoft C Runtime
//
// Time-stamp: <12/02/01 15:58:09 keuchel@keuchelnt>

#include "celib.h"
#include <time.h>

#define _ASCBUFSIZE   26

static char *store_dt(char *, int);

static char * 
store_dt (char *p, int val)
{
  *p++ = '0' + val / 10;
  *p++ = '0' + val % 10;

  return p;
}

char *
xceasctime (const struct tm *tb)
{
  static char buf[_ASCBUFSIZE];

  char *p = buf;

  int day, mon;
  int i;

  p = buf;

  day = tb->tm_wday * 3;
  mon = tb->tm_mon * 3; 

  for (i=0; i < 3; i++,p++) {
    *p = *(__dnames + day + i);
    *(p+4) = *(__mnames + mon + i);
  }

  *p = ' ';

  p += 4;

  *p++ = ' ';
  p = store_dt(p, tb->tm_mday);
  *p++ = ' ';
  p = store_dt(p, tb->tm_hour);
  *p++ = ':';
  p = store_dt(p, tb->tm_min); 
  *p++ = ':';
  p = store_dt(p, tb->tm_sec); 
  *p++ = ' ';
  p = store_dt(p, 19 + (tb->tm_year/100));
  p = store_dt(p, tb->tm_year%100);
  *p++ = '\n';
  *p = '\0';

  return buf;
}
