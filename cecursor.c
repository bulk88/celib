// cecursor.c
//
// Time-stamp: <10/08/01 21:49:59 keuchel@w2k>

#include "celib.h"

// WINCE200:winuser.h
// #define IDC_ARROW           MAKEINTRESOURCE(32512) // emulation only!?
// #define IDC_WAIT            MAKEINTRESOURCE(32514)

// WINCE211:winuser.h
// #define IDC_ARROW           MAKEINTRESOURCE(32512)
// #define IDC_WAIT            MAKEINTRESOURCE(32514)

void
XCEShowWaitCursor()
{
  HCURSOR hCursor;

  hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(32514));

  SetCursor(hCursor);
}

void
XCEShowNormalCursor()
{
  HCURSOR hCursor;

  hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(32512));

  SetCursor(hCursor);
}

