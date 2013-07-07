// cesocket.h - functions using real sockets
//
// Time-stamp: <19/02/01 21:51:55 keuchel@w2k>

#ifndef CESOCKET_H
#define CESOCKET_H 1

#include <winsock.h>
#include "celib_defs.h"

XCE_BEGIN_DECLS

XCE_EXPORT int xcewsstart();
XCE_EXPORT void xcewsstop();
XCE_EXPORT void xcewserror(char *fmt, ...);

XCE_EXPORT SOCKET xcercmd (char **ahost, unsigned short rport, 
			   const char *locuser, 
			   const char *remuser, const char *cmd, SOCKET *fd2p);

XCE_EXPORT SOCKET xcerexec(char **ahost, unsigned short rport, char *user, 
			   char *pass, char *cmd, SOCKET *fd2p);

XCE_END_DECLS

#endif
