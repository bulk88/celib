// cestdio-palmsize.c - very ugly!
//
// Time-stamp: <13/02/02 07:36:11 keuchel@netwave.de>

#include <stdarg.h>
#if UNDER_CE > 201
#include <stdio.h>
#endif
#include "stdio-palmsize.h"
#include "celib.h"
#include "fcntl.h"
#include "xcetrace.h"

extern int __xcetrace;

#define PALM_BUFSIZ     1024

#define PALM_MODEMASK   0x000F /* low bits of iobuf._flags for open mode */
#define PALM_ERRORMASK  0x00F0 /* bits for error status */
#define PALM_EOFMASK    0x0F00 /* bits for EOF status */
#define PALM_BUFMASK    0xF000 /* bits for buffer mode */

#define PALM_ERROR      0x0010
#define PALM_EOF        0x0100

#define PALM_IONBF      0x1000
#define PALM_IOLBF      0x2000
#define PALM_IOFBF      0x3000

#define PALM_SETEOF(F)     (F->_flag |= PALM_EOF)
#define PALM_SETERROR(F)   (F->_flag |= PALM_ERROR)
#define PALM_CLEARERROR(F) (F->_flag &= ~(PALM_ERROR))

FILE *palm_stdin;
FILE *palm_stdout;
FILE *palm_stderr;

PFILE *palm_files[MAXFILES];

int
palm_init_stdio()
{
  if((palm_stdin = palm_fdopen(0, "r")) == NULL)
    {
      XCEShowMessageA("fdopen(%d): %d", 0, errno);
      return -1;
    }
  if((palm_stdout = palm_fdopen(1, "w")) == NULL)
    {
      XCEShowMessageA("fdopen(%d): %d", 1, errno);
      return -1;
    }
  if((palm_stderr = palm_fdopen(2, "w")) == NULL)
    {
      XCEShowMessageA("fdopen(%d): %d", 2, errno);
      return -1;
    }

  palm_setvbuf(palm_stdin, NULL, PALM_IOLBF, PALM_BUFSIZ);
  palm_setvbuf(palm_stdout, NULL, PALM_IOLBF, PALM_BUFSIZ);
  // think LBF is ok...
  //palm_setvbuf(palm_stderr, NULL, PALM_IONBF, PALM_BUFSIZ);
  palm_setvbuf(palm_stderr, NULL, PALM_IOLBF, PALM_BUFSIZ);

  return 0;
}

int 
palm_fsetmode(FILE *fx, int mode)
{
  PFILE *f = (PFILE *) fx;

  /* we store in f->_flag... this is bad! */
  if(mode != O_RDONLY && mode != O_WRONLY && mode != O_RDWR)
    XCEShowMessageA("palm_fsetmode - bad mode %d\n", mode);

  f->_flag &= ~(PALM_MODEMASK);
  f->_flag |= mode;

  return 0;
}

int
palm_fgetmode(FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  int mode = f->_flag & PALM_MODEMASK;
  return mode;
}

