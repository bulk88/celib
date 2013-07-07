/* rcmd.c --- rcmd for winsock
 *
 * Time-stamp: <03/01/02 22:21:20 keuchel@netwave.de>
 *
 */

#define WIN32_LEAN_AND_MEAN
#if UNDER_CE > 201
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <errno.h>
#include "celib.h"
#include "cesocket.h"

#define bcopy(from, to, len) memcpy(to, from, len)

#define SENDFLAG 0
#define RECVFLAG 0

#define IPPORT_RESERVED  1024

static int wserrno; 

SOCKET rresvport(int *p);

#ifdef UNDER_CE
#define wserror xcewserror
#endif

SOCKET
xcercmd (char **ahost, unsigned short rport, const char *locuser, 
	 const char *remuser, const char *cmd, SOCKET *fd2p)
{
  SOCKET s, timo = 1;
  struct sockaddr_in sin, from;
  char c;
  int lport = IPPORT_RESERVED - 1;
  struct hostent *hp;
  int cc;

  hp = gethostbyname (*ahost);
  if (hp == 0)
    {
      wserror("%s", *ahost);
      return (INVALID_SOCKET);
    }
  *ahost = hp->h_name;
  for (;;)
    {
      s = rresvport (&lport);
      if (s == INVALID_SOCKET)
	{
	  if (wserrno == EAGAIN)
	    fprintf (stderr, "All ports in use\n");
	  else
	    wserror ("socket");
	  return (INVALID_SOCKET);
	}
      sin.sin_family = hp->h_addrtype;
      bcopy (hp->h_addr_list[0], &sin.sin_addr, hp->h_length);
      sin.sin_port = rport;

      /* when connected, exit loop */

      if (connect (s, (struct sockaddr *) &sin, sizeof (sin)) >= 0)
	{
	  //printf("Connected\n");
	  break;
	}
      else
	{
	  //printf("NotConnected: %d\n", WSAGetLastError());
	}

      wserrno = WSAGetLastError ();
      closesocket (s);  

      if (wserrno == WSAEADDRINUSE)
	{
	  lport--;
	  continue;
	}
      if (wserrno == WSAECONNREFUSED && timo <= 16)
	{
	  Sleep (timo * 1000);
	  timo *= 2;
	  continue;
	}
      if (hp->h_addr_list[1] != NULL)
	{
	  fprintf (stderr, "connect to address %s: ",
		   inet_ntoa (sin.sin_addr));
	  hp->h_addr_list++;
	  bcopy (hp->h_addr_list[0], &sin.sin_addr,
		 hp->h_length);
	  fprintf (stderr, "Trying %s...\n", inet_ntoa (sin.sin_addr));
	  continue;
	}

      WSASetLastError(wserrno);

      return (INVALID_SOCKET);
    }

  /* we are connected */

  lport--;
  if (fd2p == 0)
    {
      send (s, "", 1, SENDFLAG);
      lport = 0;
    }
  else
    {
      char num[8];
      SOCKET s2 = rresvport (&lport), s3;
      int len = sizeof (from);

      if (s2 == INVALID_SOCKET)
	goto bad;
      listen (s2, 1);
      sprintf (num, "%d", lport);
      if (send (s, num, strlen (num) + 1, SENDFLAG) != strlen (num) + 1)
	{
	  wserror ("rcmd:write: setting up stderr");
	  closesocket (s2);
	  goto bad;
	}
      s3 = accept (s2, (struct sockaddr *) &from, &len);
      closesocket (s2);
      if (s3 == INVALID_SOCKET)
	{
	  wserror ("rcmd:accept failed");
	  lport = 0;
	  goto bad;
	}
      *fd2p = s3;
      from.sin_port = ntohs ((u_short) from.sin_port);
      if (from.sin_family != AF_INET ||
	  from.sin_port >= IPPORT_RESERVED)
	{
	  fprintf (stderr,
		   "socket: protocol failure in circuit setup.\n");
	  goto bad2;
	}
    }

  /* these are null terminated */
  if((cc = send (s, locuser, strlen (locuser) + 1, SENDFLAG))
     == SOCKET_ERROR || cc == 0)
    wserror("rcmd: send");

  send (s, remuser, strlen (remuser) + 1, SENDFLAG);
  send (s, cmd, strlen (cmd) + 1, SENDFLAG);

  if ((cc = recv (s, &c, 1, RECVFLAG)) != 1)
    {
      if(cc == 0)
	fprintf(stderr, "Connection closed by foreign host\n");
      else
	wserror ("rcmd: recv");
      goto bad2;
    }
  /* get one line from server */
  if (c != 0)
    {
      while (recv (s, &c, 1, RECVFLAG) == 1)
	{
#ifdef UNDER_CE
	  fprintf(stderr, "%c", c);
#else
	  write (2, &c, 1);
#endif
	  if (c == '\n')
	    break;
	}
      goto bad2;
    }
  /* return socket to caller */
  return (s);

bad2:
  if (lport)
    closesocket (*fd2p);
bad:
  closesocket (s);
  return (INVALID_SOCKET);
}

/* the undocumented windows version of this function returns more than
   an int in alport! */

static SOCKET
rresvport (int *alport)
{
  struct sockaddr_in sin;
  SOCKET s;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  s = socket (AF_INET, SOCK_STREAM, 0);
  if (s == INVALID_SOCKET)
    {
      wserrno = WSAGetLastError ();
      return (INVALID_SOCKET);
    }
  for (;;)
    {
      sin.sin_port = htons ((u_short) * alport);
      if (bind (s, (struct sockaddr *) &sin, sizeof (sin)) == 0)
	return (s);
      wserrno = WSAGetLastError ();
      if (wserrno != WSAEADDRINUSE)
	{
	  closesocket (s);
	  return (INVALID_SOCKET);
	}
      (*alport)--;
      if (*alport == IPPORT_RESERVED / 2)
	{
	  closesocket (s);
	  wserrno = EAGAIN;
	  return (INVALID_SOCKET);
	}
    }
}

