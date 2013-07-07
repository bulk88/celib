// testexports.c - test floats under MIPS/WCE200
//
// Time-stamp: <08/05/01 20:57:48 keuchel@w2k>

#include <windows.h>

// had to test this without cedll on WCE200...

BOOL
testexports()
{
  HANDLE hFile;
  HANDLE hMod;
  FARPROC proc;
  char *buf;
  char *p;
  DWORD size;
  DWORD dwRead;
  char name[126];
  WCHAR wname[126];
  char *s, *d;
  
  //hMod = LoadLibraryW(L"coredll.dll");
  hMod = LoadLibraryW(L"winsock.dll");

  if((hFile = CreateFileW(L"\\bin\\x1.txt", GENERIC_READ, 0, NULL,
			  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL)
    {
      MessageBoxW(NULL, L"Open", L"Open", MB_OK);
      return FALSE;
    }

  size = GetFileSize(hFile, NULL);
  buf = malloc(size + 1);
  ReadFile(hFile, buf, size, &dwRead, NULL);
  CloseHandle(hFile);
  buf[size] = 0;

  for(s = buf, d = name; *s;)
    {
      if(*s == '\n')
	{
	  if(d[-1] == '\r')
	    d[-1] = 0;
	  *d = 0;

	  MultiByteToWideChar(CP_ACP, 0, name, -1, wname, sizeof(wname)/2);
	  proc = GetProcAddressW(hMod, wname);
	  if(proc == NULL)
	    {
	      MessageBoxW(NULL, wname, wname, MB_OK);
	      return FALSE;
	    }
	  d = name;
	  s++;
	}
      else
	{
	  *d++ = *s++;
	}
    }

  return TRUE;
}

int
wWinMain()
{
  testexports();
  return 0;
}
