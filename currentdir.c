// currentdir.c
//
// Time-stamp: <12/02/01 14:39:45 keuchel@keuchelnt>

#include "celib.h"

#ifdef _MT
#include "cethread.h"
#endif

#ifndef _MT
static wchar_t _current_dirw[MAX_PATH] = L"\\";
static wchar_t _current_root_dirw[MAX_PATH] = L"\\";
#endif

#ifdef _MT
#define _CURRENTDIRW _getptd()->_tcurrent_dirw
#define _CURRENTROOTDIRW _getptd()->_tcurrent_root_dirw
#else
#define _CURRENTDIRW _current_dirw
#define _CURRENTROOTDIRW _current_root_dirw
#endif

DWORD XCEAPI
XCEGetCurrentDirectoryA(DWORD dwSize, char *buf)
{
  DWORD dwLen;
  wchar_t wbuf[MAX_PATH];

  dwLen = XCEGetCurrentDirectoryW(sizeof(wbuf), wbuf);
  WideCharToMultiByte(CP_ACP, 0, wbuf, -1, buf, dwSize, NULL, NULL);
  return dwLen;
}

DWORD XCEAPI
XCEGetCurrentDirectoryW(DWORD dwSize, wchar_t *buf)
{
  wcscpy(buf, _CURRENTDIRW);

  return wcslen(buf);
}

BOOL XCEAPI
XCESetCurrentDirectoryA(const char *dir)
{
  wchar_t wdir[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, dir, -1, wdir, COUNTOF(wdir));

  return XCESetCurrentDirectoryW(wdir);
}

BOOL XCEAPI
XCESetCurrentDirectoryW(const wchar_t *wdir)
{
  DWORD dwAttr;
  wchar_t wtmp[MAX_PATH];

  XCEFixPathW(wdir, wtmp);

  if((dwAttr = GetFileAttributesW(wtmp)) == 0xFFFFFFFF)
    {
      errno =_winerror2errno(GetLastError());
      return FALSE;
    }

  if((dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
    {
      errno = ENOTDIR;
      return FALSE;
    }

  wcscpy(_CURRENTDIRW, wtmp);

  return TRUE;
}

DWORD
XCEGetCurrentRootDirectoryA(DWORD dwSize, char *buf)
{
  DWORD dwLen;
  wchar_t wbuf[MAX_PATH];

  dwLen = XCEGetCurrentRootDirectoryW(sizeof(wbuf), wbuf);
  WideCharToMultiByte(CP_ACP, 0, wbuf, -1, buf, dwSize, NULL, NULL);
  return dwLen;
}

DWORD
XCEGetCurrentRootDirectoryW(DWORD dwSize, wchar_t *buf)
{
  wcscpy(buf, _CURRENTROOTDIRW);

  return wcslen(buf);
}

BOOL
XCESetCurrentRootDirectoryA(const char *dir)
{
  wchar_t wdir[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, dir, -1, wdir, COUNTOF(wdir));

  return XCESetCurrentRootDirectoryW(wdir);
}

BOOL
XCESetCurrentRootDirectoryW(const wchar_t *wdir)
{
  DWORD dwAttr;
  wchar_t wtmp[MAX_PATH];

  XCEFixPathW(wdir, wtmp);

  if((dwAttr = GetFileAttributesW(wtmp)) == 0xFFFFFFFF)
    {
      errno =_winerror2errno(GetLastError());
      return FALSE;
    }

  if((dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
    {
      errno = ENOTDIR;
      return FALSE;
    }

  wcscpy(_CURRENTROOTDIRW, wtmp);

  return TRUE;
}
