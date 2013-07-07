// cewin32.c
//
// Time-stamp: <28/04/02 15:29:48 keuchel@netwave.de>

#include "celib.h"
#include "ceshared.h"

/* write back & discard all cached data */
#define CACHE_SYNC_DISCARD      0x001 
/* discard all cached instructions */
#define CACHE_SYNC_INSTRUCTIONS 0x002   
/* write back but don't discard data cache*/
#define CACHE_SYNC_WRITEBACK	0x004

DWORD CacheSync(DWORD);

#ifndef _WIN32_WCE_EMULATION
#define SYNC CacheSync(CACHE_SYNC_DISCARD|CACHE_SYNC_INSTRUCTIONS)
#else
#define SYNC
#endif

#if UNDER_CE <= 201
//#define tolower(_c)	( (_c)-'A'+'a' )
//#define toupper(_c)	( (_c)-'a'+'A' )
#endif

UINT _errormode;

extern int __xcetrace;

BOOL XCEAPI
XCEGetFileInformationByHandle(HANDLE hFile, 
			      LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
  return GetFileInformationByHandle(hFile, lpFileInformation);
}

BOOL XCEAPI
XCEGetUserNameA(char *ubuf, LPDWORD lpdwSize)
{
  char buf[126];

  if(XCEGetEnvironmentVariableFromRegA("USERNAME", buf, sizeof(buf)) == 0)
    return FALSE;

  if(ubuf != NULL)
    {
      strncpy(ubuf, buf, *lpdwSize);
    }

  *lpdwSize = strlen(buf);

  return TRUE;
}

BOOL XCEAPI
XCEGetComputerNameA(char *ubuf, LPDWORD lpdwSize)
{
  char buf[126];

  if(XCEGetEnvironmentVariableFromRegA("COMPUTERNAME", buf, sizeof(buf)) == 0)
    return FALSE;

  if(ubuf != NULL)
    {
      strncpy(ubuf, buf, *lpdwSize);
    }

  *lpdwSize = strlen(buf);

  return TRUE;
}

// What program uses this?

BOOL XCEAPI
XCEGetCharABCWidthsA(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC)
{
  int i, n;
  SIZE s;
  WCHAR buf[1];

  for(i = nFirst, n = 0; i <= nLast; i++, n++)
    {
      buf[0] = i;
      lpABC[n].abcA = 0;
      GetTextExtentPoint32W(hdc, buf, 1, &s);
      lpABC[n].abcB = s.cx;
      lpABC[n].abcC = 0;
    }

  return TRUE;
}

BOOL XCEAPI
XCEGetCharABCWidthsW(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC)
{
  int i, n;
  SIZE s;
  WCHAR buf[1];

  for(i = nFirst, n = 0; i <= nLast; i++, n++)
    {
      buf[0] = i;
      lpABC[n].abcA = 0;
      GetTextExtentPoint32W(hdc, buf, 1, &s);
      lpABC[n].abcB = s.cx;
      lpABC[n].abcC = 0;
    }

  return TRUE;
}

// For WCE200 progs, GetVersionEx must be loaded dynamically

#ifndef _WIN32_WCE_EMULATION
BOOL XCEAPI
XCEGetVersionExA(LPOSVERSIONINFOA lpv)
{
  OSVERSIONINFOW vw;
  FARPROC proc = NULL;
  HANDLE hMod;

  vw.dwOSVersionInfoSize = sizeof(vw);

#if 0  
#if UNDER_CE < 201
  GetVersionEx(&vw);
#else
  GetVersionExW(&vw);
#endif
#endif

  hMod = LoadLibraryW(L"coredll.dll");

#if UNDER_CE <= 201
  // this is also supported by WCE211!
  proc = GetProcAddressW(hMod, L"GetVersionEx");
#endif
  if(proc == NULL)
    proc = GetProcAddressW(hMod, L"GetVersionExW");

  if(proc == NULL)
    {
      XCEShowMessageA("Cannot find GetVersionEx in coredll");
      return FALSE;
    }

  (*proc)(&vw);

  FreeLibrary(hMod);

  memcpy(lpv, &vw, 5 * sizeof(DWORD));

  WideCharToMultiByte(CP_ACP, 0, 
		      vw.szCSDVersion, -1, 
		      lpv->szCSDVersion, sizeof(lpv->szCSDVersion), 
		      NULL, NULL);

  return TRUE;
}
#else // Emulation
BOOL XCEAPI
XCEGetVersionExA(LPOSVERSIONINFOA lpv)
{
  OSVERSIONINFOW vw;
  FARPROC proc = NULL;
  HANDLE hMod;

  // emulation cannot load coredll.dll!

  vw.dwOSVersionInfoSize = sizeof(vw);

  GetVersionExW(&vw);

  memcpy(lpv, &vw, 5 * sizeof(DWORD));

  WideCharToMultiByte(CP_ACP, 0, 
		      vw.szCSDVersion, -1, 
		      lpv->szCSDVersion, sizeof(lpv->szCSDVersion), 
		      NULL, NULL);

  return TRUE;
}
#endif

LRESULT XCEAPI
XCESendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  wchar_t *lpNewText = NULL;
  LRESULT res;
  LPARAM lParamOrig = lParam;

  // there are lots more...

  if(
     Msg == WM_SETTEXT || 
     Msg == EM_REPLACESEL ||
     Msg == LB_ADDSTRING ||
     Msg == LB_INSERTSTRING ||
     //Msg == LB_ADDFILE ||
     Msg == LB_FINDSTRING ||
     Msg == LB_FINDSTRINGEXACT ||
     Msg == LB_SELECTSTRING ||
     Msg == CB_ADDSTRING ||
     Msg == CB_INSERTSTRING ||
     //Msg == CB_DIR ||
     Msg == CB_FINDSTRING ||
     Msg == CB_FINDSTRINGEXACT ||
     Msg == CB_SELECTSTRING
     )
    {
      char *s = (char *) lParam;
      int len = strlen(s) + 1;

      lpNewText = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, s, -1, lpNewText, len);

      lParam = (LPARAM) lpNewText;
    }
  else if(
	  Msg == WM_GETTEXT
	  )
    {
      lpNewText = malloc(wParam * 2);
      lParam = (LPARAM) lpNewText;
      lpNewText[0] = 0;
    }
  else if(
	  Msg == LB_GETTEXT ||
	  Msg == CB_GETLBTEXT
	  )
    {
      lpNewText = malloc(512);
      lParam = (LPARAM) lpNewText;
      lpNewText[0] = 0;
    }

  res = SendMessageW(hWnd, Msg, wParam, lParam);

  if(Msg == WM_GETTEXT)
    {
      char *s = (char *) lParamOrig;

      WideCharToMultiByte(CP_ACP, 0, 
			  lpNewText, -1, 
			  s, wParam, NULL, NULL);
    }
  else if(Msg == LB_GETTEXT)
    {
      char *s = (char *) lParamOrig;

      // unchecked!
      WideCharToMultiByte(CP_ACP, 0, 
			  lpNewText, -1, 
			  s, 512, NULL, NULL);
    }

  if(lpNewText)
    free(lpNewText);

  return res;
}

// there is rarely text posted...

LRESULT XCEAPI
XCEPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return PostMessageW(hWnd, Msg, wParam, lParam);
}

BOOL XCEAPI
XCEPostThreadMessageA(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return PostThreadMessageW(idThread, Msg, wParam, lParam);
}

