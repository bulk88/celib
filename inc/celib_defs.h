#ifndef CELIB_DEFS_H
#define CELIB_DEFS_H 1

#ifdef _DLL
#ifdef BUILD_CELIB
#define XCE_EXPORT __declspec(dllexport)
#else
#define XCE_EXPORT __declspec(dllimport)
#endif
#else
#define XCE_EXPORT
#endif

#define XCEAPI WINAPI

#ifdef __cplusplus
#define XCE_BEGIN_DECLS extern "C" {
#define XCE_END_DECLS };
#else
#define XCE_BEGIN_DECLS
#define XCE_END_DECLS
#endif

#define __XCEP(A) A

#endif
