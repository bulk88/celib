// makeunixpath.c
//
// Time-stamp: <18/02/01 14:02:30 keuchel@w2k>

#include "celib.h"

static char _unixdir[MAX_PATH] = "";

char *
XCEGetUnixPath(const char *path)
{
  static char newpath[MAX_PATH];

  if(_unixdir[0] == 0)
    {
      if(XCEGetEnvironmentVariableFromRegA("UNIXROOTDIR", 
					   _unixdir, sizeof(_unixdir)) <= 0)
	{
	  XCEShowMessageA("Warning: UNIXROOTDIR not set");
	  strcpy(_unixdir, "/");
	}
    }

  // we expect that path is absolute...
  if(strcmp(_unixdir, "/") == 0 || strcmp(_unixdir, "\\") == 0)
    strcpy(newpath, "");
  else
    strcpy(newpath, _unixdir);

  strcat(newpath, path);

  return newpath;
}

