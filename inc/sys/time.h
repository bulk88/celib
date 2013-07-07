#ifndef SYS_TIME_H
#define SYS_TIME_H 1

#include <time.h>

#ifndef _TIMEVAL_DEFINED_
#ifndef _TIMEVAL_DEFINED
struct timeval {
  long    tv_sec; 
  long    tv_usec;
};
#define _TIMEVAL_DEFINED_
#define _TIMEVAL_DEFINED
#endif
#endif

#endif
