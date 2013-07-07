// wssocket.c
//
// Time-stamp: <23/02/02 13:16:19 keuchel@netwave.de>

#if UNDER_CE <= 201
#include <windows.h>
#endif
#include <winsock.h>
#include "celib.h"
#include "cesocket2.h"
#include "fcntl.h"
#include "ceshared.h"

#define CHECKFD(afd) \
  if(afd < 0 || afd >= MAXFDS || _fdtab[afd].fd == -1) \
    { \
      errno = EBADF; \
      return -1; \
    } \
  if( _fdtab[afd].type != XCE_FILE_TYPE_SOCKET) \
    { \
      errno = EBADF; \
      return -1; \
    }

int
xcesocket(int af, int type, int protocol)
{
  SOCKET s;
  int fd;

  if((s = socket(af, type, protocol)) == INVALID_SOCKET)
    {
      DWORD dwError = WSAGetLastError();
      
      errno = dwError;

      return -1;
    }

  fd = _getnewfd();

  _fdtab[fd].fd = fd;
  _fdtab[fd].type = XCE_FILE_TYPE_SOCKET;
  _fdtab[fd].hFile = (HANDLE) s;
  _fdtab[fd].flags = 0;

  XCETrace("xcesocket(): fd = %d, handle = %d", fd, s);

  return fd;
}

int
xceaccept(int afd, struct sockaddr *addr, int *addrlen)
{
  SOCKET s;
  SOCKET sr;
  int fd;

  CHECKFD(afd);

  s = (SOCKET) _fdtab[afd].hFile;

  if((sr = accept(s, addr, addrlen)) == INVALID_SOCKET)
    {
      return -1;
    }

  fd = _getnewfd();

  _fdtab[fd].fd = fd;
  _fdtab[fd].type = XCE_FILE_TYPE_SOCKET;
  _fdtab[fd].hFile = (HANDLE) sr;

  // is this correct?
  _fdtab[fd].flags = _fdtab[afd].flags;

  XCETrace("xceaccept(): fd = %d, handle = %d", fd, sr);

  return fd;
}

int
xcebind(int afd, const struct sockaddr *addr, int addrlen)
{
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if(bind(s, addr, addrlen) == SOCKET_ERROR)
    {
      return -1;
    }

  return 0;
}

int
xceconnect(int afd, const struct sockaddr *addr, int addrlen)
{
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if(connect(s, addr, addrlen) == SOCKET_ERROR)
    {
      return -1;
    }
  return 0;
}


int
xcegetpeername(int afd, struct sockaddr *addr, int *addrlen)
{
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if(getpeername(s, addr, addrlen) == SOCKET_ERROR)
    {
      return -1;
    }

  return 0;
}

int
xcegetsockname(int afd, struct sockaddr *addr, int *addrlen)
{
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if(getsockname(s, addr, addrlen) == SOCKET_ERROR)
    {
      return -1;
    }
  return 0;
}

int
xcegetsockopt(int afd, int level, int optname, char *optval, int *optlen)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = getsockopt(s, level, optname, optval, optlen)) == SOCKET_ERROR)
    {
      return -1;
    }
  return r;
}

int
xceioctlsocket(int afd, long cmd, u_long *argp)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = ioctlsocket(s, cmd, argp)) == SOCKET_ERROR)
    {
      return -1;
    }
  
  if(cmd == FIONBIO)
    {
      if(*argp == 1)
	_fdtab[afd].flags |= O_NONBLOCK;
      else
	_fdtab[afd].flags &= ~(O_NONBLOCK);
    }

  return r;
}

int
xcelisten(int afd, int backlog)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = listen(s, backlog)) == SOCKET_ERROR)
    {
      return -1;
    }

  return r;
}

int
xcerecv(int afd, char *buf, int len, int flags)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = recv(s, buf, len, flags)) == SOCKET_ERROR)
    {
      return -1;
    }

  return r;
}

int
xcerecvfrom(int afd, char *buf, int len, int flags, 
	    struct sockaddr *from, int *fromlen)
{
  int r;
  SOCKET s;
  int frombufsize = *fromlen;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = recvfrom(s, buf, len, flags, from, fromlen)) == SOCKET_ERROR)
    {
      return -1;
    }

  /* Winsock's recvfrom() only returns a valid 'from' when the socket
   * is connectionless.  Perl expects a valid 'from' for all types
   * of sockets, so go the extra mile.
   */

  if(frombufsize == *fromlen)
    getpeername(s, from, fromlen);

  return r;
}