FILE *
palm_fdopen(int fd, const char *mode)
{
  PFILE *f;
  int i;
  const char *pmode;

  XCETrace("fdopen(%d, %s)", fd, mode);

  if(fd < 0 || fd >= MAXFDS)
    {
      errno = EBADF;
      return NULL;
    }

  if((f = malloc(sizeof(struct _iobuf))) == NULL)
    {
      errno = ENOMEM;
      return NULL;
    }

  f->_file = fd;
  f->_bufsiz = PALM_BUFSIZ;
  f->_cnt = 0;
  f->_flag = PALM_IOFBF; // default: full buffering

  if(_fdtab[fd].type == XCE_FILE_TYPE_CONSOLE)
    f->_flag = PALM_IONBF;

  pmode = mode;
  if(*pmode == 'r')
    {
      palm_fsetmode(f, O_RDONLY);
    }
  else if(*pmode == 'w')
    {
      palm_fsetmode(f, O_WRONLY);
    }
  else if(*pmode == 'a')
    {
      palm_fsetmode(f, O_WRONLY);
    }
  else
    {
      errno = EINVAL;

      XCEShowMessageA("palm_fdopen: unhandled open mode: %s", mode);

      free(f);
      return NULL;
    }

  while(*++pmode)
    {
      switch(*pmode)
	{
	case 't':
	case 'b':
	  break;
	case '+':
	  palm_fsetmode(f, O_RDWR);
	  break;
	default:
	  errno = EINVAL;
	  XCEShowMessageA("palm_fdopen: unhandled open mode: %s", mode);
	  free(f);
	  return NULL;
	}
    }

  f->_base = f->_ptr = malloc(f->_bufsiz);
  if(f->_base == NULL)
    {
      free(f);
      errno = ENOMEM;
      return NULL;
    }
  f->_charbuf = 0; /* what is this ? */
  f->_tmpfname = NULL;

  // enter file in table, so closeall and flushall work
  for(i = 0; i < MAXFILES; i++)
    {
      if(palm_files[i] == NULL)
	{
	  palm_files[i] = f;
	  break;
	}
    }

  XCETrace("fdopen returns %x", f);

  return (FILE *) f;
}

FILE *
palm_fopen(const char *name, const char *mode)
{
  int fd;
  int flags = 0;

  if(strstr(mode, "a") != 0)
    flags = O_WRONLY|O_CREAT|O_APPEND;
  else if(strstr(mode, "rw") != 0)
    flags = O_RDWR|O_CREAT|O_TRUNC;
  else if(strstr(mode, "r") != 0)
    flags = O_RDONLY;
  else if(strstr(mode, "w") != 0)
    flags = O_WRONLY|O_CREAT|O_TRUNC;

  if((fd = xceopen(name, flags, 0666)) < 0)
    return NULL;

  return palm_fdopen(fd, mode);
}

int
palm_fclose(FILE *fx)
{
  int error = 0;
  PFILE *f = (PFILE *) fx;
  int i;

  if(f == NULL)
    {
      errno = EFAULT;
      return -1;
    }

  /* flush buffers on close */
  if(f->_cnt != 0 && palm_fflush(f) < 0)
    error = EOF;

  xceclose(f->_file);

  // remove from file table
  for(i = 0; i < MAXFILES; i++)
    {
      if(palm_files[i] == f)
	{
	  palm_files[i] = NULL;
	  break;
	}
    }

  free(f->_base);
  free(f);

  return error;
}

int
palm_printf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  if(palm_stdout == NULL)
    {
      XCEShowMessageA("printf: stdio not initialized");
      exit(1);
    }

  return palm_vfprintf(palm_stdout, fmt, ap);
}

int
palm_fprintf(FILE *fx, const char *fmt, ...)
{
  PFILE *f = (PFILE *) fx;

  va_list ap;
  va_start(ap, fmt);

  if(f == NULL)
    {
      XCEShowMessageA("fprintf: file pointer is null");
      DebugBreak();
      exit(1);
    }

  return palm_vfprintf(f, fmt, ap);
}

int 
palm_vprintf(const char *fmt, va_list ap)
{
  if(palm_stdout == NULL)
    {
      XCEShowMessageA("vprintf: stdio not initialized");
      exit(1);
    }

  return palm_vfprintf(palm_stdout, fmt, ap);
}

int
palm_ferror(FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  if(f == NULL)
    {
      errno = EFAULT;
      return -1;
    }

  return (f->_flag & PALM_ERRORMASK);
}

int
palm_clearerr(FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  PALM_CLEARERROR(f);

  return 0;
}

