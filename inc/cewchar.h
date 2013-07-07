// cewchar.h

#ifndef CEWCHAR_H
#define CEWCHAR_H 1

#include "celib_defs.h"

#define MB_CUR_MAX 1

XCE_BEGIN_DECLS

XCE_EXPORT int xcewctomb(char *mbchar, wchar_t wchar);
XCE_EXPORT int xcembtowc(wchar_t *wchar, const char *mbchar, size_t count);
XCE_EXPORT int xcemblen(const char *mbstr, size_t count);

XCE_END_DECLS

#endif