int
xcesend(int afd, const char *buf, int len, int flags)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = send(s, buf, len, flags)) == SOCKET_ERROR)
    {
      return -1;
    }

  return r;
}

int
xcesendto(int afd, const char *buf, int len, int flags,
	  const struct sockaddr *to, int tolen)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = sendto(s, buf, len, flags, to, tolen)) == SOCKET_ERROR)
    {
      return -1;
    }

  return r;
}

int
xcesetsockopt(int afd, int level, int optname, const char *optval, int optlen)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = setsockopt(s, level, optname, optval, optlen)) == SOCKET_ERROR)
    {
      DWORD dwError = WSAGetLastError();

      if(dwError == WSAENOPROTOOPT)
	errno = EINVAL;

      return -1;
    }

  return r;
}
    
int
xceshutdown(int afd, int how)
{
  int r;
  SOCKET s;

  CHECKFD(afd);
  s = (SOCKET) _fdtab[afd].hFile;

  if((r = shutdown(s, how)) < 0)
    {
      return -1;
    }

  return r;
}

// exfds are currently not used!

int
xceselect(int nfds, fd_set *rdfds, fd_set *wrfds, fd_set *exfds, 
	  const struct timeval* timeout)
{
  int i;
  fd_set r2, w2, x2;
  fd_set rout, wout, xout;
  fd_set *pr2, *pw2, *px2;
  int res = 0;
  int ftype;
  struct timeval t2;
  int terminal_ready = 0;
  int terminal_write = 0;
  int pipe_readable;
  int pipe_writeable;
  int cnt;
  MSG msg;
  int non_sockets_ready;

  pr2 = pw2 = px2 = NULL;

 again:
  pipe_readable = pipe_writeable = 0;
  non_sockets_ready = 0;

#if 1
  if(PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE) != FALSE)
    {
      if(msg.message != WM_KEYDOWN)
	{
	  GetMessageW(&msg, NULL, 0, 0);
	  TranslateMessage(&msg);
	  DispatchMessageW(&msg);
	}
    }
