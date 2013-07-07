#ifndef UNISTD_H
#define UNISTD_H 1

#include <celib_defs.h>
#include <sys/types.h>

#ifdef _MT
#include "cethread.h"
#endif

XCE_BEGIN_DECLS

XCE_EXPORT int xcegetopt (int, char * const [], const char *);

#ifdef _MT
#define optarg (_getptd()->_optarg)
#define opterr (_getptd()->_opterr)
#define optind (_getptd()->_optind)
#define optopt (_getptd()->_optopt)
#define optreset (_getptd()->_optreset)
#else
extern	 char *optarg;
extern	 int opterr;
extern	 int optind;
extern	 int optopt;
extern   int optreset;
#endif // _MT

XCE_EXPORT int xcesetuid(uid_t id);
XCE_EXPORT int xceseteuid(uid_t id);
XCE_EXPORT int xcegetuid();
XCE_EXPORT int xcegeteuid();

XCE_EXPORT int xcesetgid(int id);
XCE_EXPORT int xcesetegid(int id);
XCE_EXPORT int xcegetgid();
XCE_EXPORT int xcegetegid();

XCE_END_DECLS

#endif

