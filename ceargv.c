// ceargv.c
//
// Time-stamp: <16/04/02 19:53:31 keuchel@netwave.de>

#if UNDER_CE > 201
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include "glob.h"
#include "errno.h"
#include "xcetrace.h"

#define USE_GLOB

extern XCE_EXPORT int __xceargc;
extern XCE_EXPORT char **__xceargv;

#define GLOBFLAGS (GLOB_NOCHECK | GLOB_NOESCAPE | GLOB_NOMAGIC)
//#define GLOBFLAGS (GLOB_ERR | GLOB_NOCHECK | GLOB_NOESCAPE | GLOB_NOMAGIC)

// GLOB_BRACE seems to have a bug...
// GLOB_TILDE also

static int 
errfunc(const char *name, int err)
{
  XCEShowMessageA("glob: cannot access %s - %s\n", name, xcestrerror(err));
  return -1;
}

// argv size;
static int _size;

static void argv_append1(char *s, int doglob);
static void argv_append2(char *s);

static void
argv_append1(char *arg, int doglob)
{
  int i;
  glob_t gbuf;
  char *s, *d, *p;

  gbuf.gl_pathc = 0;
  gbuf.gl_offs = 0;
  gbuf.gl_pathv = NULL;

  p = xcestrdup(arg);

  for(s = arg, d = p; *s;)
    {
      if(*s == '\\' && s[1] == ' ')
	{
	  s++;
	}
      else
	{
	  *d++ = *s++;
	}
    }
  *d = 0;

  s = p;

  if(doglob && strchr(s, '*'))
    {
      if(xceglob(s, GLOBFLAGS, NULL, &gbuf) == 0)
	{
	  for(i = 0; i < gbuf.gl_pathc; i++)
	    argv_append2(gbuf.gl_pathv[i]);
	  xceglobfree(&gbuf);
	}
      else
	{
	  argv_append2(s);
	}
    }
  else
    {
      argv_append2(s);
    }

  free(p);
}

static void
argv_append2(char *s)
{
  if(__xceargc >= _size - 1)
    {
      // expand argv
      _size *= 2;
      __xceargv = realloc(__xceargv, _size * sizeof(char *));
    }

  __xceargv[__xceargc++] = xcestrdup(s);
  __xceargv[__xceargc] = NULL;
}

// Simple Visual C _setargv replacement, does not handle nested
// quotes.. The original matches short names! So *.cpp matches #xxx.cpp#
// (which is XXXX~1.CPP as short name). This is very bad...

void
_setargv()
{
  int i, res;
  char *p;
  char *s;
  
  _size = 10;

  __xceargc = 0;
  __xceargv = malloc(_size * sizeof(char *));

  p = malloc(MAX_PATH);
  XCEGetModuleFileNameA(NULL, p, MAX_PATH);
  argv_append1(p, 0);

  // argv[0] is part of commandline
  s = p = (char *) XCEGetCommandLineA();

  for(;;)
    {
      if(*p == 0)
	{
	  if(s != p)
	    argv_append1(s, 1);
	  break;
	}
      else if(*p == '"')
	{
	  char *start = ++p;

	  // should do a balanced match!
	  while(*p && *p != '"')
	    p++;
	  *p++ = 0;

	  argv_append1(start, 1);

	  s = p;
	}
      else if(*p == '\'')
	{
	  char *start = ++p;

	  while(*p && *p != '\'')
	    p++;
	  *p++ = 0;

	  argv_append1(start, 0);

	  s = p;
	}
      else if(*p == ' ' || *p == '\t')
	{
	  // simulate shell escape for whitespace - dont start new arg
	  if(p[-1] == '\\')
	    {
	      p++;
	      continue;
	    }

	  if(s != p)
	    {
	      *p++ = 0;
	      argv_append1(s, 1);
	    }

	  while(*p && (*p == '\t' || *p == ' '))
	    p++;

	  s = p;
	}
      else
	{
	  p++;
	}
    }

  // convert program name to unix style name
  for(p = __xceargv[0]; *p; p++)
    if(*p == '\\')
      *p = '/';
}