#endif

  if(rdfds) 
    {
      pr2 = &r2;

      FD_ZERO(pr2);

      for(i = 0; i < rdfds->fd_count; i++)
	{
	  int s = rdfds->fd_array[i];

	  ftype = xcegetfiletype(s);

	  if(ISPIPEHANDLE(xceget_osfhandle(s)))
	    {
	      char c;
	      DWORD dwRead, dwAvail, dwMsg;

	      if(XCEPeekNamedPipe(xceget_osfhandle(s), &c, 1, 
				  &dwRead, &dwAvail, &dwMsg) == FALSE)
		{
		  // we must exit the loop...
		  if(GetLastError() == ERROR_BROKEN_PIPE)
		    {
		      pipe_readable = 1;
		      non_sockets_ready++;
		    }
		}
	      else if(dwAvail != 0)
		{
		  pipe_readable = 1;
		  non_sockets_ready++;
		}
	    }
	  else if(ftype == XCE_FILE_TYPE_SOCKET)
	    {
	      //XCETrace("adding sock %d to read set", xceget_osfhandle(s));

	      FD_SET((SOCKET) xceget_osfhandle(s), pr2);
	    }
	  else if(ftype == XCE_FILE_TYPE_CONSOLE)
	    {
	      //XCETrace("adding console %d to read set", s);

	      if(PeekMessageW(&msg, NULL, WM_KEYDOWN, WM_KEYDOWN,
			      PM_NOREMOVE) != FALSE)
		{
		  XCETrace("Terminal read ready");
		  terminal_ready = 1;
		  non_sockets_ready++;
		}
	    }
	  else
	    {
	      XCEShowMessageA("Bad fd in xceselect");
	      xceabort();

	      errno = EBADF;
	      return -1;
	    }
	}
    }

  if(wrfds) 
    {
      pw2 = &w2;

      FD_ZERO(pw2);

      for(i = 0; i < wrfds->fd_count; i++)
	{
	  int s = wrfds->fd_array[i];

	  ftype = xcegetfiletype(s);

	  if(ISPIPEHANDLE(xceget_osfhandle(s)))
	    {
	      pipe_writeable = 1;
	      non_sockets_ready++;
	    }
	  else if(ftype == XCE_FILE_TYPE_SOCKET)
	    {
	      //XCETrace("adding sock %d to write set", xceget_osfhandle(s));
	      FD_SET((SOCKET) xceget_osfhandle(s), pw2);
	    }
	  else if(ftype == XCE_FILE_TYPE_CONSOLE)
	    {
	      //XCETrace("adding console %d to write set", s);
	      terminal_write = 1;
	      non_sockets_ready++;
	    }
	  else
	    {
	      XCEShowMessageA("Bad fd in xceselect");
	      xceabort();

	      errno = EBADF;
	      return -1;
	    }
	}
    }

  if(exfds)
    {
      FD_ZERO(exfds);
    }

  if(timeout == NULL)
    {
      // must have timeout when waiting for keyboard
      t2.tv_sec = 0;
      t2.tv_usec = 100;

      if(non_sockets_ready)
	t2.tv_usec = 0;

      res = select(nfds, pr2, pw2, px2, &t2);

      if(res == 0 && non_sockets_ready == 0)
	{
	  // using sleep is much better for sftp
	  Sleep(10);
	  goto again;
	}
    }
  else if(non_sockets_ready)
    {
      t2.tv_sec = 0;
      t2.tv_usec = 0;

      res = select(nfds, pr2, pw2, px2, &t2);
    }
  else
    {
      XCETrace("WARNING: select() with timeout");
      res = select(nfds, pr2, pw2, px2, timeout);
    }

  if(res == SOCKET_ERROR)
    {
      return -1;
    }

  res = 0;

  if(pr2)
    {
      cnt = rdfds->fd_count;

      FD_ZERO(&rout);

      XCETrace("Checking %d original fds for read", cnt);

      for(i = 0; i < cnt; i++)
	{
	  int s = rdfds->fd_array[i];

	  ftype = xcegetfiletype(s);

	  XCETrace("Checking read fd %d, os handle %x",
		   s, xceget_osfhandle(s));

	  if(ISPIPEHANDLE(xceget_osfhandle(s)))
	    {
	      char c;
	      DWORD dwRead, dwAvail, dwMsg;

	      if(XCEPeekNamedPipe(xceget_osfhandle(s), &c, 1, 
				  &dwRead, &dwAvail, &dwMsg) == FALSE)
		{
		  // reader needs no get aware of this...
		  if(GetLastError() == ERROR_BROKEN_PIPE)
		    FD_SET(s, &rout);
		}
	      else if(dwAvail == 0)
		{
		  XCETrace("pipe %d not readable", s);
		}
	      else
		{
		  XCETrace("pipe %d readable", s);
		  FD_SET(s, &rout);
		}
	    }
	  else if(ftype == XCE_FILE_TYPE_SOCKET)
	    {
	      if(!FD_ISSET(xceget_osfhandle(s), pr2))
		{
		  XCETrace("socket %d not readable", s);
		}
	      else
		{
		  XCETrace("socket %d readable", s);
		  FD_SET(s, &rout);
		}
	    }
	  else if(ftype == XCE_FILE_TYPE_CONSOLE)
	    {
	      if(terminal_ready)
		{
		  XCETrace("console %d readable", s);
		  FD_SET(s, &rout);
		}
	      else
		{
		  XCETrace("console %d not readable", s);
		}
	    }
	  else
	    {
	      XCEShowMessageA("Illegal fd in select");
	      DebugBreak();
	    }
	}

      res += rout.fd_count;
      memcpy(rdfds, &rout, sizeof(fd_set));
    }

  if(pw2)
    {
      cnt = wrfds->fd_count;

      FD_ZERO(&wout);

      XCETrace("Checking %d original fds for write", cnt);

      for(i = 0; i < cnt; i++)
	{
	  int s = wrfds->fd_array[i];

	  XCETrace("Checking write fd %d, os handle %x",
		   s, xceget_osfhandle(s));

	  ftype = xcegetfiletype(s);

	  if(ISPIPEHANDLE(xceget_osfhandle(s)))
	    {
	      XCETrace("pipe %d writeable", s);
	      FD_SET(s, &wout);
	    }
	  else if(ftype == XCE_FILE_TYPE_SOCKET)
	    {
	      if(!FD_ISSET(xceget_osfhandle(s), pw2))
		{
		  XCETrace("socket %d not writeable", s);
		}
	      else
		{
		  FD_SET(s, &wout);
		  XCETrace("socket %d writeable", s);
		}
	    }
	  else if(ftype == XCE_FILE_TYPE_CONSOLE)
	    {
	      XCETrace("console %d writeable", s);
	      FD_SET(s, &wout);
	    }
	  else
	    {
	      XCEShowMessageA("Illegal fd in select");
	      DebugBreak();
	    }
	}

      res += wout.fd_count;
      memcpy(wrfds, &wout, sizeof(fd_set));
    }

  XCETrace("select finally returns %d", res);

  return res;
}

int
xceclosesocket(int fd)
{
  return xceclose(fd);
}
