// logwtmp.c
//
// Time-stamp: <03/01/02 22:21:00 keuchel@netwave.de>

#if UNDER_CE > 201
#include <stdio.h>
#endif
#include <time.h>
#include "celib.h"

int
xcelogwtmp(const char *ttyline, const char *user, const char *remotehost)
{
  FILE *f;
  time_t ti;

  ti = xcetime(NULL);

  if((f = xcefopen(XCEGetUnixPath("/var/log/wtmp.log"), "a")) == NULL)
    return -1;

  if(strcmp(user, "") == 0 && strcmp(remotehost, "") == 0)
    fprintf(f, "%.24s: logout on line %s\n", xcectime(&ti), ttyline);
  else
    fprintf(f, "%.24s: login from %s@%s on line %s\n",
	    xcectime(&ti), 
	    (user) ? user : "?", 
	    (remotehost) ? remotehost : "?", 
	    (ttyline) ? ttyline : "?");
  xcefclose(f);
  return 0;
}