BOOL XCEAPI
XCEPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax,
		UINT wRemoveMsg)
{
  return PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, 
		      wRemoveMsg);
}

BOOL XCEAPI
XCEGetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
  return GetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

LONG XCEAPI
XCEDispatchMessageA(CONST MSG *lpmsg)
{
  return DispatchMessageW(lpmsg);
}

HANDLE XCEAPI
XCECreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes,
		BOOL bManualReset,  
		BOOL bInitialState, 
		const char *lpName
		)
{
  HANDLE hEvent;
  wchar_t *lpNameNew = NULL;
  int len;

  if(lpName)
    {
      len = strlen(lpName) + 1;
      lpNameNew = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);
    }

  hEvent = CreateEventW(lpEventAttributes, bManualReset,
			bInitialState, lpNameNew);

  free(lpNameNew);

  return hEvent;
}

DWORD XCEAPI
XCEGetModuleFileNameA(HMODULE hModule, LPTSTR lpName, DWORD nSize)
{
  wchar_t *lpNameNew = NULL;
  DWORD dwRes;

  lpNameNew = malloc(nSize * 2);
  dwRes = GetModuleFileNameW(hModule, lpNameNew, nSize);

  if(dwRes != 0)
    {
      WideCharToMultiByte(CP_ACP, 0, 
			  lpNameNew, nSize, 
			  lpName, nSize, 
			  NULL, NULL);
    }

  free(lpNameNew);
  return dwRes;
}

BOOL XCEAPI
XCESetWindowTextA(HWND hWnd, const char *s)
{
  wchar_t *lpNewText = NULL;
  BOOL res;
  int len = strlen(s) + 1;

  lpNewText = malloc(len * 2);

  MultiByteToWideChar(CP_ACP, 0, s, -1, lpNewText, len);

  res = SetWindowTextW(hWnd, lpNewText);

  free(lpNewText);

  return res;
}

BOOL XCEAPI
XCEGetWindowTextA(HWND hWnd, char *lpString, int nMaxCount)
{
  return XCESendMessageA(hWnd, WM_GETTEXT, nMaxCount, (LPARAM) lpString);
}

int XCEAPI
XCEMessageBoxA(HWND hwnd, const char *txt, const char *caption, UINT flags)
{
  int res;
  wchar_t *txtw = NULL;
  wchar_t *captionw = NULL;
  int txtlen = 0;
  int captionlen = 0;

  if(txt)
    {
      txtlen = strlen(txt) + 1;
      txtw = malloc(txtlen * 2);
      MultiByteToWideChar(CP_ACP, 0, txt, -1, txtw, txtlen);
    }

  if(caption)
    {
      captionlen = strlen(caption) + 1;
      captionw = malloc(captionlen * 2);
      MultiByteToWideChar(CP_ACP, 0, caption, -1, captionw, captionlen);
    }

  res = MessageBoxW(hwnd, txtw, captionw, flags);

  free(txtw);
  free(captionw);

  return res;
}

BOOL XCEAPI
XCEExtTextOutA(HDC hdc, int X, int Y, UINT fuOptions, CONST RECT *lprc,
	       const char *txt, UINT cbCount, CONST INT *lpDx)
{
  BOOL res;
  wchar_t *txtw;

  if(cbCount == -1)
    cbCount = strlen(txt);

  txtw = _alloca((cbCount + 1) * 2);

  MultiByteToWideChar(CP_ACP, 0, txt, cbCount, txtw, cbCount);

  res = ExtTextOutW(hdc, X, Y, fuOptions, lprc, txtw, cbCount, lpDx);

  return res;
}

BOOL XCEAPI
XCEAttachThreadInput(DWORD idAttach, 
		     DWORD idAttachTo,
		     BOOL fAttach)
{
  return FALSE;
}

void XCEAPI
XCEGetSystemTimeAsFileTime(LPFILETIME pft)
{
  SYSTEMTIME st;

  GetSystemTime(&st);
  SystemTimeToFileTime(&st, pft);
}

int XCEAPI
XCEGetScrollPos(HWND hWnd, int nBar)
{
  SCROLLINFO si;

  si.cbSize = sizeof(si);
  si.fMask = SIF_ALL;

  GetScrollInfo(hWnd, nBar, &si);

  return si.nPos;
}

int XCEAPI
XCESetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
{
  SCROLLINFO si;

  si.cbSize = sizeof(si);
  si.fMask = SIF_POS;
  si.nPos = nPos;

  return SetScrollInfo(hWnd, nBar, &si, bRedraw);
}

BOOL XCEAPI
XCEAdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
{
  return AdjustWindowRectEx(lpRect, dwStyle, bMenu, 0);
}

UINT XCEAPI
XCEMapVirtualKeyA(UINT uCode, UINT uMapType)
{
  return MapVirtualKeyW(uCode, uMapType);
}

BOOL XCEAPI
XCEGetTextExtentExPointA(HDC hdc,
			 const char *s, 
			 int cchString,   
			 int nMaxExtent,  
			 LPINT lpnFit,                       
			 LPINT alpDx,     
			 LPSIZE lpSize    
			 )
{
  wchar_t *ws = NULL;
  BOOL res;

  ws = malloc(cchString * 2);

  MultiByteToWideChar(CP_ACP, 0, 
		      s, cchString, 
		      ws, cchString);
  
  res = GetTextExtentExPointW(hdc, ws, cchString, nMaxExtent, lpnFit,
			      alpDx, lpSize);

  free(ws);
  return res;
}

// When object is a HFONT, buffer contains a LOGFONT!
// This the main use in emacs (menufont).

int XCEAPI
XCEGetObjectA(HGDIOBJ hgdiobj,
	      int cbBuffer,     
	      LPVOID lpvObject)
{
  if(cbBuffer == sizeof(LOGFONTA))
    {
      LOGFONTW lfw;
      LOGFONTA *plfa = (LOGFONTA *) lpvObject;
      int res;
      
      res = GetObjectW(hgdiobj, sizeof(LOGFONTW), &lfw);

      memcpy(plfa, &lfw, sizeof(LOGFONTA));

      WideCharToMultiByte(CP_ACP, 0, 
			  lfw.lfFaceName, -1, 
			  plfa->lfFaceName, 
			  COUNTOF(plfa->lfFaceName), 
			  NULL, NULL);

      return res;
    }

  return GetObjectW(hgdiobj, cbBuffer, lpvObject);
}

BOOL XCEAPI
XCEGetTextMetricsA(HDC hdc, LPTEXTMETRICA lptma)
{
  TEXTMETRICW tmw;
  BOOL res;

  res = GetTextMetricsW(hdc, &tmw);

  lptma->tmHeight = tmw.tmHeight;
  lptma->tmAscent = tmw.tmAscent;
  lptma->tmDescent = tmw.tmDescent;
  lptma->tmInternalLeading = tmw.tmInternalLeading;
  lptma->tmExternalLeading = tmw.tmExternalLeading;
  lptma->tmAveCharWidth = tmw.tmAveCharWidth;
  lptma->tmMaxCharWidth = tmw.tmMaxCharWidth;
  lptma->tmWeight = tmw.tmWeight;
  lptma->tmOverhang = tmw.tmOverhang;
  lptma->tmDigitizedAspectX = tmw.tmDigitizedAspectX;
  lptma->tmDigitizedAspectY = tmw.tmDigitizedAspectY;
  lptma->tmFirstChar = tmw.tmFirstChar;
  lptma->tmLastChar = tmw.tmLastChar;
  lptma->tmDefaultChar = tmw.tmDefaultChar;
  lptma->tmBreakChar = tmw.tmBreakChar;
  lptma->tmItalic = tmw.tmItalic;
  lptma->tmUnderlined = tmw.tmUnderlined;
  lptma->tmStruckOut = tmw.tmStruckOut;
  lptma->tmPitchAndFamily = tmw.tmPitchAndFamily;
  lptma->tmCharSet = tmw.tmCharSet;

  return res;
}

