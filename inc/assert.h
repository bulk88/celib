#ifndef ASSERT_H
#define ASSERT_H 1

#include "celib_defs.h"

XCE_BEGIN_DECLS

XCE_EXPORT int xcehandleassert(const char *fmt, ...);

XCE_END_DECLS

#ifdef NDEBUG
#  define _DIAGASSERT(e) ((void)0)
#  define assert(e)      ((void)0)
#else
#  define _DIAGASSERT(e) ((e) || xcehandleassert("Assertion %s failed in file %s: line %d", \
                                                  #e, __FILE__, __LINE__));
#  define assert(e)      ((e) || xcehandleassert("Assertion %s failed in file %s: line %d", \
                                                  #e, __FILE__, __LINE__));
#endif

#endif
