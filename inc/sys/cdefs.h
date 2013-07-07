/* cdefs.h - BSD defs */

#ifndef SYS_CDEFS_H
#define SYS_CDEFS_H

#include <stdarg.h>

#define IEEEFP 1 /* IEEE Floating point */

#define __P(X) X

#define __BEGIN_DECLS
#define __END_DECLS

#define __dead
#define __dead2
#define __pure
#define __const const
#define __aconst const
#define __attribute__(X)

#define __restrict

typedef long __off_t;
#ifndef _SIZE_T_DEFINED_
typedef unsigned int size_t;
#define _SIZE_T_DEFINED_
#endif

#define _BSD_VA_LIST_   va_list

// limits
#define LLONG_MIN	(-9223372036854775807i64 - 1)
#define LLONG_MAX	  9223372036854775807i64
#define ULLONG_MAX	  0xffffffffffffffffui64

#endif