BOOL XCEAPI
XCEAppendMenuA(HMENU hMenu,
	       UINT uFlags,
	       UINT uIDNewItem, 
	       const char *s
	       )
{
  BOOL res;
  int len;
  wchar_t *ws = NULL;

  if(s != NULL)
    {
      len = strlen(s) + 1;
      ws = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, 
			  s, -1, 
			  ws, len);
    }

  res = AppendMenuW(hMenu, uFlags, uIDNewItem, ws);

  if(ws)
    free(ws);

  return res;
}

HWND XCEAPI
XCECreateWindowExA(DWORD dwExStyle,      
		LPCTSTR lpClassName,  
		LPCTSTR lpWindowName, 
		DWORD dwStyle,        
		int x,                
		int y,                
		int nWidth,           
		int nHeight,          
		HWND hWndParent,      
		HMENU hMenu,          
		HINSTANCE hInstance,  
		LPVOID lpParam
		)
{
  wchar_t classnamew[126];
  wchar_t windownamew[126];
  wchar_t *pwindownamew;
  wchar_t *lpclassnamew;

  HWND hwnd;

  // this can be an atom...
  if(IsBadReadPtr(lpClassName, 1))
    {
      lpclassnamew = (WCHAR *) lpClassName;
    }
  else
    {
      MultiByteToWideChar(CP_ACP, 0, 
			  lpClassName, -1, 
			  classnamew, COUNTOF(classnamew));
      lpclassnamew = classnamew;
    }

  if(lpWindowName)
    {
      MultiByteToWideChar(CP_ACP, 0, 
			  lpWindowName, -1, 
			  windownamew, COUNTOF(windownamew));
      pwindownamew = windownamew;
    }
  else
    {
      pwindownamew = NULL;
    }

  hwnd = CreateWindowExW(dwExStyle, lpclassnamew, pwindownamew, dwStyle,
			 x, y, nWidth, nHeight, hWndParent, hMenu,
			 hInstance, lpParam);

  return hwnd;
}

HWND XCEAPI
XCECreateWindowA(LPCTSTR lpClassName,  
		LPCTSTR lpWindowName, 
		DWORD dwStyle,        
		int x,                
		int y,                
		int nWidth,           
		int nHeight,          
		HWND hWndParent,      
		HMENU hMenu,          
		HINSTANCE hInstance,  
		LPVOID lpParam
		)
{
  return XCECreateWindowExA(0, lpClassName, lpWindowName, dwStyle,
			  x, y, nWidth, nHeight, hWndParent, hMenu,
			  hInstance, lpParam);
}

HMODULE XCEAPI
XCEGetModuleHandleA(const char *lpName)
{
  HMODULE hMod = NULL;
  wchar_t *lpNameNew = NULL;
  int len;
  char path[256];

#if UNDER_CE > 201
  if(lpName)
    {
      len = strlen(lpName) + 1;
      lpNameNew = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);
    }

  hMod = GetModuleHandleW(lpNameNew);

  free(lpNameNew);
#else
  if(lpName == NULL)
    {
      XCEShowMessageA("GetModuleHandle() on EXE does not work in WCE200");
      return NULL;

      XCEGetModuleFileNameA(NULL, path, sizeof(path));
    }

  // LoadLibrary on EXE does not seem to work...
  hMod = XCELoadLibraryA(path);
  FreeLibrary(hMod);

  // check if loaded...
  if(XCEGetModuleFileNameA(hMod, path, sizeof(path)) == 0)
    return NULL;
#endif

  return hMod;
}

HANDLE XCEAPI
XCECreateFileMappingA(HANDLE hFile, 
		      LPSECURITY_ATTRIBUTES lpFileMappingAttributes, 
		      DWORD flProtect, 
		      DWORD dwMaximumSizeHigh, 
		      DWORD dwMaximumSizeLow, 
		      const char *lpName)
{
  HANDLE hFileMap;
  wchar_t *lpNameNew = NULL;
  int len;

  if(lpName)
    {
      len = strlen(lpName) + 1;
      lpNameNew = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);
    }

  hFileMap = CreateFileMappingW(hFile, lpFileMappingAttributes,
				flProtect, dwMaximumSizeHigh,
				dwMaximumSizeLow, lpNameNew);

  free(lpNameNew);

  return hFileMap;
}

int XCEAPI
XCEGetEnvironmentVariableA(const char *name, char *buf, int len)
{
  char *p;

  if(buf)
    buf[0] = 0;

  if((p = xcegetenv(name)) == NULL)
    return 0;

  if(buf == NULL)
    return strlen(p);

  strcpy(buf, p);

  return strlen(buf);
}

HMODULE XCEAPI
XCELoadLibraryA(const char *name)
{
  HMODULE hMod;
  wchar_t namew[MAX_PATH];
  wchar_t newpathw[MAX_PATH];

  if(__xcetrace > 1)
    XCETrace("LoadLibrary(%s)", name);

  MultiByteToWideChar(CP_ACP, 0, name, -1, namew, COUNTOF(namew));

  // try full path
  XCEFixPathW(namew, newpathw);

  hMod = LoadLibraryW(newpathw);
  if(hMod == NULL)
    {
      // try original path
      hMod = LoadLibraryW(namew);
    }

  if(hMod == NULL)
    {
      DWORD dwError = GetLastError();

      XCETrace("LoadLibrary(%s): Error %d", name, dwError);
      
      //XCEShowMessageW(L"LoadLibrary(%s): Error %d", namew, dwError);
    }

  if(__xcetrace > 1)
    XCETrace("LoadLibrary() = 0x%x", hMod);

  return hMod;
}

FARPROC XCEAPI
XCEGetProcAddressA(HMODULE hMod, const char *name)
{
  wchar_t namew[126];
  FARPROC proc;

  MultiByteToWideChar(CP_ACP, 0, name, -1, namew, COUNTOF(namew));

  if((proc = GetProcAddressW(hMod, namew)) == NULL)
    XCETrace("GetProcAddress(0x%x, %s): %d", hMod, name, GetLastError());

  return proc;
}

HWND XCEAPI
XCEFindWindowA(const char *classname, const char *title)
{
  wchar_t classnamew[126];
  wchar_t titlew[126];
  wchar_t *pclassnamew, *ptitlew;

  pclassnamew = ptitlew = NULL;

  if(classname)
    {
      MultiByteToWideChar(CP_ACP, 0, classname, -1, classnamew, 
			  COUNTOF(classnamew));
      pclassnamew = classnamew;
    }

  if(title)
    {
      MultiByteToWideChar(CP_ACP, 0, title, -1, titlew, 
			  COUNTOF(titlew));
      ptitlew = titlew;
    }

  return FindWindowW(pclassnamew, ptitlew);
}

// only old style supported yet

