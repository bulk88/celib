#ifndef PROCESS_H
#define PROCESS_H 1

#include "celib_defs.h"

#define _P_WAIT         0
#define _P_NOWAIT       1
#define _P_DETACH       4

#define P_WAIT          _P_WAIT
#define P_NOWAIT        _P_NOWAIT
#define P_DETACH        _P_DETACH

XCE_BEGIN_DECLS

int _cwait(int *, int, int);

int _execl(const char *, const char *, ...);
int _execle(const char *, const char *, ...);
int _execlp(const char *, const char *, ...);
int _execlpe(const char *, const char *, ...);
int _execv(const char *, const char * const *);
int _execve(const char *, const char * const *, const char * const *);
int _execvp(const char *, const char * const *);
int _execvpe(const char *, const char * const *, const char * const *);

int _spawnl(int, const char *, const char *, ...);
int _spawnle(int, const char *, const char *, ...);
int _spawnlp(int, const char *, const char *, ...);
int _spawnlpe(int, const char *, const char *, ...);
int _spawnv(int, const char *, const char * const *);
int _spawnve(int, const char *, const char * const *,
	     const char * const *);
int _spawnvp(int, const char *, const char * const *);
int _spawnvpe(int, const char *, const char * const *,
	      const char * const *);
int _spawn(int, const char *);

// int system(const char *);

XCE_END_DECLS

#endif
