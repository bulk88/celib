#ifndef CESOCKET2_H
#define CESOCKET2_H 1

#include "celib_defs.h"
#include "winsock.h"

XCE_BEGIN_DECLS

XCE_EXPORT int xcesocket(int af, int type, int protocol);
XCE_EXPORT int xceaccept(int afd, struct sockaddr *addr, int *addrlen);
XCE_EXPORT int xcebind(int afd, const struct sockaddr *addr, int addrlen);
XCE_EXPORT int xceconnect(int afd, const struct sockaddr *addr, int addrlen);
XCE_EXPORT int xcegetpeername(int afd, struct sockaddr *addr, int *addrlen);
XCE_EXPORT int xcegetsockname(int afd, struct sockaddr *addr, int *addrlen);
XCE_EXPORT int xcegetsockopt(int afd, int level, int optname, 
			     char *optval, int *optlen);
XCE_EXPORT int xceioctlsocket(int afd, long cmd, unsigned long *argp);
XCE_EXPORT int xcelisten(int afd, int backlog);
XCE_EXPORT int xcerecv(int afd, char *buf, int len, int flags);
XCE_EXPORT int xcerecvfrom(int afd, char *buf, int len, int flags, 
		struct sockaddr *from, int *fromlen);
XCE_EXPORT int xcesend(int afd, const char *buf, int len, int flags);
XCE_EXPORT int xcesendto(int afd, const char *buf, int len, int flags,
	      const struct sockaddr *to, int tolen);
XCE_EXPORT int xcesetsockopt(int afd, int level, int optname, 
			     const char *optval, 
			     int optlen);
XCE_EXPORT int xceshutdown(int afd, int how);

XCE_EXPORT int xceselect(int nfds, fd_set *rdfds, fd_set *wrfds, 
			 fd_set *exfds, const struct timeval* timeout);

XCE_EXPORT int xceclosesocket(int fd);

XCE_END_DECLS

#endif