BOOL XCEAPI
XCEBuildCommDCBA(LPCTSTR lpDef, LPVOID xlpDCB)
{
  // baud=1200 parity=N data=8 stop=1 
  // 9600,n,8,1
  char buf[126];
  char *p;
  int extra = 0;
  DCB *lpDCB = (DCB *) xlpDCB;

  SetLastError(0);

  strcpy(buf, lpDef);

  if((p = strtok(buf, ",")) == NULL) goto bad;
  lpDCB->BaudRate = atol(p);

  if((p = strtok(NULL, ",")) == NULL) goto bad;
  if(tolower(*p) == 'n')
    {
      lpDCB->fParity = 0;
      lpDCB->Parity = NOPARITY;
    }
  else
    {
      goto bad;
    }

  if((p = strtok(NULL, ",")) == NULL) goto bad;
  lpDCB->ByteSize = atol(p);
  if(lpDCB->ByteSize != 7 && lpDCB->ByteSize != 8)
    goto bad;

  if((p = strtok(NULL, ",")) == NULL) goto bad;
  lpDCB->StopBits = atol(p);
  if(lpDCB->StopBits == 1)
    lpDCB->StopBits = ONESTOPBIT;
  else if(lpDCB->StopBits == 2)
    lpDCB->StopBits = TWOSTOPBITS;
  else
    goto bad;

  if((p = strtok(NULL, ",")) != NULL)
    {
      if(*p == 'p')
	{
	  lpDCB->fInX = lpDCB->fOutX = FALSE;
	  lpDCB->fOutxDsrFlow = lpDCB->fOutxCtsFlow = TRUE;
	  lpDCB->fDtrControl = DTR_CONTROL_HANDSHAKE;
	  lpDCB->fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
      else if(*p == 'x')
	{
	  lpDCB->fInX = lpDCB->fOutX = TRUE;
	  lpDCB->fOutxDsrFlow = lpDCB->fOutxCtsFlow = FALSE;
	  lpDCB->fDtrControl = DTR_CONTROL_ENABLE;
	  lpDCB->fRtsControl = RTS_CONTROL_ENABLE;
	}
      else
	{
	  goto bad;
	}
    }
  else
    {
      lpDCB->fInX = lpDCB->fOutX = FALSE;
      lpDCB->fOutxDsrFlow = lpDCB->fOutxCtsFlow = FALSE;
      lpDCB->fDtrControl = DTR_CONTROL_ENABLE;
      lpDCB->fRtsControl = RTS_CONTROL_ENABLE;
    }

  return TRUE;

 bad:
  SetLastError(ERROR_INVALID_PARAMETER);
  return FALSE;
}

LPSTR XCEAPI
XCECharLowerA(LPSTR lpsz)
{
  LPTSTR p;

  if(lpsz <= (LPSTR) 0xFF)
    return (LPSTR) tolower((int) lpsz);

  for(p = lpsz; *p; p++)
    *p = tolower(*p);

  return lpsz;
}

LPSTR XCEAPI
XCECharUpperA(LPTSTR lpsz)
{
  LPTSTR p;

  if(lpsz <= (LPSTR) 0xFF)
    return (LPSTR) toupper((int) lpsz);

  for(p = lpsz; *p; p++)
    *p = toupper(*p);

  return lpsz;
}

DWORD XCEAPI
XCEGetFullPathNameA(LPCTSTR lpFileName, DWORD nBufferLength, 
		    LPTSTR lpBuffer, LPTSTR *lpFilePart)
{
  int len;
  char *p;
  char curdir[256];
  char tmp[256];
  char tmp2[256];

  if(lpFileName[0] == '/' || lpFileName[0] == '\\')
    {
      len = xcesnprintf(lpBuffer, nBufferLength, "%s", lpFileName);
    }
  else
    {
      xcegetcwd(curdir, sizeof(curdir));
      sprintf(tmp, "%s\\%s", curdir, lpFileName);
      XCEFixPathA(tmp, tmp2);
      len = xcesnprintf(lpBuffer, nBufferLength, "%s", tmp2);
    }

  for(p = lpBuffer; *p; p++)
    {
      if(*p == '/')
	*p = '\\';
    }

  if(lpFilePart)
    {
      if((p = xcestrrchr(lpBuffer, '\\')) == NULL)
	p = lpBuffer;
      else
	p++;

      *lpFilePart = p;
    }

  return len;
}

DWORD XCEAPI
XCEGetShortPathNameA(LPCTSTR lpszLongPath, LPTSTR lpszShortPath, 
		     DWORD cchBuffer)
{
  return xcesnprintf(lpszShortPath, cchBuffer, "%s", lpszLongPath);
}

#if UNDER_CE >= 300
UINT XCEAPI
XCEGetTempFileNameA(LPCTSTR lpPathName, LPCTSTR lpPrefixString, 
		   UINT uUnique, LPTSTR lpTempFileName)
{
  UINT res;
  wchar_t lpPathNameW[MAX_PATH];
  wchar_t lpPrefixStringW[MAX_PATH];
  wchar_t lpTempFileNameW[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, lpPathName, -1, lpPathNameW, 
                      MAX_PATH);
  MultiByteToWideChar(CP_ACP, 0, lpPrefixString, -1, lpPrefixStringW, 
                      MAX_PATH);

  res = GetTempFileNameW(lpPathNameW, lpPrefixStringW, uUnique, 
                         lpTempFileNameW);

  WideCharToMultiByte(CP_ACP, 0, lpTempFileNameW, -1, lpTempFileName, 
                      MAX_PATH, NULL, NULL);

  return res;
}
#else
UINT XCEAPI
XCEGetTempFileNameA(LPCTSTR lpPathName, LPCTSTR lpPrefixString, 
		   UINT uUnique, LPTSTR lpTempFileName)
{
  char *tmpdir;
  int num;

  // the spec says that it MUST be set
  if(lpPathName == NULL)
    {
      if((tmpdir = xcegetenv("TMP")) == NULL)
	tmpdir = "\\temp";
    }
  else
    {
      tmpdir = lpPathName;
    }

  if(uUnique == 0)
    num = GetTickCount();
  else
    num = uUnique;

  sprintf(lpTempFileName, "%s\\%s%x.tmp", tmpdir, lpPrefixString, num);

  if(uUnique == 0)
    {
      int fd;

      if((fd = xcecreat(lpTempFileName, 0666)) < 0)
	return 0;
      xceclose(fd);
    }

  return num;
}
#endif


DWORD XCEAPI
XCEGetTempPathA(DWORD size, LPTSTR  lpBuffer)	
{
  char *p;
  int len;

  if((p = xcegetenv("TEMP")) || (p = xcegetenv("TMP")))
    len = xcesnprintf(lpBuffer, size, "%s/", p);
  else
    len = xcesnprintf(lpBuffer, size, "%s/", "/temp");

  XCEToDosPath(lpBuffer);

  return len;
}

BOOL XCEAPI
XCEGetVolumeInformationA(LPCTSTR  lpRootPathName,	
			LPTSTR  lpVolumeNameBuffer,
			DWORD  nVolumeNameSize,	
			LPDWORD  lpVolumeSerialNumber,
			LPDWORD  lpMaximumComponentLength,
			LPDWORD  lpFileSystemFlags,	
			LPTSTR  lpFileSystemNameBuffer,
			DWORD  nFileSystemNameSize)
{
  XCEShowMessageA("GetVolumeInformationA not implemented");
  return FALSE;
}

int XCEAPI
XCEwsprintfA(char *buf, const char *fmt, ...)
{
  va_list ap;
  int len;

  va_start(ap, fmt);
  len = vsprintf(buf, fmt, ap);
  va_end(ap);

  return len;
}

int XCEAPI
XCElstrcmpA(const char *s1, const char *s2)
{
  return strcmp(s1, s2);
}

int XCEAPI
XCElstrcmpiA(const char *s1, const char *s2)
{
  return xcestricmp(s1, s2);
}

char * XCEAPI
XCElstrcpyA(char *d, const char *s)
{
  return strcpy(d, s);
}

char * XCEAPI
XCElstrcpynA(char *d, const char *s, int n)
{
  return strncpy(d, s, n);
}

int XCEAPI
XCElstrlenA(const char *s)
{
  return strlen(s);
}

UINT XCEAPI
XCESetErrorMode(UINT mode)
{
  UINT res = _errormode;

  _errormode = mode;

  return res;
}

VOID XCEAPI
XCEExitProcess(UINT code)
{
  xceexit(code);
}

// this is originally a define, but some code wants to take the
// address... Needs to be XCEAPI for TK!

BOOL XCEAPI
XCEGetTextExtentPoint32W(HDC  hdc, LPCWSTR  lpString, int  cbString, LPSIZE  lpSize)
{
  return GetTextExtentPointW(hdc, lpString, cbString, lpSize);
}

BOOL XCEAPI
XCEGetTextExtentPoint32A(HDC  hdc, LPCSTR  lpString, int  cbString, LPSIZE  lpSize)
{
  BOOL res;
  wchar_t *lpStringW;

  if(cbString == -1)
    cbString = strlen(lpString);

  lpStringW = _alloca((cbString + 1) * 2);

  MultiByteToWideChar(CP_ACP, 0, lpString, cbString, lpStringW, cbString);

  res = GetTextExtentPointW(hdc, lpStringW, cbString, lpSize);

  return res;
}

BOOL XCEAPI 
XCETextOutA(HDC  hdc, int  nXStart, int  nYStart, LPCSTR  lpString, int  cbString)
{
  UINT fuOptions = 0;
  return XCEExtTextOutA(hdc, nXStart, nYStart, fuOptions, NULL, lpString, cbString, NULL);
}

BOOL XCEAPI 
XCETextOutW(HDC  hdc, int  nXStart, int  nYStart, LPCWSTR  lpString, int  cbString)
{
  UINT fuOptions = 0;
  return ExtTextOutW(hdc, nXStart, nYStart, fuOptions, NULL, lpString, cbString, NULL);
}

BOOL XCEAPI
XCEGetCharWidthA(HDC hdc, UINT iFirstChar, UINT  iLastChar, LPINT  lpBuffer)
{
  SIZE s;
  WCHAR buf[1];
  int i, n;

  for(i = iFirstChar, n = 0; i <= iLastChar; i++)
    {
      buf[0] = i;
      GetTextExtentPoint32W(hdc, buf, 1, &s);
      lpBuffer[n++] = s.cx;
    }

  return TRUE;
}

BOOL XCEAPI
XCEGetCharWidthW(HDC  hdc, UINT  iFirstChar, UINT  iLastChar, LPINT  lpBuffer)
{
  SIZE s;
  WCHAR buf[1];
  int i, n;

  for(i = iFirstChar, n = 0; i <= iLastChar; i++)
    {
      buf[0] = i;
      GetTextExtentPoint32W(hdc, buf, 1, &s);
      lpBuffer[n++] = s.cx;
    }

  return TRUE;
}

// only for truetype
DWORD XCEAPI
XCEGetFontData(HDC  hdc, DWORD  dwTable, DWORD  dwOffset, 
	       LPVOID  lpvBuffer, DWORD  cbData)
{
  return GDI_ERROR;
}

UINT XCEAPI
XCEGetTextCharset(HDC hdc)
{
  return ANSI_CHARSET;
}

DWORD XCEAPI
XCEGetVersion(VOID)
{
  DWORD dwRes;
  OSVERSIONINFOW vw;
  
#if UNDER_CE > 201
  GetVersionExW(&vw);
#else
  GetVersionEx(&vw);
#endif

  dwRes = (vw.dwMinorVersion  << 8) | vw.dwMajorVersion;

  return dwRes;
}

HPEN XCEAPI
XCEExtCreatePen(DWORD dwPenStyle, DWORD dwWidth, CONST LOGBRUSH *lplb, DWORD dwStyleCount, CONST DWORD *lpStyle)
{
  return CreatePen(dwPenStyle, dwWidth, RGB(0,0,0));
}

VOID XCEAPI
XCEOutputDebugStringA(LPCSTR s)
{
  int len;
  wchar_t *ws;

  len = strlen(s);
  ws = _alloca((len + 1) * 2);

  MultiByteToWideChar(CP_ACP, 0, s, -1, ws, len+1);

  OutputDebugStringW(ws);
}

HCURSOR XCEAPI
XCELoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName)
{
  wchar_t *lpCursorNameW;
  HCURSOR hc;

  if(lpCursorName > (LPCSTR) 0xFFFF)
    {
      int len = strlen(lpCursorName);

      lpCursorNameW = _alloca((len + 1) * 2);
      MultiByteToWideChar(CP_ACP, 0, lpCursorName, -1, lpCursorNameW, len + 1);
    }
  else
    {
      lpCursorNameW = (LPWSTR) (LPSTR) lpCursorName;
    }

  hc = LoadCursorW(hInstance, lpCursorNameW);

  return hc;
}

