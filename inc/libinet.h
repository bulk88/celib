#ifndef LIBINET_H
#define LIBINET_H 1

#include <winsock.h>
#include <stdio.h>
#include "celib_defs.h"

XCE_BEGIN_DECLS

XCE_EXPORT int wsstart();
XCE_EXPORT void wsstop();
XCE_EXPORT void wsperror(char *);

XCE_EXPORT SOCKET xcerexec(char **host, unsigned short port, 
			   char *user, char *pass,
			   char *cmd, SOCKET *fd2p);

XCE_EXPORT SOCKET xcercmd(char **ahost, unsigned short rport, 
			  const char *locuser, 
			  const char *remuser, const char *cmd, SOCKET *fd2p);

XCE_END_DECLS

#endif
