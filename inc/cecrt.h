#ifndef CECRT_H
#define CECRT_H 1

#include <windows.h>
#include <stdlib.h>
#include "celib_defs.h"
#include "sys/types.h"

#if UNDER_CE <= 201
typedef void FILE;
#endif

#ifndef PERL
struct tms {
  unsigned long tms_utime;
  unsigned long tms_cutime;
  unsigned long tms_stime;
};
#endif

XCE_BEGIN_DECLS

typedef int (* CONSOLE_READ_FUNC)(int, unsigned char *, int);
typedef int (* CONSOLE_WRITE_FUNC)(int, unsigned char *, int);
typedef int (* CONSOLE_IOCTL_FUNC)(int, int, void *);

extern XCE_EXPORT int __xceargc;
extern XCE_EXPORT char **__xceargv;
extern XCE_EXPORT char **__xceenviron;

extern XCE_EXPORT char **environ;

extern char *_commandline;

#if UNDER_CE > 201
extern int _fmode;
#endif

extern int __umask;

XCE_EXPORT void xceinit(const wchar_t *commandlinew);

XCE_EXPORT int xceinitconsole(CONSOLE_WRITE_FUNC wf, 
			      CONSOLE_READ_FUNC rf);

XCE_EXPORT int xceinitconsole2(HWND hwnd, 
			       CONSOLE_WRITE_FUNC wf, 
			       CONSOLE_READ_FUNC rf,
			       CONSOLE_IOCTL_FUNC cf);

XCE_EXPORT int xceprinterror(const char *fmt, ...);

XCE_EXPORT int xceisatty(int fd);
XCE_EXPORT int xcesnprintf(char *buf, size_t count, const char *fmt, ...);

XCE_EXPORT int xceftruncate(int fd, unsigned long size);
XCE_EXPORT int xcechsize(int fd, unsigned long size);

XCE_EXPORT HANDLE xceget_osfhandle(int fd);
XCE_EXPORT FILE *xcefopen(const char *path, const char *mode);
XCE_EXPORT FILE *xcefreopen(const char *path, const char *mode, FILE *stream);
XCE_EXPORT int xcefclose();
XCE_EXPORT FILE *xcefdopen(int fd, const char *mode);
XCE_EXPORT int xcefileno(FILE *f);
XCE_EXPORT FILE *xcepopen(const char *command, const char *mode);
XCE_EXPORT int xcepclose(FILE *f);
XCE_EXPORT int xcegetpid();
XCE_EXPORT char *xcestrrchr(const char * string, int ch);
XCE_EXPORT int xcecreat(const char *filename, int pmode);
XCE_EXPORT int xceopen(const char *fname, int mode, ...);
XCE_EXPORT int xceopenw(const wchar_t *fname, int mode, ...);
XCE_EXPORT int xceread(int fd, void *buf, int size);
XCE_EXPORT int xcewrite(int fd, void *buf, int size);
XCE_EXPORT int xceclose(int fd);
XCE_EXPORT off_t xcelseek(int fd, int off, int whence);
XCE_EXPORT int xcestat(const char *fname, struct xcestat *st);
XCE_EXPORT int xcefstat(int fd, struct xcestat *st);
XCE_EXPORT char *xcegetenv(const char *name);
XCE_EXPORT int xceputenv(const char *s);
XCE_EXPORT void xceabort();
XCE_EXPORT int xcechdir(const char *dirname);
XCE_EXPORT int xcechroot(const char *dirname);
XCE_EXPORT char *xcegetcwd(char *buf, int size);
XCE_EXPORT int xcesystem(const char *cmd);
XCE_EXPORT void xcerewind(FILE *f);
XCE_EXPORT int xcelocking(int fd, int mode, int nbytes);
XCE_EXPORT char *xcestrerror(int n);
XCE_EXPORT void xceperror(const char *s);
XCE_EXPORT int xceaccess(const char *fname, int mode);
XCE_EXPORT int xceunlink(const char *fname);
XCE_EXPORT int xceraise(int);
XCE_EXPORT char *xcestrdup(const char * string);
XCE_EXPORT char *xcestrndup(const char * string, int len);
XCE_EXPORT int xcestricmp(const char * dst, const char * src);
XCE_EXPORT int xcestrcasecmp(const char * dst, const char * src);
XCE_EXPORT int xcestrcmpi(const char * dst, const char * src);
XCE_EXPORT int xcestrnicmp(const char * first, const char * last, size_t count);
XCE_EXPORT char *xcestrupr(char *string);
XCE_EXPORT char *xcestrlwr(char *string);
XCE_EXPORT size_t xcestrspn(const char *string, const char *control);
XCE_EXPORT size_t xcestrcspn(const char *string, const char *control);
XCE_EXPORT char *xcestrsep(char **stringp, const char *delim);