int
palm_fflush(FILE *fx)
{
  int cc;
  int mode;
  PFILE *f = (PFILE *) fx;

  if(__xcetrace & XCE_TRACE_STDIO && f)
    XCETrace("fflush(fd = %d)", f->_file);

  // TBD: must flush all streams when f is NULL!
  if(f == NULL)
    {
      XCETrace("warning: fflush(NULL) called");
      errno = EFAULT;
      return -1;
    }

  mode = palm_fgetmode(f);

  if(mode == O_WRONLY || mode == O_RDWR || mode == O_APPEND)
    {
      if(mode == O_RDWR)
	{
	  if(__xcetrace & XCE_TRACE_STDIO)
	    XCETrace("warning: fflush called for read-write file!");
	}

      cc = xcewrite(f->_file, f->_base, f->_cnt);

      if(cc < 0)
	{
	  PALM_SETERROR(f);
	  PALM_SETEOF(f);
	  return EOF;
	}
      else if(cc != f->_cnt)
	{
	  PALM_SETEOF(f);
	  return EOF;
	}
      f->_cnt = 0;
      f->_ptr = f->_base;
    }
  else if(mode == O_RDONLY)
    {
      f->_cnt = 0;
      f->_ptr = f->_base;
    }
  else
    {
      XCETrace("error: fflush: unhandled mode: %x", mode);
      DebugBreak();
    }

  return 0;
}

int
palm_feof(FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  //XCETRACE2(XCE_TRACE_STDIO, ("feof(%x)", fx));

  if(f->_flag & PALM_EOF)
    {
      //XCETRACE2(XCE_TRACE_STDIO, ("feof(%x): 1", fx));
      return 1;
    }

  return 0;
}

int
palm_fwrite(const char *buf, int size, int count, FILE *fx)
{
  long n, i;
  const unsigned char *p;
  PFILE *f = (PFILE *) fx;

  // optimize a la fread!

  for(p = buf, n = 0; n < count; n++)
    {
      for(i = 0; i < size; i++, p++)
	{
	  if(palm_fputc(*p, f) == EOF)
	    goto done;
	}
    }

 done:
  return n;
}

int
palm_fread(char *buf, int size, int count, FILE *fx)
{
  unsigned char *p;
  int n = 0, i;
  int total;
  int c;
  PFILE *f = (PFILE *) fx;

  // TODO: This should do direct buffer copy if there is 
  // enough in the buffer! If requested size if larger than buffer,
  // a direct read should be done!

  total = size * count;

  if(total <= f->_cnt)
    {
      memcpy(buf, f->_ptr, total);

      f->_ptr += total;
      f->_cnt -= total;

      return count;
    }
  else if(total > f->_bufsiz)
    {
      int nread;

      memcpy(buf, f->_ptr, f->_cnt);

      buf += f->_cnt;
      n += f->_cnt;
      total -= f->_cnt;

      f->_ptr = f->_base;
      f->_cnt = 0;

      // read directly into the user buffer...
      if((nread = xceread(f->_file, buf, total)) < 0)
	{
	  if(n == 0)
	    {
	      PALM_SETERROR(f);
              PALM_SETEOF(f);
	    }

	  return n / size;
	}

      n += nread;

      if(n == 0)
	{
	  PALM_SETEOF(f);
	}
      return n / size;
    }

  // small read...
  for(p = buf, n = 0; n < count; n++)
    {
      for(i = 0; i < size; i++)
	{
	  if((c = palm_getc(f)) == EOF)
	    goto done;
	  *p++ = c;
	}
    }

 done:
  return n;
}

char *
palm_gets(char *s)
{
  int c;
  char *p;
  PFILE *f = palm_stdin;

  p = s;

  while((c = palm_getc(f)) != EOF)
    {
      *p++ = c;
      if(c == '\n')
	break;
    }

  if(c == EOF)
    return NULL;

  *--p = 0;

  return s;
}

char *
palm_fgets(char *s, int n, FILE *fx)
{
  int c;
  char *p;
  PFILE *f = (PFILE *) fx;

  p = s;

  while((c = palm_getc(f)) != EOF && --n > 0)
    {
      *p++ = c;
      if(c == '\n')
	break;
    }
  *p = 0;

  if(c == EOF)
    return NULL;

  return s;
}

