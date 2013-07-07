// ceinit.c
//
// Time-stamp: <24/01/02 21:16:42 keuchel@netwave.de>

#include "celib.h"
#include "version.h"

int __xceargc;
char **__xceargv;
char **__xceenviron;

#undef environ
char **environ;
char *_commandline;
char *__progname;

void
xceinit(const wchar_t *commandlinew)
{
  int len;
  char path[MAX_PATH];
  char *p;

  _init_multithread();

  if(commandlinew != NULL)
    {
      len = wcslen(commandlinew) + 1;
      _commandline = malloc(len);

      WideCharToMultiByte(CP_ACP, 0, 
			  commandlinew, -1, 
			  _commandline, len, 
			  NULL, NULL);
    }
  else
    {
      _commandline = xcestrdup("");
    }

  _initenv();

  // needed for globbing!!!
  if((p = xcegetenv("PWD")) != NULL)
    XCESetCurrentDirectoryA(p);
  else
    XCESetCurrentDirectoryA("\\");

  _setargv();

  __progname = __xceargv[0];

  _init_io();

  palm_init_stdio();

  // reset shared stuff...
  xceshared_reset();

  xcesetenv("CELIBVERSION", VERSION, 0);
}

void
xcedllinit()
{
  xceshared_init();
}
