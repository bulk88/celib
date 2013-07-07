// test1w.c
//
// Time-stamp: <07/01/01 16:41:22 keuchel@lightning>

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <commctrl.h>
#include "test1wres.h"

// NOTE: The command bar is a child window
// and obscures the edit window!!!

#define GET_WM_PARENTNOTIFY_MSG(wp, lp)             LOWORD(wp)
#define GET_WM_PARENTNOTIFY_HWNDCHILD(wp, lp)       (HWND)(lp)

#undef BUILD_CELIB
#include "celib.h"

static HINSTANCE hInst; 
static HWND hEdit;
static HWND hwndCmdBar;

static int commandbar_height;

static char *szTitle     = "EditApp";
static char *szAppName   = "EditApp";

static char *szClassName = "EditAppClass";
static wchar_t *szClassNameW = L"EditAppClass";

static char *szIconName  = NULL;

#define IDM_EXIT 0x100
#define WM_START (WM_USER+100)

BOOL InitApplication(HINSTANCE);
HWND InitInstance(HINSTANCE, INT);
void Message(HWND hwnd, const char *fmt, ...);
void ShowText(char *s);

void InitMenu(HWND hwnd);

int APIENTRY 
wWinMain(HINSTANCE hInstance,
	 HINSTANCE hPrevInstance, 
	 wchar_t *lpCmdLine, 
	 int nCmdShow)
{
  MSG msg;
  HWND hwnd;

  if (!hPrevInstance)
    {
      if (!InitApplication(hInstance))
        {
	  return FALSE;  
        }
    }

  if((hwnd = InitInstance(hInstance, nCmdShow)) == NULL)
    {
      return FALSE;
    }

  PostMessage(hwnd, WM_START, 0, 0L);

  while(GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg); 
    }
  return TRUE;
}

LRESULT CALLBACK 
WndProc(HWND hwnd,
	UINT uMessage,
	WPARAM wparam,
	LPARAM lparam)
{
  switch(uMessage)
    {
#if 0
    case WM_KEYDOWN:
      {
	int nVirtKey;
	UINT scancode;

	nVirtKey = (int) wparam;
	scancode = XCEMapVirtualKeyA(nVirtKey, 0);

	XCEShowMessageA("Virtkey %d Scancode %d", nVirtKey, scancode);
      }
      break;
#endif

#if 1
    case WM_CHAR:
      {
	unsigned short charcode = wparam;
	XCEShowMessageA("CharCode: %c 0x%x", charcode, charcode);
      }
      break;
#endif

    case WM_CREATE:
      InitMenu(hwnd);
      break;
    case WM_COMMAND:
      if(lparam != 0) // notification when window handle in lparam...
	{
	  int nmsg = GET_WM_PARENTNOTIFY_MSG(wparam, lparam);
	  HANDLE hwnd = (HWND) GET_WM_PARENTNOTIFY_HWNDCHILD(wparam, lparam);

	  switch(nmsg)
	    {
	    case EN_MAXTEXT:
	      SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM) "");
	    }
	}
      else
	{
	  switch(GET_WM_COMMAND_ID(wparam,lparam))
	    {
	    case IDM_EXIT:
	      DestroyWindow(hwnd);
	      break;
	    }
	}
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_SIZE:
      {
	SetWindowPos(hEdit, NULL, 0, 0, LOWORD(lparam), HIWORD(lparam),
		     SWP_NOMOVE|SWP_NOZORDER);
	return DefWindowProcW(hwnd, uMessage, wparam, lparam);
      }	
    case WM_START:
      // Message(hwnd, "WM_START received");
      break;
    default:
      return DefWindowProcW(hwnd, uMessage, wparam, lparam);
    }
  return 0;
}

BOOL 
InitApplication(HINSTANCE hInstance)
{
  WNDCLASSW wc;

  wc.style         = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc   = (WNDPROC)WndProc;       
  wc.cbClsExtra    = 0;                      
  wc.cbWndExtra    = 0;                      
  wc.hInstance     = hInstance;              
  wc.hIcon         = NULL; 
  wc.hCursor       = NULL;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = szClassNameW;

  if (!RegisterClassW((LPWNDCLASSW)&wc.style))
    return FALSE;

  return TRUE;
}

void
InitMenu(HWND hwnd)
{
  HMENU hMenu;
  HMENU hMenuFile;
  HMENU hMenuFileDrop;
  RECT r;

  hwndCmdBar = CommandBar_Create(hInst, hwnd, 1);

#if 1
  if(CommandBar_InsertMenubar(hwndCmdBar, hInst, IDM_MENU, 0) == FALSE)
    printf("InsertMenuBar: %d\n", GetLastError());

  GetWindowRect(hwndCmdBar, &r);
  commandbar_height = r.bottom - r.top;

  hMenu = CommandBar_GetMenu(hwndCmdBar, 0);

  hMenuFileDrop = CreatePopupMenu();
  AppendMenuW(hMenuFileDrop, MF_STRING, IDM_EXIT, L"&Exit");
  InsertMenuW(hMenu, 0, MF_BYPOSITION|MF_POPUP, (UINT) hMenuFileDrop, L"&File");
  //AppendMenuW(hMenu, MF_POPUP, (UINT) hMenuFileDrop, L"&File");
  AppendMenuW(GetSubMenu(hMenu, 0), MF_STRING, IDM_EXIT, L"&Exit");
#endif

#if 0
  hMenu = CreateMenu();
  hMenuFile = CreateMenu();
  AppendMenuW(hMenuFileDrop, MF_STRING, IDM_EXIT, L"&Exit");
  AppendMenuW(hMenuFile, MF_POPUP, (UINT) hMenuFileDrop, L"&File");

  // Error 120: Not supported on this system...
  if(CommandBar_InsertMenubarEx(hwndCmdBar, hInst, hMenu, 0) == FALSE)
    printf("InsertMenuBarEx: %d\n", GetLastError());
#endif

  // add exit button
  CommandBar_AddAdornments(hwndCmdBar, 0, 0);
}

HWND
InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  HWND hwnd;
  RECT r;

  hInst = hInstance; 

  hwnd = CreateWindow(szClassName, 
		      szTitle,        
		      //WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
		      WS_VISIBLE,
		      CW_USEDEFAULT, 0,   
		      CW_USEDEFAULT, 0,   
		      NULL,               
		      NULL,               
		      hInstance,           
		      NULL);               
    
  if (!hwnd)
    return NULL;

#if 1
  // init edit
  GetClientRect(hwnd, &r);
  hEdit = CreateWindow("Edit", "", 
		       WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_MULTILINE|
		       /*ES_READONLY|*/ES_AUTOVSCROLL|WS_BORDER,
		       r.left, r.top + commandbar_height, 
		       r.right, r.bottom - commandbar_height,
		       hwnd, NULL,
		       hInstance, NULL);
  if(!hEdit)
    {
      Message(NULL, "Cannot create edit");
      return NULL;
    }
#endif

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);        

  return hwnd;
}

void
Message(HWND hwnd, const char *fmt, ...)
{
  va_list ap;
  char buf[1024];

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  MessageBox(hwnd, buf, szAppName, MB_OK);
}

void
ShowText(char *s)
{
  char *buf = (char *)malloc(strlen(s) * 2);
  char *sp, *dp;

  for(sp = s, dp = buf; *sp;)
    {
      if(*sp == '\n')
	*dp++ = '\r';
      *dp++ = *sp++;
    }
  *dp = 0;

  SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM) buf);

  free(buf);
}
