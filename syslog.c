/* syslog.c */

/* Time-stamp: <03/01/02 22:22:07 keuchel@netwave.de> */

#if UNDER_CE > 201
#include <stdio.h>
#endif
#include <stdarg.h>
#include <time.h>
#include <errno.h>

#define WIN32_LEAN_AND_MEAN
#if UNDER_CE <= 201
#include <windows.h>
#endif
#include <winsock.h>
#include <syslog.h>
#ifndef UNDER_CE
#include <paths.h>
#endif
#include <fcntl.h>

#include "celib.h"

char *__LogTag;
int __LogStat;
int __LogOpened = 0;

static int LogFacility;
static int LogSocket;
static struct sockaddr_in LogAddr;

int
xceconnectlog()
{
#ifndef UNDER_CE
  struct servent *sp;

  wsstart();
  
  if((sp = getservbyname("syslog", "udp")) == NULL)
    return -1;

  LogAddr.sin_family = AF_INET;
  LogAddr.sin_port = htons(sp->s_port);
  LogAddr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
  LogSocket = socket(PF_INET, SOCK_DGRAM, 0);
  __LogOpened = 1;
#endif

  return 0;
}

void
xceopenlog(const char *ident, int logstat, int logfac)
{
  static char modname[126];

  XCEGetModuleFileNameA(NULL, modname, sizeof(modname));

  if(ident == NULL)
    __LogTag = modname;
  else
    __LogTag = (char *)ident;

  __LogStat = logstat;
  LogFacility = logfac;
#ifndef UNDER_CE
  connectlog();
#endif
}

void
xcecloselog()
{
#ifndef UNDER_CE
  closesocket(LogSocket);
#endif
  LogSocket = INVALID_SOCKET;
}

void
xcesyslog(int pri, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  xcevsyslog(pri, fmt, ap);
  va_end(ap);
}

void
xcevsyslog(int pri, const char *fmt, va_list ap)
{
  FILE *f;
  char *s;
  time_t now;
  char buf[1024];
  char newfmt[1024];
  int count = 0;
  char *p, *d;
  int fd;
  int send_error = 0;
  int saveerrno = errno;
  int savewinerror = GetLastError();
  int savewinsockerror = WSAGetLastError();

  switch(LOG_PRI(pri))
    {
    case LOG_DEBUG:
      s = "DEBUG:";
      break;
    case LOG_INFO:
      s = "INFO:";
      break;
    case LOG_ERR:
      s = "ERROR:";
      break;
    case LOG_NOTICE:
      s = "NOTICE:";
      break;
    case LOG_WARNING:
      s = "WARNING:";
      break;
    default:
      s = "UNKNOWN:";
      break;
    }

  for(p = (char *)fmt, d = newfmt; *p;)
    {
      if(*p == '%')
	{
	  if(p[1] == 'm')
	    {
	      if(saveerrno)
		d += sprintf(d, "%s", xcestrerror(saveerrno));
	      else if(savewinerror)
		d += sprintf(d, "%s", xcewinstrerror(savewinerror));
	      else if(savewinsockerror)
		d += sprintf(d, "%s", xcewsstrerror(savewinsockerror));
	      p += 2;
	    }
	  else if(p[1] == 'w')
	    {
	      if(savewinerror)
		d += sprintf(d, "%s", xcewinstrerror(savewinerror));
	      else if(savewinsockerror)
		d += sprintf(d, "%s", xcewsstrerror(savewinsockerror));
	      p += 2;
	    }
	  else
	    {
	      *d++ = *p++;
	    }
	}
      else
	{
	  *d++ = *p++;
	}
    }

  *d = 0;

  xcetime(&now);
  count = sprintf(buf, "<%d>%.15s %s", pri, xcectime(&now) + 4, __LogTag, s);
  if(__LogStat & LOG_PID)
    count += sprintf(buf + count, "[%x]", xcegetpid());
  count += sprintf(buf + count, ":");
  count += vsprintf(buf + count, newfmt, ap);

  if(buf[count - 1] != '\n')
    count += sprintf(buf + count, "\n");

  /* output to stderr */
  if(__LogStat & LOG_PERROR)
    fprintf(stderr, "%s: %s", s, buf);

  if(!__LogOpened)
    xceopenlog(__LogTag, __LogStat | LOG_NDELAY, 0);

#ifndef UNDER_CE
  if(sendto(LogSocket, buf, strlen(buf), 0, (struct sockaddr *)& LogAddr,
	    sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
      send_error = WSAGetLastError();
    }

  if((__LogStat & LOG_CONS) && (fd = open(_PATH_CONSOLE, O_WRONLY, 0)) >= 0)
    {
      _write(fd, buf, strlen(buf));
      _close(fd);
    }
#endif

  if((fd = xceopen("/Speicherkarte2/var/log/syslog.log",
		 O_WRONLY|O_APPEND|O_CREAT,666)) > 0)
    {
      xcewrite(fd, s, strlen(s));
      xcewrite(fd, buf + 3, strlen(buf) - 3);
      xceclose(fd);
    }
}

#ifdef TEST
void
main()
{
  xceopenlog("TEST", 0, 0);
  xcesyslog(LOG_ERR, "Test failed: %m");
}

#endif
