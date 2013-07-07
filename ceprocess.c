// ceprocess.h
//
// Time-stamp: <08/09/01 10:43:16 keuchel@netwave.de>

#include "celib.h"
#include "sys/stat.h"

// NOTE: This function cannot be used as the real SearchPath!!!
int XCEAPI
XCESearchPathA(const char *pathlist, const char *file, char *retpath)
{
  char path[MAX_PATH];
  char *ps, *pe;
  char *pl;
  struct xcestat st;
  char *pend;
  char szCurrentDir[MAX_PATH];

  XCEGetCurrentDirectoryA(sizeof(szCurrentDir), szCurrentDir);

  if(pathlist == NULL)
    return -1;

  ps = pe = pl = malloc(strlen(pathlist) + strlen(szCurrentDir) + 2);
  sprintf(ps, "%s;%s", szCurrentDir, pathlist);

  pend = pl + strlen(pl);

  while(ps < pend)
    {
      while(*pe && *pe != ';')
	pe++;
      *pe++ = 0;

      if(strcmp(ps, "/") == 0 ||
	 strcmp(ps, "\\") == 0)
	{
	  sprintf(path, "%s%s", ps, file);
	}
      else
	{
	  sprintf(path, "%s\\%s", ps, file);
	}

      //printf("SearchPath: %s\n", path);

      if(xcestat(path, &st) == 0)
	{
	  strcpy(retpath, path);
	  free(pl);
	  return 0;
	}
      ps = pe;
    }

  free(pl);
  return -1;
}

int
XCEExecuteProcessA(const char *commandline, BOOL bWait, LPDWORD lpdwProcId)
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  char szAppName[126];
  char *szCommandLine = NULL;
  char *p;
  DWORD dwExitCode = 0;
  char path[MAX_PATH];
  char *d;
  BOOL bRes;
  int commandlen;

  XCETrace("XCEExecuteProcessA(%s)", commandline);

  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  if(commandline[0] == '"')
    {
      for(p = (char *)commandline + 1, d = szAppName; *p && *p != '"';)
	{
	  *d++ = *p++;
	}
      p++;
    }
  else
    {
      for(p = (char *)commandline, d = szAppName; *p && *p != ' ';)
	{
	  *d++ = *p++;
	}
    }

  *d++ = 0;

  while(*p == ' ')
    p++;

  commandlen = strlen(p);
  szCommandLine = malloc(commandlen+1);
  strcpy(szCommandLine, p);

  for(p = szAppName; *p; p++)
    {
      if(*p == '/')
	*p = '\\';
    }

  if(szAppName[0] != '\\')
    {
      if(strstr(szAppName, ".exe") == NULL)
	strcat(szAppName, ".exe");

      if(XCESearchPathA(xcegetenv("PATH"), szAppName, path) == 0)
	strcpy(szAppName, path);

      // fix path, in case in contains forward slashes...
      for(p = szAppName; *p; p++)
	{
	  if(*p == '/')
	    *p = '\\';
	}
    }

  bRes = XCECreateProcessA(szAppName, szCommandLine, NULL, NULL,
			   FALSE, 0, NULL, NULL, &si, &pi);

  free(szCommandLine);

  if(bRes == FALSE)
    {
      return -1;
    }

  if(bWait)
    {
      WaitForSingleObject(pi.hProcess, INFINITE);
      GetExitCodeProcess(pi.hProcess, &dwExitCode);
    }
  else
    {
      if(lpdwProcId)
	*lpdwProcId = pi.dwProcessId;
    }

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  return dwExitCode;
}

int
XCEWaitProcess(DWORD dwProcId)
{
  HANDLE hProcess;
  DWORD dwExitCode;

  if((hProcess = OpenProcess(0, FALSE, dwProcId)) == NULL)
    return -1;

  WaitForSingleObject(hProcess, INFINITE);
  GetExitCodeProcess(hProcess, &dwExitCode);

  CloseHandle(hProcess);

  return dwExitCode;
}

DWORD XCEAPI
XCESearchPath2A(LPCSTR  lpszPath,	// address of search path 
		LPCSTR  lpszFile,	// address of filename 
		LPCSTR  lpszExtension, // address of extension 
		DWORD   cchReturnBuffer, // size, in characters, of buffer 
		LPSTR   lpszReturnBuffer, // address of buffer found filename 
		LPSTR   *plpszFilePart // address of pointer file component 
		)
{
  char fname[256];
  int res;
  char *p;

  if(lpszPath == NULL)
    lpszPath = xcegetenv("PATH");

  strcpy(fname, lpszFile);

  if(lpszExtension)
    strcat(fname, lpszExtension);

  if(fname[0] == '\\' || fname[0] == '/')
    {
      if(XCEGetFileAttributesA(fname) == (DWORD) -1)
	return 0;

      strcpy(lpszReturnBuffer, fname);

      if((p = xcestrrchr(lpszReturnBuffer, '\\')) != NULL)
	*plpszFilePart = ++p;
      return strlen(lpszReturnBuffer);
    }

  if((res = XCESearchPathA(lpszPath, fname, lpszReturnBuffer)) == 0)
    {
      if((p = xcestrrchr(lpszReturnBuffer, '\\')) != NULL)
	*plpszFilePart = ++p;
      return strlen(lpszReturnBuffer);
    }

  return 0;
}
