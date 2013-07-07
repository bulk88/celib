// iocntl.h
//
// Time-stamp: <23/09/01 09:04:38 keuchel@netwave.de>

#ifndef SYS_IOCTL_H
#define SYS_IOCTL_H

#include "celib_defs.h"
#include <io.h>

XCE_BEGIN_DECLS

XCE_EXPORT int xceioctl(int fd, int flag, void *arg);

XCE_END_DECLS

#endif
