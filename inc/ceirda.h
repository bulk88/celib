// ceirda.h
// 
// Time-stamp: <31/05/01 07:43:05 keuchel@w2k>

#include "celib_defs.h"

XCE_BEGIN_DECLS

// build address, does a device enumeration
XCE_EXPORT int xceirda_getaddr(const char *hname, const char *sname,
			       SOCKADDR_IRDA *ir_addr);

XCE_END_DECLS

