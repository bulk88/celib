// ceenv.c
//
// Time-stamp: <03/01/02 22:34:15 keuchel@netwave.de>

#include "celib.h"

static int _envsize = 10;
static int _envcount = 0;

extern int __xcetrace;

void
_appendenv(char *s)
{
  if(_envcount >= _envsize - 1)
    {
      // expand argv
      _envsize *= 2;
      __xceenviron = environ = realloc(environ, _envsize * sizeof(char *));
    }
  // we do not free the glob vector or command-line, so we do not dup
  environ[_envcount++] = s;
  environ[_envcount] = NULL;
}

char *
_findenv(const char *name)
{
  int i;
  char *s, *p;
  int res;

  for(i = 0; i < _envcount; i++)
    {
      s = environ[i];

      if((p = strchr(s, '=')) == NULL)
	return NULL;

      *p = 0;
      res = strcmp(name, s);
      *p = '=';

      if(res == 0)
	return p + 1;
    }
  return NULL;
}

int
_replaceenv(const char *str)
{
  int i;
  char *s, *p;

  for(i = 0; i < _envcount; i++)
    {
      s = environ[i];
      p = strchr(s, '=') + 1;

      if(strncmp(str, s, p - s) == 0)
	{
	  free(environ[i]);
	  environ[i] = (char *) str;

	  return 0;
	}
    }
  return -1;
}

void
_initenv_from_reg()
{
  int res;
  char valname[126];
  char data[1024];
  char buf[1024];
  HKEY hKey;
  DWORD dwValSize = sizeof(valname);
  DWORD dwDataSize = sizeof(data);
  DWORD dwType;
  int idx = 0;
  char *p;

  __xceenviron = environ = malloc(_envsize * sizeof(char *));
  environ[0] = NULL;

  if((res = XCERegOpenKeyExA(HKEY_LOCAL_MACHINE, "Environment", 0,
			     KEY_READ, &hKey)) != 0)
    {

      // don you message box anymore, use defaults...
#if 0
      if(res == ERROR_INVALID_PARAMETER) // 87
	{
	  XCEShowMessageA("Please create the key HKLM\\Environment"
			  "and put the value UNIXROOTDIR in it");
	}
      else
	{
	  XCEShowMessageA("RegOpenKeyEx: Environment: %d", res);
	}
#endif

      _appendenv(xcestrdup("UNIXROOTDIR=\\"));
      _appendenv(xcestrdup("PATH=\\;\\windows"));
      _appendenv(xcestrdup("HOME=\\"));
      _appendenv(xcestrdup("TMP=\\"));

      return;
    }

  while(1)
    {
      dwValSize = sizeof(valname);
      dwDataSize = sizeof(data);

      res = XCERegEnumValueA(hKey, idx++, valname, &dwValSize,
			     NULL, &dwType, data, &dwDataSize);

      if(res != 0)
	{
	  if(res != ERROR_NO_MORE_ITEMS)
	    XCEShowMessageA("RegEnumValue: %d", res);
	  break;
	}

      if(dwType != REG_SZ)
	continue;

#if UNDER_CE <= 201
      strcpy(buf, valname);
      strcat(buf, "=");
      strcat(buf, data);
#else
      sprintf(buf, "%s=%s", valname, data);
#endif

      _appendenv(xcestrdup(buf));
    }

  XCERegCloseKey(hKey);
}

void
_initenv_from_envblock(char *buf)
{
  char *s;

  __xceenviron = environ = malloc(_envsize * sizeof(char *));
  environ[0] = NULL;

  for(s = buf; *s;)
    {
      _appendenv(xcestrdup(s));
      s += strlen(s) + 1;
    }
}

void
_initenv()
{
  char buf[MAX_ENVIRONBLOCKSIZE];
  char *p;

  xceshared_getenvironblock(buf);

  if(buf[0] != 0)
    _initenv_from_envblock(buf);
  else
    _initenv_from_reg();

  xceshared_setenvironblock("");

  if((p = xcegetenv("XCE_TRACE")) != NULL)
    {
      __xcetrace = atoi(p);
    }
  if((p = xcegetenv("XCE_TRACE_FILE")) != NULL && p[0] != 0)
    {
      XCEOpenTraceFile(p);
    }
}

char *
xcegetenv(const char *name)
{
  char *p;

  return _findenv(name);
}

int
xceputenv(const char *s)
{
  char *p = xcestrdup(s);

  if(_replaceenv(p) < 0)
    _appendenv(p);

  return 0;
}

int
xcesetenv(const char *name, const char *val, int flags)
{
  int len;
  char *p;

  len = strlen(name) + strlen(val) + 2;
  p = _alloca(len);
  strcpy(p, name);
  strcat(p, "=");
  strcat(p, val);
  xceputenv(p);

  return 0;
}
