// wsdb.c - missing netdb stuff for windows ce
//
// Time-stamp: <09/08/01 21:47:18 keuchel@w2k>

#include <windows.h>
#include <winsock.h>
#include "celib.h"

#ifdef PALM_SIZE
#define xcefopen palm_fopen
#define xcefclose palm_fclose
#endif

struct servent *
xcegetservbyname(const char *sname, const char *sproto)
{
  static struct servent serv;
  static char name[64];
  static char proto[64];

  FILE *f;
  char path[MAX_PATH];
  int port;
  char buf[512];

  if(sproto == NULL)
    sproto = "tcp";

  strcpy(path, XCEGetUnixPath("/etc/services"));

  if((f = xcefopen(path, "r")) == NULL)
    {
      XCEShowMessageA("Cannot open %s", path);
      return NULL;
    }

  while(fgets(buf, sizeof(buf), f))
    {
      if(buf[0] == '#' || buf[0] == '\n')
	continue;

      if(sscanf(buf, "%s %d/%s", name, &port, proto) == 3)
	{
	  if(strcmp(sname, name) == 0 && strcmp(sproto, proto) == 0)
	    {
	      serv.s_name = name;
	      serv.s_aliases = NULL;
	      serv.s_port = htons((unsigned short) port);
	      serv.s_proto = proto;

	      xcefclose(f);
	      return &serv;
	    }
	}
    }

  XCEShowMessageA("Cannot find service %s", sname);
  errno = ENOENT;

  return NULL;
}

struct servent *
xcegetservbyport(int aport, const char *sproto)
{
  static struct servent serv;
  static char name[64];
  static char proto[64];

  FILE *f;
  char path[MAX_PATH];
  int port;
  char buf[512];

  strcpy(path, XCEGetUnixPath("/etc/services"));

  if((f = xcefopen(path, "r")) == NULL)
    {
      XCEShowMessageA("Cannot open %s", path);
      return NULL;
    }

  while(fgets(buf, sizeof(buf), f))
    {
      if(buf[0] == '#' || buf[0] == '\n')
	continue;

      if(sscanf(buf, "%s %d/%s", name, &port, proto) == 3)
	{
	  if(aport == port && strcmp(sproto, proto) == 0)
	    {
	      serv.s_name = name;
	      serv.s_aliases = NULL;
	      serv.s_port = htons((unsigned short) port);
	      serv.s_proto = proto;

	      xcefclose(f);
	      return &serv;
	    }
	}
    }

  XCEShowMessageA("Cannot find service for port %d", aport);
  errno = ENOENT;

  return NULL;
}

struct protoent *
xcegetprotobyname(const char *name)
{
  static struct protoent pent;

  memset(&pent, 0, sizeof(pent));

  if(strcmp(name, "tcp"))
    {
      pent.p_name = "tcp";
      pent.p_proto = PF_UNSPEC;
    }
  else if(strcmp(name, "udp"))
    {
      pent.p_name = "udp";
      pent.p_proto = PF_UNSPEC;
    }
  else
    {
      errno = ENOENT;
      return NULL;
    }

  return &pent;
}

struct protoent *
xcegetprotobynumber(int number)
{
  static struct protoent pent;

  memset(&pent, 0, sizeof(pent));

  if(number == IPPROTO_TCP)
    {
      pent.p_name = "tcp";
      pent.p_proto = PF_UNSPEC;
    }
  else if(number == IPPROTO_UDP)
    {
      pent.p_name = "udp";
      pent.p_proto = PF_UNSPEC;
    }
  else
    {
      errno = ENOENT;
      return NULL;
    }

  return &pent;
}
 
