#ifndef LOCALE_H
#define LOCALE_H 1

#include "celib_defs.h"

#define LC_ALL          0
#define LC_COLLATE      1
#define LC_CTYPE        2
#define LC_MONETARY     3
#define LC_NUMERIC      4
#define LC_TIME         5

#define LC_MIN          LC_ALL
#define LC_MAX          LC_TIME

XCE_BEGIN_DECLS

XCE_EXPORT char *xcesetlocale(int, const char *);

XCE_END_DECLS

#endif