int
palm_vfprintf(FILE *fx, const char *fmt, va_list ap)
{
  int size = PALM_BUFSIZ;
  char *buf;
  int count;
  int i;

  PFILE *f = (PFILE *) fx;

  if(f == NULL)
    {
      errno = EFAULT;
      return -1;
    }

  if((buf = malloc(size)) == NULL)
    {
      errno = ENOMEM;
      return -1;
    }

  /* This is very stupid! */
  while((count = _vsnprintf(buf, size, fmt, ap)) < 0)
    {
      size *= 2;

      if((buf = realloc(buf, size)) == NULL)
	{
	  errno = ENOMEM;
	  return -1;
	}
    }

  for(i = 0; i < count; i++)
    {
      if(palm_putc(buf[i], f) == EOF)
	break;
    }

  palm_fflush(f);

  free(buf);
  return i;
}

int
palm_getc(FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  if(f == NULL)
    {
      errno = EFAULT;
      XCETRACE(("getc: NULL FILE pointer\n"));
      return EOF;
    }

  if(f->_cnt == 0)
    {
      f->_cnt = xceread(f->_file, f->_base, f->_bufsiz);

      if(f->_cnt < 0)
	{
	  PALM_SETERROR(f);
	  PALM_SETEOF(f);
          XCETRACE(("getc: xceread failed: errno = %d\n", errno));
	  return EOF;
	}
      else if(f->_cnt == 0)
	{
	  PALM_SETEOF(f);
	  return EOF;
	}
      f->_ptr = f->_base;
    }

  f->_cnt--;
  return *(f->_ptr++);
}

int
palm_putc(int c, FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  if(f == NULL)
    {
      errno = EFAULT;
      return -1;
    }

  if(f->_cnt == f->_bufsiz)
    {
      if(palm_fflush(f) < 0)
	return EOF;
    }

  f->_cnt++;
  *f->_ptr++ = (unsigned char) c;

  if(((f->_flag & PALM_BUFMASK) == PALM_IONBF) ||
     ((f->_flag & PALM_BUFMASK) == PALM_IOLBF && c == '\n'))
    {
      if(palm_fflush(f) < 0)
	return EOF;
    }
  return c;
}

int
palm_fputc(int c, FILE *fx)
{
  return palm_putc(c, fx);
}

int
palm_putchar(int c)
{
  return palm_putc(c, palm_stdout);
}

int
palm_getchar()
{
  return palm_fgetc(palm_stdin);
}

int
palm_fgetc(FILE *fx)
{
  return palm_getc(fx);
}

int
palm_fileno(FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  return f->_file;
}

int
palm_ungetc(int c, FILE *fx)
{
  PFILE *f = (PFILE *) fx;

  // very dangerous!

  if(f->_ptr == f->_base)
    {
      XCEShowMessageA("Error in %s: %d", __FILE__, __LINE__);
      DebugBreak();
    }

  *(--(f->_ptr)) = c;
  f->_cnt++;

  return c;
}

long
palm_ftell(FILE *fx)
{
  PFILE *f = (PFILE *) fx;
  long pos;

  if((pos = xcelseek(f->_file, 0, SEEK_CUR)) < 0)
    return -1;

  pos -= f->_cnt;

  return pos;
}

long
palm_fseek(FILE *fx, off_t off, int whence)
{
  PFILE *f = (PFILE *) fx;
  long pos;

  if(whence == SEEK_CUR)
    off -= f->_cnt;

  if(palm_fflush(fx) < 0)
    return -1;

  if((pos = xcelseek(f->_file, off, whence)) < 0)
    return -1;

  // must return 0 on success
  return 0;
}

int
palm_puts(const char *buf)
{
  palm_fputs(buf, palm_stdout);
  palm_fputc('\n', palm_stdout);
  return 0;
}

