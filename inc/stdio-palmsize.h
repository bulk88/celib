#ifndef STDIO_PALMSIZE_H
#define STDIO_PALMSIZE_H 1

#include <stdarg.h>
#include "celib_defs.h"
#include "sys/types.h"

XCE_BEGIN_DECLS

#if UNDER_CE <= 201
typedef void FILE;
typedef long fpos_t;

#define EOF (-1)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#endif

struct _iobuf {
  unsigned char *_ptr;
  int   _cnt;
  unsigned char *_base;
  int   _flag;
  int   _file;
  int   _charbuf;
  int   _bufsiz;
  char *_tmpfname;
};

typedef struct _iobuf PFILE;

XCE_EXPORT int palm_putchar(int c);
XCE_EXPORT int palm_getchar(void);
XCE_EXPORT long palm_ftell(FILE *fx);
XCE_EXPORT long palm_fseek(FILE *fx, off_t off, int whence);
XCE_EXPORT int palm_ungetc(int c, FILE *fx);
XCE_EXPORT int palm_fsetmode(FILE *f, int mode);
XCE_EXPORT int palm_fgetmode(FILE *f);
XCE_EXPORT FILE *palm_fdopen(int fd, const char *mode);
XCE_EXPORT FILE *palm_fopen(const char *name, const char *mode);
XCE_EXPORT int palm_fclose(FILE *f);
XCE_EXPORT int palm_printf(const char *fmt, ...);
XCE_EXPORT int palm_fprintf(FILE *f, const char *fmt, ...);
XCE_EXPORT int palm_vprintf(const char *fmt, va_list ap);
XCE_EXPORT int palm_ferror(FILE *f);
XCE_EXPORT int palm_clearerr(FILE *f);
XCE_EXPORT int palm_fflush(FILE *f);
XCE_EXPORT int palm_feof(FILE *f);
XCE_EXPORT int palm_fwrite(const void *buf, int size, int n, FILE *f);
XCE_EXPORT int palm_fread(const void *buf, int size, int n, FILE *f);
XCE_EXPORT char *palm_fgets(char *s, int n, FILE *f);
XCE_EXPORT char *palm_gets(char *s);
XCE_EXPORT int palm_vprintf(const char *fmt, va_list ap);
XCE_EXPORT int palm_vfprintf(FILE *f, const char *fmt, va_list ap);
XCE_EXPORT int palm_getc(FILE *f);
XCE_EXPORT int palm_putc(int c, FILE *f);
XCE_EXPORT int palm_fputc(int c, FILE *f);
XCE_EXPORT int palm_fgetc(FILE *f);
XCE_EXPORT int palm_fputs(const char *buf, FILE *f);
XCE_EXPORT int palm_puts(const char *buf);

XCE_EXPORT int palm_fileno(FILE *f);

XCE_EXPORT int palm_fgetpos(FILE *f, off_t *pos);
XCE_EXPORT int palm_fsetpos(FILE *f, off_t *pos);
XCE_EXPORT int palm_flushall();
XCE_EXPORT int palm_fcloseall();
XCE_EXPORT int palm_setvbuf(FILE *f, char *buffer, int mode, size_t size);

XCE_EXPORT int palm_rewind(FILE *f);
XCE_EXPORT FILE *palm_freopen(const char *path, const char *mode, FILE *f);


#if UNDER_CE <= 201
XCE_EXPORT int palm_sprintf(char *buf, const char *fmt, ...);
XCE_EXPORT int palm_vsprintf(char *buf, const char *fmt, va_list ap);
XCE_EXPORT int palm_sscanf(const char *buf, const char *fmt, ...);
#endif

extern XCE_EXPORT FILE *palm_stdin;
extern XCE_EXPORT FILE *palm_stdout;
extern XCE_EXPORT FILE *palm_stderr;

#undef stdin
#define stdin palm_stdin
#undef stdout
#define stdout palm_stdout
#undef stderr
#define stderr palm_stderr

#undef fopen
#define fopen palm_fopen
#undef _fopen
#define _fopen palm_fopen
#undef fclose
#define fclose palm_fclose
#define printf palm_printf
#define fprintf palm_fprintf
#define gets palm_gets
#define fgets palm_fgets
#undef _fileno
#define _fileno palm_fileno
#undef fileno
#define fileno palm_fileno
#undef fdopen
#define fdopen palm_fdopen
#define feof palm_feof
#define fputc palm_fputc
#define ferror palm_ferror
#define fread palm_fread
#define fwrite palm_fwrite
#define fflush palm_fflush
#define feof palm_feof
#define clearerr palm_clearerr
#define fgetc palm_fgetc
#define putchar palm_putchar
#define getchar palm_getchar
#define ftell palm_ftell
#define fseek palm_fseek
#define vprintf palm_vprintf
#define vfprintf palm_vfprintf
#define vfwprintf palm_vfwprintf
#define fwprintf palm_fwprintf
#define ungetc palm_ungetc
#define puts palm_puts
#define fputs palm_fputs
#undef putc
#define putc palm_putc
#undef getc
#define getc palm_getc

#define fgetpos palm_fgetpos
#define fsetpos palm_fsetpos
#define _flushall palm_flushall
#define _fcloseall palm_fcloseall
#undef flushall
#define flushall palm_flushall
#undef fcloseall
#define fcloseall palm_fcloseall
#define setvbuf palm_setvbuf

#undef rewind
#define rewind palm_rewind

#define freopen palm_freopen

#if UNDER_CE <= 201
#define sprintf   palm_sprintf
#define vsprintf  palm_vsprintf
#define sscanf    palm_sscanf
#endif

XCE_END_DECLS

#endif