HICON XCEAPI
XCELoadIconA(HINSTANCE  hInstance, LPCTSTR  lpIconName)
{
  wchar_t *lpIconNameW;
  HICON hi;

  if(lpIconName > (LPCSTR) 0xFFFF)
    {
      int len = strlen(lpIconName);

      lpIconNameW = _alloca((len + 1) * 2);
      MultiByteToWideChar(CP_ACP, 0, lpIconName, -1, lpIconNameW, len + 1);
    }
  else
    {
      lpIconNameW = (LPWSTR) (LPSTR) lpIconName;
    }

  hi = LoadIconW(hInstance, lpIconNameW);

  return hi;
}

UINT XCEAPI
XCEGetDlgItemTextA(HWND hDlg, int nIDDlgItem, LPSTR lpString, int nMaxCount)
{
  LPWSTR lpStringW;
  UINT res;

  lpStringW = _alloca((nMaxCount + 1) * 2);

  res = GetDlgItemTextW(hDlg, nIDDlgItem, lpStringW, nMaxCount);

  if(res >= 0)
    {
      WideCharToMultiByte(CP_ACP, 0, lpStringW, -1, lpString, 
			  nMaxCount, NULL, NULL);
    }
  return res;
}

BOOL XCEAPI 
XCESetDlgItemTextA(HWND hwndDlg, int idControl, LPCSTR lpsz)
{
  LPWSTR lpszW;
  int len;
  BOOL res;

  len = strlen(lpsz);

  lpszW = _alloca((len + 1) * 2);
  MultiByteToWideChar(CP_ACP, 0, lpsz, -1, lpszW, len + 1);

  res = SetDlgItemTextW(hwndDlg, idControl, lpszW);

  return res;
}

LONG XCEAPI 
XCEGetWindowLongA(HWND hWnd, int nIndex)
{
  if(hWnd == NULL)
    DebugBreak();
  return GetWindowLongW(hWnd, nIndex);
}