XCE_EXPORT int xceumask(int mask);
XCE_EXPORT int xcemkdir(const char *dirname);
XCE_EXPORT int xcermdir(const char *dirname);
XCE_EXPORT int xcerename(const char *old, const char *newname);
XCE_EXPORT int xcechmod(const char *fname, int mode);
XCE_EXPORT char *xcemktemp(char *templ);
XCE_EXPORT void xceexit(int);
XCE_EXPORT int xcestrncasecmp (const char * first, const char * last, size_t count);

XCE_EXPORT struct servent *xcegetservbyname(const char *sname, const char *sproto);
XCE_EXPORT struct servent * xcegetservbyport(int aport, const char *sproto);
XCE_EXPORT struct protoent *xcegetprotobyname(const char *name);
XCE_EXPORT struct protoent *xcegetprotobynumber(int number);

XCE_EXPORT double xcestrtod(const char *s, char **errorptr);
XCE_EXPORT long xcestrtol(const char *s, char **errorptr, int base);
XCE_EXPORT unsigned long xcestrtoul(const char *s, char **errorptr, int base);

XCE_EXPORT void *xcecalloc(size_t num, size_t size);

XCE_EXPORT int xcealarm(int secs);

XCE_EXPORT int xceascii2addr(int af, const char *ascii, void *result);
XCE_EXPORT int xceinet_aton(const char *cp, struct in_addr *addr);
XCE_EXPORT char *xcelocalhost();
XCE_EXPORT int xcelogwtmp(const char *ttyline, const char *user, const char *remotehost);

XCE_EXPORT void *xcebcopy(void *from, void *to, size_t size);
XCE_EXPORT int xcebcmp(const void *a1, const void *a2, size_t size);
XCE_EXPORT void *xcebzero(void *block, size_t size);
XCE_EXPORT char *xceindex(const char *s, int c);
XCE_EXPORT char *xcerindex(const char *s, int c);

XCE_EXPORT int xcechown(const char *filename, int owner, int group);

XCE_EXPORT unsigned int xcesleep(unsigned int secs);

XCE_EXPORT char *xcestrpbrk(const char *s1, const char *s2);
XCE_EXPORT size_t xcestrspn(const char *s1, const char *s2);

XCE_EXPORT char *xcewsstrerror(int code);
XCE_EXPORT char *xcewinstrerror(int code);
XCE_EXPORT char *xcegetlogin();

// also in time.h
XCE_EXPORT int xceutime (const char *fname, struct _utimbuf *times);
XCE_EXPORT int xcefutime (int fh, struct _utimbuf *times);

XCE_EXPORT int xceremove(const char *path);

XCE_EXPORT double xceatof(const char *s);

#if UNDER_CE <= 200
//XCE_EXPORT int xcetolower(int);
//XCE_EXPORT int xcetoupper(int);
#endif

XCE_EXPORT int xcestrcoll(const char *string1, const char *string2);

XCE_EXPORT int xceexecv(const char *cmdname, const char *const *argv);
XCE_EXPORT int xceexecvp(const char *cmdname, const char *const *argv);
XCE_EXPORT int xceexecl(const char *cmdname, ...);
XCE_EXPORT int xceexeclp(const char *cmdname, ...);

XCE_EXPORT void *xcebsearch(const void *key, const void *base, 
			    size_t num, size_t width, 
			    int (*compare ) ( const void *e1, const void *e2));

XCE_EXPORT char *xcetmpnam(char *s);

// unsupported
XCE_EXPORT int xcestrftime(char *s, int size, const char *templ, 
			   const struct tm *brokentime);

XCE_EXPORT int xcedup(int fd);
XCE_EXPORT int xcedup2(int fd1, int fd2);

XCE_EXPORT int xceopen_osfilehandle(HANDLE hFile, int oflag);
XCE_EXPORT int xcesetfiletype(int fd, int type);
XCE_EXPORT int xcegetfiletype(int fd);
XCE_EXPORT int xcepipe(int fds[2]);

XCE_EXPORT int xcesetenv(const char *name, const char *val, int flags);
// utility function
XCE_EXPORT char *xcegettimezonename(char *buf, int size);

XCE_EXPORT long xceclock();
XCE_EXPORT long xcetimes(struct tms *buf);

XCE_END_DECLS

#endif
