// ceutil.c
//
// Time-stamp: <13/04/01 20:00:20 keuchel@w2k>

#include "celib.h"

void
XCEShowMessageA(const char *fmt, ...)
{
  va_list ap;
  char buf[512];
  wchar_t wbuf[512];

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, COUNTOF(wbuf));

  MessageBoxW(NULL, wbuf, _T("Message"), MB_OK);
}

void
XCEShowMessageW(const wchar_t *fmt, ...)
{
  va_list ap;
  wchar_t buf[512];

  va_start(ap, fmt);
  vswprintf(buf, fmt, ap);
  va_end(ap);

  MessageBoxW(NULL, buf, _T("Message"), MB_OK);
}

char *
XCEToDosPath(char *s)
{
  char *p;

  for(p = s; *p; p++)
    if(*p == '/')
      *p = '\\';

  return s;
}

char *
XCEToUnixPath(char *s)
{
  char *p;

  for(p = s; *p; p++)
    if(*p == '\\')
      *p = '/';

  return s;
}

int
XCEGetEnvironmentVariableFromRegA(const char *name, char *buf, int len)
{
  int res;
  char data[1024];
  HKEY hKey;
  DWORD dwDataSize = sizeof(data);
  DWORD dwType;

  if(buf)
    buf[0] = 0;

  if((res = XCERegOpenKeyExA(HKEY_LOCAL_MACHINE, "Environment", 0,
			     KEY_READ, &hKey)) != 0)
    {
      return 0;
    }

  res = XCERegQueryValueExA(hKey, name, NULL, &dwType, 
			 data, &dwDataSize);

  XCERegCloseKey(hKey);

  if(res != 0 || dwType != REG_SZ)
    return 0;

  if(buf == NULL)
    return strlen(data);

  strncpy(buf, data, len);

  return strlen(buf);
}

BOOL
XCESetEnvironmentVariableInRegA(const char *name, const char *value)
{
  int res;
  HKEY hKey;
  
  if((res = XCERegOpenKeyExA(HKEY_LOCAL_MACHINE, "Environment", 0,
			     KEY_READ, &hKey)) != 0)
    {
      return FALSE;
    }

  res = XCERegSetValueExA(hKey, name, 0, REG_SZ, 
			  (LPBYTE) value, strlen(value) + 1);

  XCERegCloseKey(hKey);

  return res == 0;
}

//////////////////////////////////////////////////////////////////////

int
XCEGetRegStringA(HKEY hKey, const char *keyname, const char *valname, 
		 char *buf, int len)
{
  int res;
  char data[1024];
  DWORD dwDataSize = sizeof(data);
  DWORD dwType;

  if(buf)
    buf[0] = 0;

  if((res = XCERegOpenKeyExA(hKey, keyname, 0,
			     KEY_READ, &hKey)) != 0)
    {
      return 0;
    }

  res = XCERegQueryValueExA(hKey, valname, NULL, &dwType, 
			 data, &dwDataSize);

  XCERegCloseKey(hKey);

  if(res != 0 || dwType != REG_SZ)
    return 0;

  if(buf == NULL)
    return strlen(data);

  strncpy(buf, data, len);

  return strlen(buf);
}

BOOL
XCESetRegStringA(HKEY hKey, const char *keyname, const char *valname, 
		 const char *value)
{
  int res;
  
  if((res = XCERegOpenKeyExA(hKey, keyname, 0,
			     KEY_READ, &hKey)) != 0)
    {
      return FALSE;
    }

  res = XCERegSetValueExA(hKey, valname, 0, REG_SZ, 
			  (LPBYTE) value, strlen(value) + 1);

  XCERegCloseKey(hKey);

  return res == 0;
}