LONG XCEAPI 
XCESetWindowLongA(HWND hWnd, int nIndex, LONG newLong)
{
  if(hWnd == NULL)
    DebugBreak();
  return SetWindowLongW(hWnd, nIndex, newLong);
}

BOOL XCEAPI 
XCEIsIconic(HWND hwnd)
{
  return FALSE;
}

BOOL XCEAPI 
XCEIsZoomed(HWND hwnd)
{
  return FALSE;
}

ATOM XCEAPI
XCERegisterClassA(CONST WNDCLASSA *lpwc)
{
  ATOM res;
  LPCSTR lpszClassName;
  LPWSTR lpszClassNameW;
  WNDCLASSW wcW;
  int len;

  lpszClassName = lpwc->lpszClassName;
  len = strlen(lpszClassName);

  memcpy(&wcW, lpwc, sizeof(WNDCLASSA));

  lpszClassNameW = _alloca((len + 1) * 2);
  MultiByteToWideChar(CP_ACP, 0, lpszClassName, -1, lpszClassNameW, len + 1);
  
  wcW.lpszClassName = lpszClassNameW;
  wcW.lpszMenuName = NULL;

  res = RegisterClassW(&wcW);

  return res;
}

BOOL XCEAPI
XCEUnregisterClassA(LPCSTR lpszClassName, HINSTANCE  hinst)
{
  int len;
  LPWSTR lpszClassNameW;

  len = strlen(lpszClassName);

  lpszClassNameW = _alloca((len + 1) * 2);

  MultiByteToWideChar(CP_ACP, 0, lpszClassName, -1, lpszClassNameW, len + 1);

  return UnregisterClassW(lpszClassNameW, hinst);
}

int XCEAPI 
XCEMulDiv(int nNumber, int nNumerator, int nDenominator)
{
  __int64 res;

  res = ((__int64) nNumber * (__int64) nNumerator) / (__int64) nDenominator;

  return (int) res;
}

LRESULT XCEAPI 
XCEDefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

LONG XCEAPI
XCESendDlgItemMessageA(HWND hwndDlg, int idControl, UINT uMsg, 
		       WPARAM wParam, LPARAM lParam)
{
  LONG res;
  wchar_t *wbuf = NULL;
  char *buf = NULL;
  int len = 0;

  if(uMsg == WM_SETTEXT ||
     uMsg == CB_ADDSTRING ||
     uMsg == CB_SELECTSTRING)
    {
      if(lParam != 0)
	{
	  buf = (char *) lParam;
	  len = strlen(buf) + 1;
      
	  wbuf = _alloca(len * 2);
	  MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, len);
	  lParam = (LPARAM) wbuf;
	}
    }
  else if(uMsg == WM_GETTEXT)
    {
      len = wParam;
      buf = (char *) lParam;
      wbuf = _alloca(len * 2);
      lParam = (LPARAM) wbuf;
    }

  res = SendDlgItemMessageW(hwndDlg, idControl, uMsg, wParam, lParam);

  if(uMsg == WM_GETTEXT)
    {
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, (char *) buf, len, 0, 0);
    }

  return res;
}

LRESULT XCEAPI
XCECallWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return CallWindowProcW(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
}

int XCEAPI 
XCEGetTextFaceA(HDC hdc, int nCount, LPSTR lpFaceName)
{
  LPWSTR lpFaceNameW;
  int res;

  lpFaceNameW = _alloca((nCount + 1) * 2);

  res = GetTextFaceW(hdc, nCount, lpFaceNameW);

  WideCharToMultiByte(CP_ACP, 0, lpFaceNameW, -1, lpFaceName, nCount, NULL, NULL);

  return res;
}

HDC XCEAPI 
XCECreateDCA(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, CONST DEVMODE *lpInitData)
{
  HDC hdc;
  LPWSTR lpszDriverW;
  int len;

  if(lpszDevice != NULL || lpszOutput != NULL || lpInitData != NULL)
    xceabort();

  len = strlen(lpszDriver);
  lpszDriverW = _alloca((len + 1) * 2);

  MultiByteToWideChar(CP_ACP, 0, lpszDriver, -1, lpszDriverW, len + 1);

  hdc = CreateDCW(lpszDriverW, NULL, NULL, NULL);

  return hdc;
}

BOOL XCEAPI
XCEInsertMenuA(HMENU hMenu, UINT uPosition, UINT uFlags, UINT uIDNewItem, LPCTSTR lpNewItem)
{
  LPWSTR lpNewItemW;
  int len;

  if(uFlags & MF_STRING)
    {
      len = strlen(lpNewItem);
      lpNewItemW = _alloca((len + 1) * 2);

      MultiByteToWideChar(CP_ACP, 0, lpNewItem, -1, lpNewItemW, len + 1);
    }
  else
    {
      lpNewItemW = (LPWSTR) (LPSTR) lpNewItem;
    }

  return InsertMenuW(hMenu, uPosition, uFlags, uIDNewItem, lpNewItemW);
}

BOOL XCEAPI
XCEGetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl)
{
  memset(lpwndpl, 0, sizeof(WINDOWPLACEMENT));

  // This is wrong when the window is minimized!
  lpwndpl->showCmd = SW_SHOWNORMAL;
  GetWindowRect(hWnd, &lpwndpl->rcNormalPosition);

  return TRUE;
}

BOOL WINAPI
XCEMoveWindow(HWND hwnd, UINT x, UINT y, UINT w, UINT h, BOOL repaint)
{
  BOOL bRes;

  // Windows CE ignores repaint!
  bRes = MoveWindow(hwnd, x, y, w, h, repaint);

  if(bRes && repaint)
    {
      InvalidateRect(hwnd, NULL, TRUE);
      // important to call UpdateWindow!
      UpdateWindow(hwnd);
    }

  return bRes;
}

VOID XCEAPI
XCECopyMemory(PVOID dest, const PVOID src, DWORD len)
{
  memcpy(dest, src, len);
}

// This function is not implemented in the palm sdk

HWND XCEAPI
XCEGetDesktopWindow()
{
  // This is defined as (HWND) 0. How do you distinguish
  // it from an invalid window?
  return HWND_DESKTOP;
}

//////////////////////////////////////////////////////////////////////
// Sun May 27 2001
//
// This is for tk, only tested there, seems to work.

