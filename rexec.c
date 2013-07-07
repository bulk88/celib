/* rexec.c --- rexec for winsock
 *
 * Time-stamp: <22/02/01 07:23:38 keuchel@w2k>
 *
 */

#include <windows.h>
#include <winsock.h>
#include <stdarg.h>
#include "celib.h"
#include "cesocket.h"

#define bcopy(from, to, len) memcpy(to, from, len)

#define SENDFLAG 0
#define RECVFLAG 0

#define IPPORT_RESERVED  1024

#define WSAEAGAIN 23

static int wserrno;

#define wserror(X)

SOCKET rresvport (int *alport);

SOCKET
xcerexec(char **ahost, unsigned short rport, char *user, 
	 char *pass, char *cmd, SOCKET *fd2p)
{
  int wserrno; 
  SOCKET s, timo = 1;
  struct sockaddr_in sin, from;
  char c;
  int lport = IPPORT_RESERVED - 1;
  struct hostent *hp = NULL;
  char username[16+1] = "";
  char password[16+1] = "";
  struct in_addr ina;

  if(user == NULL || pass == NULL)
    {
      return SOCKET_ERROR;
    }

  // first check numeric address...
  if(xceascii2addr(AF_INET, *ahost, &ina) < 0)
    {
      hp = gethostbyname (*ahost);

      if (hp == NULL)
        {
          wserror(("rexec:gethostbyname failed"));
          return (INVALID_SOCKET);
        }

      *ahost = hp->h_name;
    }
  else
    {
    }

  for (;;)
    {
      s = rresvport(&lport);
      if (s == INVALID_SOCKET)
	{
	  if (WSAGetLastError() == WSAEAGAIN)
	    wserror (("rexec:socket:all ports in use"));
	  else
	    wserror (("rexec:socket failed"));
	  return (INVALID_SOCKET);
	}

      if(hp)
        {
          sin.sin_family = hp->h_addrtype;
          bcopy (hp->h_addr_list[0], &sin.sin_addr, hp->h_length);
        }
      else
        {
          sin.sin_family = AF_INET;
          bcopy(&ina, &sin.sin_addr, 4);
        }

      sin.sin_port = rport;

      /* when connected, exit loop */

      if (connect(s, (struct sockaddr *) &sin, sizeof (sin)) >= 0)
	break;

      wserrno = WSAGetLastError();

      closesocket (s);  

      if (wserrno == WSAEADDRINUSE)
	{
	  lport--;
	  continue;
	}

      if (wserrno == WSAECONNREFUSED && timo <= 16)
	{
	  Sleep(timo * 1000);
	  timo *= 2;
	  continue;
	}

      if (hp && hp->h_addr_list[1] != NULL)
	{
	  //wserror(("connect to address %s: "), inet_ntoa (sin.sin_addr));
	  hp->h_addr_list++;
	  bcopy (hp->h_addr_list[0], &sin.sin_addr, hp->h_length);
	  //wserror(("trying %s..."), inet_ntoa (sin.sin_addr));
	  continue;
	}
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
      wchar_t wnum[8];

      SOCKET s2 = rresvport (&lport), s3;
      int len = sizeof (from);

      if (s2 == INVALID_SOCKET)
	goto bad;
      listen (s2, 1);

      sprintf(num, ("%d"), lport);

      if (send (s, num, strlen (num) + 1, SENDFLAG) != (int) strlen (num) + 1)
	{
	  wserror (("rexec:write: setting up stderr"));
	  closesocket (s2);
	  goto bad;
	}
      s3 = accept (s2, (struct sockaddr *) &from, &len);
      closesocket (s2);
      if (s3 == INVALID_SOCKET)
	{
	  wserror (("rexec:accept failed"));
	  lport = 0;
	  goto bad;
	}
      *fd2p = s3;
      from.sin_port = ntohs ((u_short) from.sin_port);
      if (from.sin_family != AF_INET 
#if 0
          // SCO does not obeye this...
          || from.sin_port >= IPPORT_RESERVED
#endif
          )
	{
	  wserror(("socket: protocol failure in circuit setup."));
	  goto bad2;
	}
    }
  /* these are null terminated */
  send (s, user, strlen (user) + 1, SENDFLAG);
  send (s, pass, strlen (pass) + 1, SENDFLAG);
  send (s, cmd, strlen (cmd) + 1, SENDFLAG);
  if (recv (s, &c, 1, RECVFLAG) != 1)
    {
      wserror (("rexec:cannot read from socket"));
      goto bad2;
    }
  /* get one line from server: "Permission denied" */
  if (c != 0)
    {
      wchar_t buf[126];
      int i = 0;

      while (recv (s, &c, 1, RECVFLAG) == 1)
	{
          buf[i++] = c;
	  if (c == '\n')
	    break;
	}
      buf[i] = 0;

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
	  wserrno = WSAEAGAIN;
	  return (INVALID_SOCKET);
	}
    }
}
