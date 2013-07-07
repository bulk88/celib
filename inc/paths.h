#ifndef PATHS_H
#define PATHS_H

#include "celib_defs.h"

XCE_EXPORT char *XCEGetUnixPath(const char *);

#define _PATH_BSHELL       XCEGetUnixPath("/bin/sh.exe")
#define _PATH_FTPUSERS	   XCEGetUnixPath("/etc/ftpusers")
#define _PATH_FTPWELCOME   XCEGetUnixPath("/etc/ftpwelcome")
#define _PATH_NOLOGIN	   XCEGetUnixPath("/etc/nologin")
#define _PATH_HEQUIV	   XCEGetUnixPath("/etc/hosts.equiv")
#define _PATH_FTPLOGINMESG XCEGetUnixPath("/etc/motd")
#define _PATH_LOGDIR       XCEGetUnixPath("/var/log")
#define _PATH_TMP          XCEGetUnixPath("/tmp")

#endif