int XCEAPI
XCEStretchDIBits(
    HDC  hdc,	// handle of device context 
    int  XDest,	// x-coordinate of upper-left corner of dest. rect. 
    int  YDest,	// y-coordinate of upper-left corner of dest. rect. 
    int  nDestWidth,	// width of destination rectangle 
    int  nDestHeight,	// height of destination rectangle 
    int  XSrc,	// x-coordinate of upper-left corner of source rect. 
    int  YSrc,	// y-coordinate of upper-left corner of source rect. 
    int  nSrcWidth,	// width of source rectangle 
    int  nSrcHeight,	// height of source rectangle 
    VOID  *lpBits,	// address of bitmap bits 
    BITMAPINFO *lpBitsInfo,	// address of bitmap data 
    UINT  iUsage,	// usage 
    DWORD  dwRop 	// raster operation code 
    )
{
  HDC hdcmem;
  HBITMAP hbitmap;
  LPBYTE lpDestBits;
  DWORD dwLen;
  int nPadWidth = 4;
  int pad = 0;
  BOOL bSuccess;
  LPBITMAPINFOHEADER lpH;

  lpH = &lpBitsInfo->bmiHeader;

  if(lpH->biWidth % nPadWidth)
    pad = nPadWidth - lpH->biWidth % nPadWidth;

  // This is wrong when the infoheader is followed by color data...
  // We hope that biSizeImage is set then...
  if(lpH->biHeight < 0)
    dwLen = ((lpH->biWidth+pad)*(-lpH->biHeight)*lpH->biBitCount)/8;
  else
    dwLen = ((lpH->biWidth+pad)*(lpH->biHeight)*lpH->biBitCount)/8;

  // includes colordata, if any...
  if(lpH->biSizeImage != 0)
    dwLen = lpH->biSizeImage;

  if((hdcmem = CreateCompatibleDC(hdc)) == NULL)
    return GDI_ERROR;

  if((hbitmap = CreateDIBSection(hdcmem, lpBitsInfo, iUsage, &lpDestBits,
				 NULL, 0)) == NULL)
    {
      DeleteDC(hdcmem);
      return GDI_ERROR;
    }

  memcpy(lpDestBits, lpBits, dwLen);

  SelectObject(hdcmem, hbitmap);

  bSuccess = StretchBlt(hdc, 
			XDest,
			YDest,
			nDestWidth,
			nDestHeight,
			hdcmem,
			XSrc,
			YSrc,
			nSrcWidth,
			nSrcHeight,
			dwRop);

  DeleteDC(hdcmem);
  DeleteObject(hbitmap);

  if(!bSuccess)
    return GDI_ERROR;

  // should be number of scanlines copied...
  return 1;
}

//////////////////////////////////////////////////////////////////////
// Sun May 27 2001
//
// WinCE does not handle COLOR_XXX correctly!

int XCEAPI
XCEFillRect(HDC hdc, CONST RECT *lprc, HBRUSH hbr)
{
  int res;

  if((DWORD) hbr & 0x40000000) // COLOR_XXX, not handled by CE!
    {
      HBRUSH hbrnew;
      COLORREF cr;

      cr = GetSysColor((DWORD) hbr - 1);
      hbrnew = CreateSolidBrush(cr);
      res = FillRect(hdc, lprc, hbrnew);
      DeleteObject(hbrnew);
    }
  else
    {
      res = FillRect(hdc, lprc, hbr);
    }

  return res;
}

//////////////////////////////////////////////////////////////////////
// Sun Jul 22 2001

// "Not all Windows CE-based devices will contain the system
// message-table resources. This is a selectable part of the Windows
// CE operating system and is often removed to conserve space."

DWORD
XCEFormatMessageA(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId,
		  DWORD dwLangId, LPTSTR lpBuffer, DWORD nSize,
		  va_list *args)
{
  if(dwFlags & FORMAT_MESSAGE_ALLOCATE_BUFFER)
    {
      char *buf = LocalAlloc(LPTR, 126);

      *(char **) lpBuffer = buf;

      if(dwMessageId > 0x7FFFFFF)
	sprintf(buf, "OsError 0x%x", dwMessageId, dwMessageId);
      else
	sprintf(buf, "OsError %d", dwMessageId);

      return strlen(buf);
    }

  if(dwMessageId > 0x7FFFFFF)
    sprintf(lpBuffer, "OsError 0x%x", dwMessageId);
  else
    sprintf(lpBuffer, "OsError %d", dwMessageId);

  return strlen(lpBuffer);
}

HANDLE
XCECreateMutexA(LPSECURITY_ATTRIBUTES lpsa, BOOL bInitialOwner, LPCTSTR name)
{
  wchar_t *lpnamew = NULL;
  wchar_t namew[126];

  if(name)
    {
      MultiByteToWideChar(CP_ACP, 0, name, -1, namew, sizeof(namew)/2);
      lpnamew = namew;
    }

  return CreateMutexW(lpsa, bInitialOwner, lpnamew);
}

