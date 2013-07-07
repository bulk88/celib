// cetoolhelp.c - toolhelp functions
//
// Time-stamp: <01/01/01 15:59:02 keuchel@lightning>

#include "celib.h"
#include <tlhelp32.h>

typedef struct tagMODULEENTRY32W {
  DWORD   dwSize;
  DWORD   th32ModuleID;
  DWORD   th32ProcessID;
  DWORD   GlblcntUsage;
  DWORD   ProccntUsage;
  BYTE   *modBaseAddr;
  DWORD   modBaseSize;
  HMODULE hModule;
  wchar_t szModule[MAX_PATH];
  wchar_t szExePath[MAX_PATH];
  DWORD   dwFlags;
} MODULEENTRY32W, *PMODULEENTRY32W, *LPMODULEENTRY32W;

typedef struct tagPROCESSENTRY32W { 
  DWORD dwSize; 
  DWORD cntUsage; 
  DWORD th32ProcessID; 
  DWORD th32DefaultHeapID; 
  DWORD th32ModuleID; 
  DWORD cntThreads; 
  DWORD th32ParentProcessID; 
  LONG pcPriClassBase; 
  DWORD dwFlags; 
  wchar_t szExeFile[MAX_PATH]; 
  DWORD th32MemoryBase;
  DWORD th32AccessKey;
} PROCESSENTRY32W, *PPROCESSENTRY32W, *LPPROCESSENTRY32W;

void
XCEEnumModules()
{
  HANDLE hSnap;
  MODULEENTRY32W modent;
  char szModule[MAX_PATH];
  char szExePath[MAX_PATH];
  
  memset(&modent, 0, sizeof(modent));
  modent.dwSize = sizeof(modent);

  if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0)) != (HANDLE) -1)
    {
      if(Module32First(hSnap, (void *) &modent))
	{
	  do {
	    WideCharToMultiByte(CP_ACP, 0, 
				(wchar_t *)modent.szModule, -1, 
				szModule, COUNTOF(szModule), 
				NULL, NULL);

	    WideCharToMultiByte(CP_ACP, 0, 
				(wchar_t *)modent.szExePath, -1, 
				szExePath, COUNTOF(szExePath), 
				NULL, NULL);

	    printf("Module: %s\n", szModule);
	    printf("ExePath: %s\n", szExePath);
	    printf("Base: %p\n", modent.modBaseAddr);

	  } while(Module32Next(hSnap, (void *) &modent));
	}
      else
	{
	  fprintf(stderr, "Module32First: %d\n", GetLastError());
	}
      CloseToolhelp32Snapshot(hSnap);
    }
  else
    {
      fprintf(stderr, "CreateSnapshot: %d\n", GetLastError());
    }
}

void
XCEEnumProcesses()
{
  HANDLE hSnap;
  PROCESSENTRY32W procent;
  char szExeFile[MAX_PATH];
  
  memset(&procent, 0, sizeof(procent));
  procent.dwSize = sizeof(procent);

  if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != (HANDLE) -1)
    {
      if(Process32First(hSnap, (void *) &procent))
	{
	  do {
	    WideCharToMultiByte(CP_ACP, 0, 
				(wchar_t *)procent.szExeFile, -1, 
				szExeFile, COUNTOF(szExeFile), 
				NULL, NULL);

	    printf("ExePath: %s\n", szExeFile);
	    printf("Base: %p\n", procent.th32MemoryBase);
	    // these are huge vals...
	    printf("ProcessID: 0x%x\n", procent.th32ProcessID);

	  } while(Process32Next(hSnap, (void *) &procent));
	}
      else
	{
	  fprintf(stderr, "Process32First: %d\n", GetLastError());
	}
      CloseToolhelp32Snapshot(hSnap);
    }
  else
    {
      fprintf(stderr, "CreateSnapshot: %d\n", GetLastError());
    }
}

DWORD
XCEGetProcessBaseAddress(DWORD dwProcId)
{
  HANDLE hSnap;
  PROCESSENTRY32W procent;
  DWORD dwBaseAddress = 0xFFFFFFFF;

  memset(&procent, 0, sizeof(procent));
  procent.dwSize = sizeof(procent);

  if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != (HANDLE) -1)
    {
      if(Process32First(hSnap, (void *) &procent))
	{
	  do {
	    if(procent.th32ProcessID == dwProcId)
	      {
		dwBaseAddress = procent.th32MemoryBase;
		break;
	      }

	  } while(Process32Next(hSnap, (void *) &procent));
	}
      else
	{
	  fprintf(stderr, "Process32First: %d\n", GetLastError());
	}
      CloseToolhelp32Snapshot(hSnap);
    }
  else
    {
      fprintf(stderr, "CreateSnapshot: %d\n", GetLastError());
    }

  return dwBaseAddress;
}

