// xcetrace.h - traceflags
//
// Time-stamp: <12/08/01 10:30:10 keuchel@w2k>

#ifndef XCETRACE_H
#define XCETRACE_H 1

#define XCE_TRACE_READWRITE 0x0008
#define XCE_TRACE_STDIO     0x0010
#define XCE_TRACE_SIGNALS   0x0020
#define XCE_TRACE_PIPES     0x0040
#define XCE_TRACE_CONSOLE   0x0080

extern int __xcetrace;


#define XCETRACE2(L,X) if(__xcetrace & L) XCETrace X

#endif
