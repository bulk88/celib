#ifndef CETHREAD_H
#define CETHREAD_H 1

#include <celib_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _tiddata {
  unsigned long _tid; 
  unsigned long _thandle;
  // per thread errno
  int _terrno;
  // currentdir is per thread, so you can write a multithreaded ftpd.
  // this differs from normal Win32, where currentdir is per process.
  unsigned short _tcurrent_dirw[256];
  unsigned short _tcurrent_root_dirw[256];
  // getopt
  char *_optarg;
  int _optind;
  int _opterr;
  int _optopt;
  int _optreset;
  // other stuff to be inserted (strerror, asctime, ...)
  void * _initaddr; 
  void * _initarg;
  void * _curwin; // for console
};

typedef struct _tiddata * _ptiddata;

// get per thread data
XCE_EXPORT _ptiddata _getptd(void); 

XCE_EXPORT unsigned long _xcebeginthread(void (* initialcode) (void *), unsigned stacksize, void * argument);
XCE_EXPORT void _xceendthread (void);

XCE_EXPORT unsigned long _xcebeginthreadex (void *security, unsigned stacksize, unsigned (*initialcode) (void *), void * argument, unsigned createflag, unsigned *thrdaddr);
XCE_EXPORT void _xceendthreadex (unsigned retcode);

#ifdef __cplusplus
};
#endif

#endif
