// Time-stamp: <10/05/02 09:27:31 keuchel@netwave.de>

#ifndef CEWIN32_H
#define CEWIN32_H 1

#ifndef _XSERVER

#include "celib_defs.h"

#if UNDER_CE < 300
#define GlobalLock LocalLock
#define GlobalUnlock LocalUnlock
#define GlobalAlloc LocalAlloc
#define GlobalFree LocalFree
#define GlobalHandle LocalHandle
#define GlobalSize LocalSize

#define HGLOBAL HLOCAL
#define GHND LHND

#define GMEM_MOVEABLE LMEM_MOVEABLE
#define GMEM_ZEROINIT LMEM_ZEROINIT
#define GMEM_DDESHARE 0
#endif

#define MF_DISABLED         0

#define CS_CLASSDC          0

#if UNDER_CE < 300
#define WS_THICKFRAME       0
#endif

#if UNDER_CE <= 201
#define WS_MAXIMIZEBOX 0
#define WS_MINIMIZEBOX 0
#define WS_SIZEBOX     0
#define SW_SHOWMAXIMIZED SW_SHOW
#endif

#define WS_TILED            WS_OVERLAPPED

#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW \
WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|\
WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX
#endif

#define STD_INPUT_HANDLE  0
#define STD_OUTPUT_HANDLE 1
#define STD_ERROR_HANDLE  2

#define ENABLE_ECHO_INPUT         0x1
#define ENABLE_LINE_INPUT         0x2
#define ENABLE_PROCESSED_INPUT    0x4

#define ENABLE_PROCESSED_OUTPUT	  0x10
#define ENABLE_WRAP_AT_EOL_OUTPUT 0x20

#define SEM_FAILCRITICALERRORS     1
#define SEM_NOALIGNMENTFAULTEXCEPT 2
#define SEM_NOGPFAULTERRORBOX      4
#define SEM_NOOPENFILEERRORBOX	   8

#define FILE_TYPE_UNKNOWN          1
#define FILE_TYPE_DISK	           2
#define FILE_TYPE_CHAR	           3
#define FILE_TYPE_PIPE             4

#define MB_TASKMODAL               0
#define MB_SYSTEMMODAL             0

#ifndef SBS_RIGHTALIGN
#define SBS_RIGHTALIGN             0
#define SBS_BOTTOMALIGN            0
#endif

#define SM_CXHTHUMB                SM_CXHSCROLL
#define SM_CYVTHUMB                SM_CXVSCROLL

#define WS_EX_NOPARENTNOTIFY       0

#define MAKEPOINTS(l)       (*((POINTS FAR *)&(l)))

#define FONTENUMPROCW FONTENUMPROC

// Not defined, but seems to be sent on Jornada680!
#define WM_NCACTIVATE              0x0086

//////////////////////////////////////////////////////////////////////
// Process flags
#define STARTF_USESHOWWINDOW    0x00000001
#define STARTF_USESIZE          0x00000002
#define STARTF_USEPOSITION      0x00000004
#define STARTF_USECOUNTCHARS    0x00000008
#define STARTF_USEFILLATTRIBUTE 0x00000010
#define STARTF_RUNFULLSCREEN    0x00000020
#define STARTF_FORCEONFEEDBACK  0x00000040
#define STARTF_FORCEOFFFEEDBACK 0x00000080
#define STARTF_USESTDHANDLES    0x00000100

//////////////////////////////////////////////////////////////////////

// gdi, needed for emacs

XCE_BEGIN_DECLS

typedef struct _ABC {
    int     abcA;
    UINT    abcB;
    int     abcC;
} ABC, *PABC, NEAR *NPABC, FAR *LPABC;

typedef struct _WINDOWPLACEMENT {
    UINT  length;
    UINT  flags;
    UINT  showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT  rcNormalPosition;
} WINDOWPLACEMENT;
typedef WINDOWPLACEMENT *PWINDOWPLACEMENT, *LPWINDOWPLACEMENT;

typedef struct _MINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;