BOOL
XCECreateProcessA(LPCTSTR appname, 
		  LPCTSTR commandline,
		  LPVOID lpsaproc, 
		  LPVOID lpsathread, 
		  BOOL bInheritHandles,
		  DWORD dwCreationFlags,
		  LPVOID lpEnv,
		  LPCTSTR lpCurDir,
		  LPSTARTUPINFO lpsi,
		  LPPROCESS_INFORMATION lppi
		  )
{
  wchar_t wszAppName[126];
  char szAppName[126];
  wchar_t *wszCommandLine = NULL;
  char *szCommandLine = NULL;
  char *p;
  DWORD dwExitCode = 0;
  char path[MAX_PATH];
  char *d;
  char szCurrentDir[MAX_PATH];
  BOOL bRes;
  int commandlen;
  char buf[126];

  XCETrace("XCECreateProcess(%s,%s)", appname, commandline);

  if(appname != NULL)
    {
      int len;
      char *p;

      len = strlen(appname) + strlen(commandline) + 4;
      p = _alloca(len);

      if(appname[0] == '"')
	sprintf(p, "%s %s", appname, commandline);
      else
	sprintf(p, "\"%s\" %s", appname, commandline);

      commandline = p;
    }

  if(commandline[0] == '"')
    {
      for(p = (char *)commandline + 1, d = szAppName; *p && *p != '"';)
	{
	  *d++ = *p++;
	}

      if(*p != 0)
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
  wszCommandLine = malloc((commandlen+1)*2);
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

  //printf("App: %s Command: %s\n", szAppName, szCommandLine);

  MultiByteToWideChar(CP_ACP, 0, szAppName, -1,
                      wszAppName, sizeof(wszAppName)/2);
  MultiByteToWideChar(CP_ACP, 0, szCommandLine, -1,
                      wszCommandLine, commandlen + 1);

  // pass path to child...
  XCEGetCurrentDirectoryA(sizeof(szCurrentDir), szCurrentDir);
  // PWD is better, as this is checked by some unix programs...
  // XCESetEnvironmentVariableInRegA("PWD", szCurrentDir);
  // Only set it locally, passed via shared environment
  xcesetenv("PWD", szCurrentDir, 1);
  sprintf(buf, "%x", GetCurrentProcessId());
  xcesetenv("PPID", buf, 1);

  xceshared_lock();

  // initialize shared section for child
  if(lpEnv != NULL)
    {
      xceshared_setenvironblock(lpEnv);
    }
  else
    {
      char *lpCurrentEnv = XCEGetEnvironmentStrings();
      xceshared_setenvironblock(lpCurrentEnv);
      XCEFreeEnvironmentStrings(lpCurrentEnv);
    }

  if(lpCurDir != NULL)
    xceshared_setcwd(lpCurDir);
  else
    xceshared_setcwd(szCurrentDir);

  if(lpsi->dwFlags & STARTF_USESTDHANDLES)
    {
      xceshared_setstdhandle(STD_INPUT_HANDLE,
			     MAKEINHERIT(lpsi->hStdInput));
      xceshared_setstdhandle(STD_OUTPUT_HANDLE, 
			     MAKEINHERIT(lpsi->hStdOutput));
      xceshared_setstdhandle(STD_ERROR_HANDLE, 
			     MAKEINHERIT(lpsi->hStdError));

      // must increment open count because the
      // call normally closes the pipes. emacs
      // does no duplicate them...
      pipe_increment_opencount(MAKEINHERIT(lpsi->hStdInput));
      pipe_increment_opencount(MAKEINHERIT(lpsi->hStdOutput));
      pipe_increment_opencount(MAKEINHERIT(lpsi->hStdError));
    }
  else
    {
#if 0
      // this can interfere with the parents own in/out!
      // showed up when calling gnuplot from maxima (system())
      // commented out Thu Sep 06 2001
      xceshared_setstdhandle(STD_INPUT_HANDLE, 
			     XCEGetStdHandle(STD_INPUT_HANDLE));
      xceshared_setstdhandle(STD_OUTPUT_HANDLE, 
			     XCEGetStdHandle(STD_OUTPUT_HANDLE));
      xceshared_setstdhandle(STD_ERROR_HANDLE, 
			     XCEGetStdHandle(STD_ERROR_HANDLE));
#endif
    }

  XCETrace("shared output set to %x", 
	   xceshared_getstdhandle(STD_OUTPUT_HANDLE));

  xceshared_setshowwindow(TRUE);
  if(lpsi->dwFlags & STARTF_USESHOWWINDOW)
    {
      if(lpsi->wShowWindow == SW_HIDE)
	xceshared_setshowwindow(FALSE);
    }

  // TODO: released too early, but the child must be able to access it
  xceshared_release();

  XCETraceW(L"CreateProcess(%s, %.50s)", wszAppName, wszCommandLine);

  //SYNC;

  bRes = CreateProcessW(wszAppName, wszCommandLine, NULL, NULL,
			FALSE, 0, NULL, NULL, lpsi, lppi);

  // TODO: Wait for process ready (event)
  // TODO: Remove shared stuff, otherwise a non-child may get
  // confused!
  if(bRes == FALSE)
    {
      xceshared_reset();
    }

  free(szCommandLine);
  free(wszCommandLine);

  return bRes;
}

BOOL
XCEFreeEnvironmentStrings(LPCSTR buf)
{
  free((LPVOID) buf);
  return TRUE;
}

LPVOID
XCEGetEnvironmentStrings(VOID)
{
  LPSTR buf;
  int i;
  int size = 0;
  char *p;

  for(i = 0; environ[i] != NULL; i++)
    {
      size += strlen(environ[i]) + 1;
    }
  size += 1;

  if((buf = malloc(size)) == NULL)
    return NULL;

  for(i = 0, p = buf; environ[i] != NULL; i++)
    {
      strcpy(p, environ[i]);
      p += strlen(environ[i]) + 1;
    }
  *p = 0;

  return (LPVOID) buf;
}

int
XCEDialogBoxIndirectParamA(HINSTANCE  hInstance,
			   LPCDLGTEMPLATE  hDialogTemplate,
			   HWND  hWndParent,
			   DLGPROC  lpDialogFunc,
			   LPARAM  dwInitParam)
{
  return DialogBoxIndirectParamW(hInstance, hDialogTemplate, hWndParent,
				 lpDialogFunc, dwInitParam);
}

HRSRC XCEAPI
XCEFindResourceA(HMODULE  hModule, LPCSTR  lpName, LPCSTR  lpType)
{
  HRSRC hr;
  wchar_t *lpNameW;
  wchar_t *lpTypeW;

  if(lpName > (LPCSTR) 0xFFFF)
    {
      int len = strlen(lpName);

      lpNameW = _alloca((len + 1) * 2);
      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameW, len + 1);
    }
  else
    {
      lpNameW = (LPWSTR) (LPSTR) lpName;
    }

  if(lpType > (LPCSTR) 0xFFFF)
    {
      int len = strlen(lpType);

      lpTypeW = _alloca((len + 1) * 2);
      MultiByteToWideChar(CP_ACP, 0, lpType, -1, lpTypeW, len + 1);
    }
  else
    {
      lpTypeW = (LPWSTR) (LPSTR) lpType;
    }

  hr = FindResourceW(hModule, lpNameW, lpTypeW);

  return hr;
}

void
XCEZeroMemory(void *p, unsigned int len)
{
  memset(p, 0, len);
}

extern char *_commandline;

char *
XCEGetCommandLineA()
{
  return _commandline;
}

//////////////////////////////////////////////////////////////////////
// Tue Mar 26 2002

int XCEAPI
XCEGetWindowTextLengthA(HWND hWnd)
{
  return GetWindowTextLengthW(hWnd);
}

BOOL XCEAPI
XCEGetMenuItemInfoA(HMENU hMenu, UINT uItem, BOOL fByPos, 
		    LPMENUITEMINFOA lpmii)
{
  BOOL bRes;
  MENUITEMINFOW wmii;
  
  DebugBreak();

  bRes = GetMenuItemInfoW(hMenu, uItem, fByPos, &wmii);

  return bRes;
}

UINT XCEAPI
XCERegisterWindowMessageA(LPCSTR s)
{
  WCHAR wmsg[126];

  MultiByteToWideChar(CP_ACP, 0, s, -1, wmsg, sizeof(wmsg)/2);

  return RegisterWindowMessageW(wmsg);
}

//////////////////////////////////////////////////////////////////////
// Wed Apr 24 2002 (for Vim)

DWORD XCEAPI
XCESetClassLongA(HWND  hwnd,
		 int  nIndex,
		 LONG  lNewVal
		 )
{
  return SetClassLongW(hwnd, nIndex, lNewVal);
}

int XCEAPI
XCEGetClassNameA(HWND  hWnd,	
		 LPTSTR  lpClassName,
		 int  nMaxCount 
		 )
{
  WCHAR classnamew[126];
  int res;

  if((res = GetClassNameW(hWnd, classnamew, sizeof(classnamew))) == 0)
    return res;

  WideCharToMultiByte(CP_ACP, 0, classnamew, -1, lpClassName, nMaxCount, 0, 0);
		      
  return res;
}

HFONT XCEAPI
XCECreateFontA(int  nHeight,	// logical height of font 
	       int  nWidth,	// logical average character width 
	       int  nEscapement,	// angle of escapement 
	       int  nOrientation,	// base-line orientation angle 
	       int  fnWeight,	// font weight 
	       DWORD  fdwItalic,	// italic attribute flag 
	       DWORD  fdwUnderline,	// underline attribute flag 
	       DWORD  fdwStrikeOut,	// strikeout attribute flag 
	       DWORD  fdwCharSet,	// character set identifier 
	       DWORD  fdwOutputPrecision,	// output precision 
	       DWORD  fdwClipPrecision,	// clipping precision 
	       DWORD  fdwQuality,	// output quality 
	       DWORD  fdwPitchAndFamily,	// pitch and family 
	       LPCTSTR  lpszFace 	// address of typeface name string 
	       )
{
  LOGFONTA lf;

  lf.lfHeight = nHeight;
  lf.lfWidth = nWidth;
  lf.lfEscapement = nEscapement;
  lf.lfOrientation = nOrientation;
  lf.lfWeight = fnWeight;
  lf.lfItalic = fdwItalic;
  lf.lfUnderline = fdwUnderline;
  lf.lfStrikeOut = fdwStrikeOut;
  lf.lfCharSet = fdwCharSet;
  lf.lfOutPrecision = fdwOutputPrecision;
  lf.lfClipPrecision = fdwClipPrecision;
  lf.lfQuality = fdwQuality;
  lf.lfPitchAndFamily = fdwPitchAndFamily;
  strcpy(lf.lfFaceName, lpszFace);

  return XCECreateFontIndirectA(&lf);
}