int
palm_fputs(const char *buf, FILE *fx)
{
  PFILE *f = (PFILE *) fx;
  const char *p;

  for(p = buf; *p; p++)
    {
      if(palm_fputc(*p, f) == EOF)
	return EOF;
    }
  return 0;
}

FILE *
palm_freopen(const char *path, const char *mode, FILE *f)
{
  PFILE *pf = (PFILE *) f;
  PFILE *fnew;

  XCETrace("freopen(%s, %s)", path, mode);

  if(pf->_file == 0)
    {
      // must turn off console input here...
      if((fnew = palm_fopen(path, mode)) != NULL)
	{
	  palm_fclose(palm_stdin);
	  palm_stdin = (FILE *) fnew;
	  return fnew;
	}
      else
	{
	  return NULL;
	}
    }
  else if(pf->_file == 1)
    {
      if((fnew = palm_fopen(path, mode)) != NULL)
	{
	  palm_fclose(palm_stdout);
	  palm_stdout = (FILE *) fnew;
	  return fnew;
	}
      else
	{
	  return NULL;
	}
    }
  else if(pf->_file == 2)
    {
      if((fnew = palm_fopen(path, mode)) != NULL)
	{
	  palm_fclose(palm_stderr);
	  palm_stderr = (FILE *) fnew;
	  return fnew;
	}
      else
	{
	  return NULL;
	}
    }

  errno = EINVAL;

  return NULL;
}

int
palm_fgetpos(FILE *f, off_t *pos)
{
  off_t off;

  if((off = palm_ftell(f)) < 0)
    return -1;

  *pos = off;
  return 0;
}

int
palm_fsetpos(FILE *f, off_t *pos)
{
  if((palm_fseek(f, *pos, SEEK_SET)) < 0)
    return -1;

  return 0;
}

int
palm_flushall()
{
  int i;

  for(i = 0; i < MAXFILES; i++)
    {
      if(palm_files[i] != NULL)
	{
	  palm_fflush(palm_files[i]);
	}
    }
  return 0;
}

int
palm_fcloseall()
{
  int i;

  for(i = 0; i < MAXFILES; i++)
    {
      if(palm_files[i] != NULL)
	{
	  palm_fclose(palm_files[i]);
	}
    }
  return 0;
}

int 
palm_setvbuf(FILE *f, char *buffer, int mode, size_t size)
{
  PFILE *pf = (PFILE *) f;

  if(mode != PALM_IONBF && mode != PALM_IOLBF && mode != PALM_IOFBF)
    {
      XCETrace("Invalid buffer mode %x", mode);
      DebugBreak();
      return -1;
    }

  free(pf->_base);

  if(buffer == NULL)
    {
      // some progs use a  size of 0 for NBF, we use a buffer anyway,
      // also for non-buffered files...
      if(size == 0)
	size = PALM_BUFSIZ;

      buffer = malloc(size);
    }

  pf->_base = pf->_ptr = buffer;
  pf->_bufsiz = size;
  pf->_cnt = 0;

  pf->_flag &= ~(PALM_BUFMASK);
  pf->_flag |= mode;

  return 0;
}

int 
palm_rewind(FILE *f)
{
  if(palm_fseek(f, 0, SEEK_SET) < 0)
    return -1;

  return 0;
}

#if UNDER_CE <= 201
int
palm_sprintf(char *buf, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  return bsd_vsprintf(buf, fmt, ap);
}

int
palm_vsprintf(char *buf, const char *fmt, va_list ap)
{
  return bsd_vsprintf(buf, fmt, ap);
}

int
palm_sscanf(const char *buf, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  return bsd_vsscanf(buf, fmt, ap);
}

#endif

FILE *
xcefopen(const char *fname, const char *mode)
{
  return palm_fopen(fname, mode);
}

int 
xcefclose(FILE *f)
{
  return palm_fclose(f);
}

FILE *
xcefreopen(const char *path, const char *mode, FILE *f)
{
  return palm_freopen(path, mode, f);
}
