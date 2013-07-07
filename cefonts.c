// cefonts.c
//
// Time-stamp: <03/01/02 22:24:35 keuchel@netwave.de>

#include "celib.h"

#if UNDER_CE <= 201
#include <malloc.h>
#endif

HFONT XCEAPI
XCECreateFontIndirectA(CONST LOGFONTA *lplf)
{
  LOGFONTW lfw;
  HFONT hFont;

  lfw.lfHeight = lplf->lfHeight;
  lfw.lfWidth = lplf->lfWidth;
  lfw.lfEscapement = lplf->lfEscapement;
  lfw.lfOrientation = lplf->lfOrientation;
  lfw.lfWeight = lplf->lfWeight;
  lfw.lfItalic = lplf->lfItalic;
  lfw.lfUnderline = lplf->lfUnderline;
  lfw.lfStrikeOut = lplf->lfStrikeOut;
  lfw.lfCharSet = lplf->lfCharSet;
  lfw.lfOutPrecision = lplf->lfOutPrecision;
  lfw.lfClipPrecision = lplf->lfClipPrecision;
  lfw.lfQuality = lplf->lfQuality;
  lfw.lfPitchAndFamily = lplf->lfPitchAndFamily;

  MultiByteToWideChar(CP_ACP, 0, 
		      lplf->lfFaceName, -1, 
		      lfw.lfFaceName, COUNTOF(lfw.lfFaceName));

  hFont = CreateFontIndirectW(&lfw);

  return hFont;
}

typedef struct _MyEnumFontFamArg
{
  FONTENUMPROC fn;
  LPARAM lParam;
} MYENUMFONTFAMARG;

int CALLBACK
MyEnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm,
		  int fonttype, LPARAM lParam)
{
  MYENUMFONTFAMARG *parg = (MYENUMFONTFAMARG *) lParam;
  FONTENUMPROC fn = parg->fn;
  ENUMLOGFONT elfa;
  NEWTEXTMETRICA tma;
  LOGFONTA *plfa;
  LOGFONTW *plfw;

  plfa = &elfa.elfLogFont;
  plfw = &lpelf->elfLogFont;

  memcpy(plfa, plfw, sizeof(LOGFONTA));

  WideCharToMultiByte(CP_ACP, 0, 
		      plfw->lfFaceName, -1, 
		      plfa->lfFaceName, 
		      LF_FACESIZE,
		      NULL, NULL);

  WideCharToMultiByte(CP_ACP, 0, 
		      lpelf->elfFullName, -1, 
		      elfa.elfFullName, 
		      LF_FULLFACESIZE,
		      NULL, NULL);

  WideCharToMultiByte(CP_ACP, 0, 
		      lpelf->elfStyle, -1, 
		      elfa.elfStyle, 
		      LF_FACESIZE,
		      NULL, NULL);

  tma.tmHeight = lpntm->tmHeight;
  tma.tmAscent = lpntm->tmAscent;
  tma.tmDescent = lpntm->tmDescent;
  tma.tmInternalLeading = lpntm->tmInternalLeading;
  tma.tmExternalLeading = lpntm->tmExternalLeading;
  tma.tmAveCharWidth = lpntm->tmAveCharWidth;
  tma.tmMaxCharWidth = lpntm->tmMaxCharWidth;
  tma.tmWeight = lpntm->tmWeight;
  tma.tmOverhang = lpntm->tmOverhang;
  tma.tmDigitizedAspectX = lpntm->tmDigitizedAspectX;
  tma.tmDigitizedAspectY = lpntm->tmDigitizedAspectY;
  tma.tmFirstChar = lpntm->tmFirstChar;
  tma.tmLastChar = lpntm->tmLastChar;
  tma.tmDefaultChar = lpntm->tmDefaultChar;
  tma.tmBreakChar = lpntm->tmBreakChar;
  tma.tmItalic = lpntm->tmItalic;
  tma.tmUnderlined = lpntm->tmUnderlined;
  tma.tmStruckOut = lpntm->tmStruckOut;
  tma.tmPitchAndFamily = lpntm->tmPitchAndFamily;
  tma.tmCharSet = lpntm->tmCharSet;

  return (*fn)(&elfa, &tma, fonttype, parg->lParam);
}

int XCEAPI
XCEEnumFontFamilies(HDC hdc, LPCSTR lpszFamily, 
		    FONTENUMPROC lpEnumFontFamProc, LPARAM lParam)
{
  MYENUMFONTFAMARG arg;
  wchar_t *lpszFamilyW = NULL;

  if(lpszFamily != NULL)
    {
      int len = strlen(lpszFamily);

      lpszFamilyW = alloca(len + 1);
      MultiByteToWideChar(CP_ACP, 0, lpszFamily, len, lpszFamilyW, len);
    }

  arg.fn = lpEnumFontFamProc;
  arg.lParam = lParam;

  return EnumFontFamiliesW(hdc, lpszFamilyW, (FONTENUMPROC) MyEnumFontFamProc,
			   (LPARAM) &arg);
}
