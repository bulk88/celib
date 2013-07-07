#ifndef SYS_TIMEB_H
#define SYS_TIMEB_H 1

struct _timeb {
  time_t time;
  unsigned short millitm;
  short timezone;
  short dstflag;
};

struct timeb {
  time_t time;
  unsigned short millitm;
  short timezone;
  short dstflag;
};

#endif
