#ifndef CEPROFILE_H
#define CEPROFILE_H 1

#define XCE_MAXPROFILE 20

typedef struct _xceprofentry 
{
  char *name;
  int count;
  __int64 start;
  __int64 stop;
  __int64 total;
} XCEPROFILEENTRY;

extern XCEPROFILEENTRY __proftab[XCE_MAXPROFILE];

#ifdef _PROFILE
#define XCE_PROFILE_START(X) \
  __proftab[X].name = #X; \
  __proftab[X].count++; \
  QueryPerformanceCounter((LARGE_INTEGER *) &__proftab[X].start);

#define XCE_PROFILE_STOP(X) \
  QueryPerformanceCounter((LARGE_INTEGER *) &__proftab[X].stop); \
  __proftab[X].total += (__proftab[X].stop - __proftab[X].start);
#else
#define XCE_PROFILE_START(X) ((void) 0)
#define XCE_PROFILE_STOP(X)  ((void) 0)
#endif

void XCEDumpProfiles();

#endif