#if UNDER_CE <= 201
typedef struct _OSVERSIONINFOA{
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR szCSDVersion[ 128 ];
} OSVERSIONINFOA, *LPOSVERSIONINFOA;

typedef struct _OSVERSIONINFOW{
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR szCSDVersion[ 128 ];
} OSVERSIONINFOW, *LPOSVERSIONINFOW;
#endif

//////////////////////////////////////////////////////////////////////

XCE_EXPORT BOOL XCEAPI XCEAdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
XCE_EXPORT BOOL XCEAPI XCEAppendMenuA(HMENU hMenu, UINT uFlags, UINT uIDNewItem, const char *s);
XCE_EXPORT BOOL XCEAPI XCEAttachThreadInput(DWORD idAttach, DWORD idAttachTo, BOOL fAttach);
XCE_EXPORT BOOL XCEAPI XCECopyFileA(const char *lpName1, const char *lpName2, BOOL);
XCE_EXPORT BOOL XCEAPI XCECopyFileW(const wchar_t *lpName1, const wchar_t *lpName2, BOOL);
XCE_EXPORT BOOL XCEAPI XCECreateDirectoryA(const char *lpName, LPSECURITY_ATTRIBUTES);
XCE_EXPORT BOOL XCEAPI XCECreateDirectoryW(const wchar_t *lpName, LPSECURITY_ATTRIBUTES);
XCE_EXPORT HANDLE XCEAPI XCECreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const char *lpName);
XCE_EXPORT HANDLE XCEAPI XCECreateFileA(LPCSTR fname, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
XCE_EXPORT HANDLE XCEAPI XCECreateFileForMappingA(LPCSTR fname, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile); 
XCE_EXPORT HANDLE XCEAPI XCECreateFileForMappingW(LPCWSTR fname, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
XCE_EXPORT HANDLE XCEAPI XCECreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, const char *lpName);
XCE_EXPORT HANDLE XCEAPI XCECreateFileW(LPCWSTR wfname, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
XCE_EXPORT HFONT XCEAPI XCECreateFontIndirectA(CONST LOGFONT *lplf);

XCE_EXPORT HWND XCEAPI XCECreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

XCE_EXPORT HWND XCEAPI XCECreateWindowA(LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

XCE_EXPORT BOOL XCEAPI XCEDeleteFileA(const char *fname);
XCE_EXPORT BOOL XCEAPI XCEDeleteFileW(const wchar_t *fname);
XCE_EXPORT LONG XCEAPI XCEDispatchMessageA(CONST MSG *lpmsg);
XCE_EXPORT BOOL XCEAPI XCEExtTextOutA(HDC hdc, int X, int Y, UINT fuOptions, CONST RECT *lprc, const char *txt, UINT cbCount, CONST INT *lpDx);
XCE_EXPORT HANDLE XCEAPI XCEFindFirstFileA(const char *lpName, LPWIN32_FIND_DATAA lpfd);
XCE_EXPORT HANDLE XCEAPI XCEFindFirstFileW(const wchar_t *oldpath, LPWIN32_FIND_DATAW lpfd);
XCE_EXPORT BOOL XCEAPI XCEFindNextFileA(HANDLE hFind, LPWIN32_FIND_DATAA lpfd);
XCE_EXPORT BOOL XCEAPI XCEFindNextFileW(HANDLE hFind, LPWIN32_FIND_DATAW lpfd);
XCE_EXPORT HWND XCEAPI XCEFindWindowA(const char *classname, const char *title);
XCE_EXPORT BOOL XCEAPI XCEGetCharABCWidthsA(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC);
XCE_EXPORT BOOL XCEAPI XCEGetCharABCWidthsW(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC);
XCE_EXPORT BOOL XCEAPI XCEGetComputerNameA(char *buf, LPDWORD lpdwSize);
XCE_EXPORT DWORD XCEAPI XCEGetCurrentDirectoryA(DWORD dwSize, char *buf);
XCE_EXPORT DWORD XCEAPI XCEGetCurrentDirectoryW(DWORD dwSize, wchar_t *buf);
XCE_EXPORT DWORD XCEGetCurrentRootDirectoryA(DWORD dwSize, char *buf);
XCE_EXPORT DWORD XCEGetCurrentRootDirectoryW(DWORD dwSize, wchar_t *buf);
XCE_EXPORT BOOL XCEAPI XCEGetDiskFreeSpaceExA(const char *path, ULARGE_INTEGER *pl1, ULARGE_INTEGER *pl2, ULARGE_INTEGER *pl3);
XCE_EXPORT int XCEAPI XCEGetEnvironmentVariableA(const char *name, char *buf, int len);
XCE_EXPORT DWORD XCEAPI XCEGetFileAttributesA(const char *fname);
XCE_EXPORT DWORD XCEAPI XCEGetFileAttributesW(const wchar_t *fname);
XCE_EXPORT BOOL XCEAPI XCEGetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInfo);
XCE_EXPORT BOOL XCEAPI XCEGetMessageA(LPMSG lpMsg, HWND hWnd, UINT, UINT);
XCE_EXPORT DWORD XCEAPI XCEGetModuleFileNameA(HMODULE hModule, LPSTR lpName, DWORD nSize);
XCE_EXPORT HMODULE XCEAPI XCEGetModuleHandleA(const char *lpName);
XCE_EXPORT int XCEAPI XCEGetObjectA(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject);
XCE_EXPORT FARPROC XCEAPI XCEGetProcAddressA(HMODULE hMod, const char *name);
XCE_EXPORT int XCEAPI XCEGetScrollPos(HWND hWnd, int nBar);
XCE_EXPORT BOOL XCEAPI XCEGetTextExtentExPointA(HDC hdc, const char *s, int cchString, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize);
XCE_EXPORT BOOL XCEAPI XCEGetTextMetricsA(HDC hdc, LPTEXTMETRIC lptm);
XCE_EXPORT BOOL XCEAPI XCEGetUserNameA(char *buf, LPDWORD lpdwSize);
XCE_EXPORT BOOL XCEAPI XCEGetVersionExA(LPOSVERSIONINFOA lpv);
XCE_EXPORT HMODULE XCEAPI XCELoadLibraryA(const char *name);
XCE_EXPORT UINT XCEAPI XCEMapVirtualKeyA(UINT uCode, UINT uMapType);
XCE_EXPORT int XCEAPI XCEMessageBoxA(HWND hwnd, const char *txt, const char *caption, UINT);
XCE_EXPORT BOOL XCEAPI XCEMoveFileA(const char *lpName1, const char *lpName2);
XCE_EXPORT BOOL XCEAPI XCEMoveFileW(const wchar_t *lpName1, const wchar_t *lpName2);
XCE_EXPORT BOOL XCEAPI XCEPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT, UINT, UINT wRemoveMsg);
XCE_EXPORT LRESULT XCEAPI XCEPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
XCE_EXPORT BOOL XCEAPI XCEPostThreadMessageA(DWORD idThread, UINT Msg, WPARAM, LPARAM);
XCE_EXPORT LONG XCEAPI XCERegEnumValueA(HKEY hKey, DWORD dwIndex, char *lpValueName, LPDWORD lpcbValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
XCE_EXPORT LONG XCEAPI XCERegOpenKeyExA(HKEY hKey, const char *subkey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
XCE_EXPORT LONG XCEAPI XCERegQueryValueExA(HKEY hKey, const char *valname, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
XCE_EXPORT LONG XCEAPI XCERegSetValueExA(HKEY hKey, const char *valname, DWORD dwReserved, DWORD dwType, LPBYTE lpData, DWORD dwSize);
XCE_EXPORT BOOL XCEAPI XCERemoveDirectoryA(const char *lpName);
XCE_EXPORT BOOL XCEAPI XCERemoveDirectoryW(const wchar_t *lpName);
XCE_EXPORT int XCEAPI XCESearchPathA(const char *pathlist, const char *file, char *retpath);
XCE_EXPORT LRESULT XCEAPI XCESendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
XCE_EXPORT BOOL XCEAPI XCESetCurrentDirectoryA(const char *dir);
XCE_EXPORT BOOL XCEAPI XCESetCurrentDirectoryW(const wchar_t *wdir);
XCE_EXPORT BOOL XCESetCurrentRootDirectoryA(const char *dir);
XCE_EXPORT BOOL XCESetCurrentRootDirectoryW(const wchar_t *wdir);
XCE_EXPORT BOOL XCEAPI XCESetFileAttributesA(const char *fname, DWORD dwAttr);
XCE_EXPORT BOOL XCEAPI XCESetFileAttributesW(const wchar_t *fname, DWORD dwAttr);
XCE_EXPORT int XCEAPI XCESetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw);
XCE_EXPORT BOOL XCEAPI XCESetWindowTextA(HWND hWnd, const char *s);
XCE_EXPORT int XCEAPI XCEwsprintfA(char *buf, const char *fmt, ...);
XCE_EXPORT int XCEAPI XCElstrcmpA(const char *s1, const char *s2);
XCE_EXPORT int XCEAPI XCElstrcmpiA(const char *s1, const char *s2);
XCE_EXPORT char * XCEAPI XCElstrcpyA(char *d, const char *s);
XCE_EXPORT char * XCEAPI XCElstrcpynA(char *d, const char *s, int n);
XCE_EXPORT int XCEAPI XCElstrlenA(const char *s);
XCE_EXPORT BOOL XCEAPI XCEBuildCommDCBA(LPCSTR lpDef, LPVOID lpDCB);
XCE_EXPORT LPSTR XCEAPI XCECharLowerA(LPSTR lpsz);
XCE_EXPORT LPSTR XCEAPI XCECharUpperA(LPSTR lpsz);
XCE_EXPORT DWORD XCEAPI XCEGetFullPathNameA(LPCSTR  lpFileName, DWORD nBufferLength, LPSTR  lpBuffer, LPSTR  *lpFilePart);
XCE_EXPORT DWORD XCEAPI XCEGetShortPathNameA(LPCSTR lpszLongPath, LPSTR lpszShortPath, DWORD cchBuffer);
XCE_EXPORT UINT XCEAPI XCEGetTempFileNameA(LPCSTR lpPathName, LPCSTR lpPrefixString, UINT uUnique, LPSTR lpTempFileName);
XCE_EXPORT DWORD XCEAPI XCEGetTempPathA(DWORD size, LPSTR lpBuffer);
XCE_EXPORT BOOL XCEAPI XCEGetVolumeInformationA(LPCSTR  lpRootPathName, LPSTR  lpVolumeNameBuffer, DWORD  nVolumeNameSize, LPDWORD  lpVolumeSerialNumber, LPDWORD  lpMaximumComponentLength, LPDWORD  lpFileSystemFlags, LPSTR  lpFileSystemNameBuffer, DWORD  nFileSystemNameSize);
XCE_EXPORT UINT XCEAPI XCESetErrorMode(UINT mode);
XCE_EXPORT VOID XCEAPI XCEExitProcess(UINT code);
XCE_EXPORT BOOL XCEAPI XCEIsIconic(HWND hwnd);
XCE_EXPORT BOOL XCEAPI XCEIsZoomed(HWND hwnd);
XCE_EXPORT BOOL XCEAPI XCEGetTextExtentPoint32A(HDC  hdc, LPCSTR  lpString, int  cbString, LPSIZE  lpSize);
XCE_EXPORT BOOL XCEAPI XCEGetTextExtentPoint32W(HDC  hdc, LPCWSTR  lpString, int  cbString, LPSIZE  lpSize);
XCE_EXPORT BOOL XCEAPI XCETextOutA(HDC  hdc, int  nXStart, int  nYStart, LPCSTR  lpString, int  cbString);
XCE_EXPORT BOOL XCEAPI XCETextOutW(HDC  hdc, int  nXStart, int  nYStart, LPCWSTR  lpString, int  cbString);
XCE_EXPORT BOOL XCEAPI XCEGetCharWidthA(HDC  hdc, UINT  iFirstChar, UINT  iLastChar, LPINT  lpBuffer);
XCE_EXPORT BOOL XCEAPI XCEGetCharWidthW(HDC  hdc, UINT  iFirstChar, UINT  iLastChar, LPINT  lpBuffer);
XCE_EXPORT DWORD XCEAPI XCEGetFontData(HDC  hdc, DWORD  dwTable, DWORD  dwOffset, LPVOID  lpvBuffer, DWORD  cbData);
XCE_EXPORT UINT XCEAPI XCEGetTextCharset(HDC hdc);
XCE_EXPORT DWORD XCEAPI XCEGetVersion(VOID);
XCE_EXPORT HPEN XCEAPI XCEExtCreatePen(DWORD dwPenStyle, DWORD dwWidth, CONST LOGBRUSH *lplb, DWORD dwStyleCount, CONST DWORD *lpStyle);
XCE_EXPORT VOID XCEAPI XCEOutputDebugStringA(LPCSTR s);
XCE_EXPORT HCURSOR XCEAPI XCELoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName);
XCE_EXPORT HICON XCEAPI XCELoadIconA(HINSTANCE  hInstance, LPCSTR  lpIconName);
XCE_EXPORT HRSRC XCEAPI XCEFindResourceA(HMODULE  hModule, LPCSTR  lpName, LPCSTR  lpType);
XCE_EXPORT UINT XCEAPI XCEGetDlgItemTextA(HWND hDlg, int nIDDlgItem, LPSTR lpString, int nMaxCount);
XCE_EXPORT BOOL XCEAPI XCESetDlgItemTextA(HWND hwndDlg, int idControl, LPCSTR lpsz);
XCE_EXPORT LONG XCEAPI XCEGetWindowLongA(HWND hWnd, int nIndex);
XCE_EXPORT LONG XCEAPI XCESetWindowLongA(HWND hWnd, int nIndex, LONG newLong);
XCE_EXPORT ATOM XCEAPI XCERegisterClassA(const WNDCLASSA *lpwc);
XCE_EXPORT BOOL XCEAPI XCEUnregisterClassA(LPCSTR lpszClassName, HINSTANCE  hinst);
XCE_EXPORT int XCEAPI XCEMulDiv(int nNumber, int nNumerator, int nDenominator);
XCE_EXPORT LRESULT XCEAPI XCEDefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
XCE_EXPORT LONG XCEAPI XCESendDlgItemMessageA(HWND hwndDlg, int idControl, UINT uMsg, WPARAM wParam, LPARAM lParam);
XCE_EXPORT LRESULT XCEAPI XCECallWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
XCE_EXPORT BOOL XCEAPI XCEChooseColor(LPVOID);
// Why was this defined-out???
#if 1 //UNDER_CE > 200
XCE_EXPORT BOOL XCEAPI XCEGetOpenFileNameA(void *lpofn);
XCE_EXPORT BOOL XCEAPI XCEGetSaveFileNameA(void *lpofn);
#endif
XCE_EXPORT int XCEAPI XCEGetTextFaceA(HDC hdc, int nCount, LPSTR lpFaceName);
XCE_EXPORT HDC XCEAPI XCECreateDCA(LPCSTR lpszDriver, LPCSTR lpszDevice, LPCSTR lpszOutput, CONST DEVMODE *lpInitData);
XCE_EXPORT BOOL XCEAPI XCEInsertMenuA(HMENU hMenu, UINT uPosition, UINT uFlags, UINT uIDNewItem, LPCSTR lpNewItem);
XCE_EXPORT BOOL XCEAPI XCEGetWindowPlacement(HWND hWnd, WINDOWPLACEMENT *lpwndpl);
XCE_EXPORT void XCEAPI XCEGetSystemTimeAsFileTime(LPFILETIME pft);

XCE_EXPORT BOOL XCEAPI XCEMoveWindow(HWND hwnd, UINT x, UINT y, UINT w, UINT h, BOOL repaint);

XCE_EXPORT BOOL XCEAPI XCEGetWindowTextA(HWND hWnd, char *lpString, int nMaxCount);

XCE_EXPORT VOID XCEAPI XCECopyMemory(PVOID dest, CONST PVOID src, DWORD len);

//////////////////////////////////////////////////////////////////////

XCE_EXPORT int XCEAPI XCEToAscii(UINT uVirtKey, UINT uScanCode, 
				 PBYTE lpbKeyState,
				 LPWORD lpwTransKey, UINT fuState);

XCE_EXPORT SHORT XCEAPI XCEVkKeyScan(unsigned char c);

XCE_EXPORT int XCEAPI XCEEnumFontFamilies(HDC hdc, LPCSTR lpszFamily, 
			       FONTENUMPROC lpEnumFontFamProc, LPARAM lParam);

//////////////////////////////////////////////////////////////////////
// util funcs...

XCE_EXPORT int XCEGetRegStringA(HKEY hKey, const char *keyname, 
				const char *valname, 
				char *buf, int len);

XCE_EXPORT BOOL XCESetRegStringA(HKEY hKey, const char *keyname, 
				 const char *valname, 
				 const char *value);

//////////////////////////////////////////////////////////////////////

XCE_EXPORT wchar_t * XCEAPI XCEMakeUnicode(const char *s, int len);

//////////////////////////////////////////////////////////////////////

XCE_EXPORT HWND XCEAPI XCEGetDesktopWindow();

//////////////////////////////////////////////////////////////////////
// Sun May 27 2001

XCE_EXPORT int XCEAPI XCEStretchDIBits(
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
    );

XCE_EXPORT int XCEAPI XCEFillRect(HDC hdc, CONST RECT *lprc, HBRUSH hbr);

//////////////////////////////////////////////////////////////////////

XCE_EXPORT DWORD XCEFormatMessageA(DWORD dwFlags, LPCVOID lpSource, 
				   DWORD dwMessageId, DWORD dwLangId, 
				   LPSTR lpBuffer, DWORD nSize,
				   va_list *args);

XCE_EXPORT HANDLE XCECreateMutexA(LPSECURITY_ATTRIBUTES lpsa, 
				  BOOL bInitialOwner, LPCSTR name);

XCE_EXPORT BOOL XCECreateProcessA(LPCSTR exe, LPCSTR cmdline, 
				  LPVOID lpProcessAttributes, 
				  LPVOID lpThreadAttributes, 
				  BOOL bInherit, DWORD dwFlags, 
				  LPVOID lpEnv, LPCSTR lpCurDir, 
				  LPSTARTUPINFO psi, 
				  LPPROCESS_INFORMATION ppi);

XCE_EXPORT LPVOID XCEGetEnvironmentStrings(VOID);
XCE_EXPORT BOOL XCEFreeEnvironmentStrings(LPCSTR buf);

//////////////////////////////////////////////////////////////////////

XCE_EXPORT int 
XCEDialogBoxIndirectParamA(HINSTANCE  hInstance,
			   LPCDLGTEMPLATE  hDialogTemplate,
			   HWND  hWndParent,
			   DLGPROC  lpDialogFunc,
			   LPARAM  dwInitParam);

//////////////////////////////////////////////////////////////////////

XCE_EXPORT BOOL 
XCESetConsoleMode(HANDLE handle, DWORD mode);
XCE_EXPORT BOOL 
XCESetStdHandle(DWORD type, HANDLE handle);
XCE_EXPORT HANDLE 
XCEGetStdHandle(DWORD type);
XCE_EXPORT BOOL 
XCEReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, 
LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
XCE_EXPORT BOOL 
XCEWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, 
	     LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
XCE_EXPORT BOOL 
XCECloseHandle(HANDLE h);
XCE_EXPORT BOOL 
XCEDuplicateHandle(HANDLE hProc1, HANDLE hFile1, 
		   HANDLE hProc2, HANDLE *phFile2,
		   DWORD dwAccess, BOOL bInherit, 
		   DWORD dwOpts
		   );
XCE_EXPORT BOOL 
XCECreatePipe(PHANDLE hRead, PHANDLE hWrite, 
	      LPSECURITY_ATTRIBUTES lpsa,
	      DWORD nSize);

XCE_EXPORT DWORD
XCEGetFileType(HANDLE hFile);

XCE_EXPORT BOOL 
XCEPeekNamedPipe(HANDLE   hPipe, 
		 LPVOID   lpvBuffer,	
		 DWORD    cbBuffer,
		 LPDWORD  lpcbRead,
		 LPDWORD  lpcbAvail,
		 LPDWORD  lpcbMessage);

XCE_EXPORT void
XCEZeroMemory(void *p, unsigned int len);

XCE_EXPORT char *
XCEGetCommandLineA();

XCE_EXPORT DWORD XCEAPI
XCESearchPath2A(LPCSTR  lpszPath,	
		LPCSTR  lpszFile,	
		LPCSTR  lpszExtension, 
		DWORD   cchReturnBuffer, 
		LPSTR   lpszReturnBuffer, 
		LPSTR   *plpszFilePart 
		);

XCE_EXPORT LONG XCEAPI
XCERegCreateKeyExA(HKEY hKey,         
		   const char *subkey,  
		   DWORD dwRes,
		   LPSTR lpszClass,
		   DWORD ulOptions,   
		   REGSAM samDesired,
		   LPSECURITY_ATTRIBUTES sec_att,
		   PHKEY phkResult,
		   DWORD *lpdwDisp
		   );

XCE_EXPORT LONG XCEAPI
XCERegDeleteKeyA(HKEY  hKey,
		 LPCTSTR  lpszSubKey 
		 );

XCE_EXPORT LONG XCEAPI 
XCERegEnumKeyExA(HKEY  hKey,	
		 DWORD  iSubkey,
		 LPSTR  lpszName,
		 LPDWORD  lpcchName,
		 LPDWORD  lpdwReserved,
		 LPSTR  lpszClass,
		 LPDWORD  lpcchClass,
		 PFILETIME  lpftLastWrite
		 );

XCE_EXPORT LONG XCEAPI
XCERegCreateKeyA(HKEY  hKey,
		 LPCTSTR  lpszSubKey,
		 PHKEY  phkResult
		 );

XCE_EXPORT LONG XCEAPI
XCERegOpenKeyA(HKEY  hKey,
	       LPCTSTR  lpszSubKey,
	       PHKEY  phkResult
	       );

XCE_EXPORT LONG XCEAPI 
XCERegEnumKeyA(HKEY  hKey,	
	       DWORD  iSubKey,
	       LPSTR  lpszName,
	       DWORD  cchName
	       );

//////////////////////////////////////////////////////////////////////
// Tue Mar 26 2002

XCE_EXPORT int XCEAPI
XCEGetWindowTextLengthA(HWND hWnd);

XCE_EXPORT BOOL XCEAPI
XCEGetMenuItemInfoA(HMENU hMenu, UINT uItem, BOOL fByPos, 
		    LPMENUITEMINFOA lpmii);

XCE_EXPORT UINT XCEAPI
XCERegisterWindowMessageA(LPCSTR msg);

//////////////////////////////////////////////////////////////////////
// Wed Apr 24 2002

XCE_EXPORT DWORD XCEAPI
XCESetClassLongA(HWND  hwnd,
		 int  nIndex,
		 LONG  lNewVal
		 );

//////////////////////////////////////////////////////////////////////

XCE_EXPORT DWORD XCEAPI
XCESetClassLongA(HWND  hwnd,
		 int  nIndex,
		 LONG  lNewVal
		 );

XCE_EXPORT int XCEAPI
XCEGetClassNameA(HWND  hWnd,	
		 LPTSTR  lpClassName,
		 int  nMaxCount 
		 );

XCE_EXPORT HFONT XCEAPI
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
	       );

XCE_END_DECLS

#endif // _XSERVER

#endif
