// cecommdlg.c
//
// Time-stamp: <10/05/02 09:03:01 keuchel@netwave.de>

#include "celib.h"
#include <commdlg.h>

BOOL XCEAPI
XCEChooseColor(LPVOID lp)
{
  XCEShowMessageA("ChooseColor not supported");
  return FALSE;
}

#define OPEN_FILE 1
#define SAVE_FILE 2

static BOOL
XCEFileNameNameDlg(LPOPENFILENAMEA lpofna, int type)
{
  BOOL status;
  LPOPENFILENAMEW lpofnw = malloc(sizeof(OPENFILENAMEW));
  char *p;
  int len;

  memcpy(lpofnw, lpofna, sizeof(OPENFILENAMEW));

  if(lpofna->lpstrFilter)
    {
      for(p = (char *) lpofna->lpstrFilter; ; p++)
	{
	  if(p[0] == 0 && p[1] == 0)
	    break;
	}

      len = p + 1 - lpofna->lpstrFilter;
      lpofnw->lpstrFilter = XCEMakeUnicode(lpofna->lpstrFilter, len);
    }

  lpofnw->lpstrCustomFilter = NULL;

  if(lpofna->lpstrFile != NULL)
    {
      lpofnw->lpstrFile = malloc(lpofna->nMaxFile * sizeof(wchar_t));
      MultiByteToWideChar(CP_ACP, 0,
			  lpofna->lpstrFile, -1, 
			  lpofnw->lpstrFile, lpofna->nMaxFile);
    }

  if(lpofna->lpstrFileTitle != NULL)
    {
      lpofnw->lpstrFileTitle = malloc(lpofna->nMaxFileTitle * sizeof(wchar_t));
    }

  if(lpofna->lpstrInitialDir != NULL)
    {
      wchar_t *p;
      
      lpofnw->lpstrInitialDir = XCEMakeUnicode(lpofna->lpstrInitialDir, -1);
      
      for(p = lpofnw->lpstrInitialDir; *p; p++)
	{
	  if(*p == '/')
	    *p = '\\';
	}
    }

  if(lpofna->lpstrTitle != NULL)
    {
      lpofnw->lpstrTitle = XCEMakeUnicode(lpofna->lpstrTitle, -1);
    }

  if(lpofna->lpstrDefExt != NULL)
    {
      lpofnw->lpstrDefExt = XCEMakeUnicode(lpofna->lpstrDefExt, -1);
    }

  if(type == OPEN_FILE)
    status = GetOpenFileNameW(lpofnw);
  else
    status = GetSaveFileNameW(lpofnw);

  if(status == TRUE)
    {
      WideCharToMultiByte(CP_ACP, 0, 
			  lpofnw->lpstrFile, -1,
			  lpofna->lpstrFile, lpofna->nMaxFile,
			  NULL, NULL);

      if(lpofna->lpstrFileTitle)
	{
	  WideCharToMultiByte(CP_ACP, 0, 
			      lpofnw->lpstrFileTitle, -1,
			      lpofna->lpstrFileTitle, lpofna->nMaxFileTitle,
			      NULL, NULL);
	}

      lpofna->nFileOffset = lpofnw->nFileOffset;
      lpofna->nFileExtension = lpofnw->nFileExtension;
    }

  free((void *) lpofnw->lpstrFilter);
  free((void *) lpofnw->lpstrFile);
  free((void *) lpofnw->lpstrFileTitle);
  free((void *) lpofnw->lpstrInitialDir);
  free((void *) lpofnw->lpstrTitle);
  free((void *) lpofnw->lpstrDefExt);

  free(lpofnw);

  return status;
}

BOOL XCEAPI 
XCEGetOpenFileNameA(LPOPENFILENAMEA lpofna)
{
  return XCEFileNameNameDlg(lpofna, OPEN_FILE);
}

BOOL XCEAPI 
XCEGetSaveFileNameA(LPOPENFILENAMEA lpofna)
{
  return XCEFileNameNameDlg(lpofna, SAVE_FILE);
}
