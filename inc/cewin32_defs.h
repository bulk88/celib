#ifndef CEWIN32_DEFS_H
#define CEWIN32_DEFS_H 1

#define AdjustWindowRect XCEAdjustWindowRect
#define AppendMenuA XCEAppendMenuA
#define AttachThreadInput XCEAttachThreadInput
#define CloseHandle XCECloseHandle
#define CopyFileA XCECopyFileA
#define MoveFileA XCEMoveFileA
#define CreateEventA XCECreateEventA
#define CreateFileA XCECreateFileA
#define CreateFileForMappingA XCECreateFileForMappingA
#define CreateFileMappingA XCECreateFileMappingA
#define CreateFontIndirectA XCECreateFontIndirectA
#define CreateWindowExA XCECreateWindowExA
#undef CreateWindowA
#define CreateWindowA XCECreateWindowA
#define DeleteFileA XCEDeleteFileA
#define DispatchMessageA XCEDispatchMessageA
#define ExtTextOutA XCEExtTextOutA
#define FindFirstFileA XCEFindFirstFileA
#define FindNextFileA XCEFindNextFileA
#define FindWindowA XCEFindWindowA
#define GetCharABCWidths XCEGetCharABCWidthsA
#define GetCharABCWidthsA XCEGetCharABCWidthsA
#define GetCharABCWidthsW XCEGetCharABCWidthsW
#define GetComputerNameA XCEGetComputerNameA
#define GetCurrentDirectory XCEGetCurrentDirectoryA
#define GetCurrentDirectoryA XCEGetCurrentDirectoryA
#define GetEnvironmentVariable XCEGetEnvironmentVariableA
#define GetEnvironmentVariableA XCEGetEnvironmentVariableA
#define GetEnvironmentVariableFromReg XCEGetEnvironmentVariableFromRegA
#define GetEnvironmentVariableFromRegA XCEGetEnvironmentVariableFromRegA
#define GetFileAttributesA XCEGetFileAttributesA
#define SetFileAttributesA XCESetFileAttributesA
#define GetMessageA XCEGetMessageA
#define GetModuleFileNameA XCEGetModuleFileNameA
#define GetModuleHandleA XCEGetModuleHandleA
#define GetObjectA XCEGetObjectA
#undef GetProcAddress
#define GetProcAddress XCEGetProcAddressA
#define GetProcAddressA XCEGetProcAddressA
#define GetScrollPos XCEGetScrollPos
#define GetTextExtentExPointA XCEGetTextExtentExPointA
#define GetTextMetricsA XCEGetTextMetricsA
#define GetUserNameA XCEGetUserNameA
#define GetVersionExA XCEGetVersionExA
#define LoadLibraryA XCELoadLibraryA
#define MapVirtualKeyA XCEMapVirtualKeyA
#define MessageBoxA XCEMessageBoxA
#define PeekMessageA XCEPeekMessageA
#define PostMessageA XCEPostMessageA
#define PostThreadMessageA XCEPostThreadMessageA
#define ReadFile XCEReadFile
#define RegEnumValueA XCERegEnumValueA
#define RegOpenKeyExA XCERegOpenKeyExA
#define RegQueryValueExA XCERegQueryValueExA
#define RegSetValueExA XCERegSetValueExA
#define SendMessageA XCESendMessageA
#define SetCurrentDirectory XCESetCurrentDirectoryA
#define SetCurrentDirectoryA XCESetCurrentDirectoryA
#define SetScrollPos XCESetScrollPos
#define SetWindowTextA XCESetWindowTextA
#define WriteFile XCEWriteFile

#define wsprintfA XCEwsprintfA
#define lstrcpyA XCElstrcpyA
#define lstrcpynA XCElstrcpynA
#define lstrcmpA XCElstrcmpA
#define lstrcmpiA XCElstrcmpiA
#define lstrlenA XCElstrlenA

// These are not defined at all under WinCE
#define GetUserName XCEGetUserNameA
#define GetComputerName XCEGetComputerNameA
#define SetConsoleMode XCESetConsoleMode
#define GetStdHandle XCEGetStdHandle
#define SetStdHandle XCESetStdHandle
#define SetErrorMode XCESetErrorMode
#define GetFileType XCEGetFileType
#define ExitProcess XCEExitProcess

#define IsIconic XCEIsIconic
#define IsZoomed XCEIsZoomed

#define TextOut XCETextOutA
#define TextOutA XCETextOutA
#define TextOutW XCETextOutW
#define GetCharWidth XCEGetCharWidthA
#define GetCharWidthA XCEGetCharWidthA
#define GetCharWidthW XCEGetCharWidthW
#define GetFontData XCEGetFontData
#define GetTextCharset XCEGetTextCharset
#define GetVersion XCEGetVersion
#define ExtCreatePen XCEExtCreatePen

#define OutputDebugStringA XCEOutputDebugStringA

#define LoadCursorA XCELoadCursorA
#define LoadIconA XCELoadIconA
#define FindResourceA XCEFindResourceA

#define GetDlgItemTextA XCEGetDlgItemTextA
#define SetDlgItemTextA XCESetDlgItemTextA
#define GetWindowLongA XCEGetWindowLongA
#define SetWindowLongA XCESetWindowLongA

#define RegisterClassA XCERegisterClassA
#define UnregisterClassA XCEUnregisterClassA

#define MulDiv XCEMulDiv

#define DefWindowProcA XCEDefWindowProcA
#define SendDlgItemMessageA XCESendDlgItemMessageA
#define CallWindowProcA XCECallWindowProcA

#define ChooseColor XCEChooseColor
#define GetOpenFileNameA XCEGetOpenFileNameA
#define GetSaveFileNameA XCEGetSaveFileNameA

#define CharUpperA XCECharUpperA
#define CharLowerA XCECharLowerA

#define GetTextFaceA XCEGetTextFaceA
#define CreateDCA XCECreateDCA
#define InsertMenuA XCEInsertMenuA
#define GetWindowPlacement XCEGetWindowPlacement

#define MoveWindow XCEMoveWindow
#define GetWindowTextA XCEGetWindowTextA

#if UNDER_CE < 300
#define CopyMemory XCECopyMemory
#endif

#define ToAscii XCEToAscii
#define VkKeyScan XCEVkKeyScan
#define EnumFontFamiliesA XCEEnumFontFamilies

#define GetDesktopWindow XCEGetDesktopWindow
#define GetFullPathName XCEGetFullPathNameA

#define StretchDIBits XCEStretchDIBits
#define FillRect XCEFillRect

#undef FormatMessage
#define FormatMessage XCEFormatMessageA
#define FormatMessageA XCEFormatMessageA
#undef CreateMutex
#define CreateMutex XCECreateMutexA
#define CreateMutexA XCECreateMutexA
#undef CreateProcess
#define CreateProcess XCECreateProcessA
#define CreateProcessA XCECreateProcessA

#define DuplicateHandle XCEDuplicateHandle
#define CreatePipe XCECreatePipe
#define GetFileType XCEGetFileType

#define GetEnvironmentStrings XCEGetEnvironmentStrings
#define FreeEnvironmentStrings XCEFreeEnvironmentStrings

#undef DialogBoxIndirectParam
#define DialogBoxIndirectParam XCEDialogBoxIndirectParamA
#define DialogBoxIndirectParamA XCEDialogBoxIndirectParamA

#define PeekNamedPipe XCEPeekNamedPipe

// these are define in WINCE300
#undef ZeroMemory
#define ZeroMemory XCEZeroMemory
#undef GetCommandLine
#define GetCommandLine XCEGetCommandLineA
#define GetCommandLineA XCEGetCommandLineA

#undef RegCreateKeyEx
#define RegCreateKeyEx XCERegCreateKeyExA
#define RegCreateKeyExA XCERegCreateKeyExA
#undef RegDeleteKey
#define RegDeleteKey XCERegDeleteKeyA
#define RegDeleteKeyA XCERegDeleteKeyA
#undef RegEnumKeyEx
#define RegEnumKeyEx XCERegEnumKeyExA
#define RegEnumKeyExA XCERegEnumKeyExA

#define RegCreateKey XCERegCreateKeyA
#define RegCreateKeyA XCERegCreateKeyA
#define RegOpenKey XCERegOpenKeyA
#define RegOpenKeyA XCERegOpenKeyA
#define RegEnumKey XCERegEnumKeyA
#define RegEnumKeyA XCERegEnumKeyA

#undef GetTempFileName
#define GetTempFileName XCEGetTempFileNameA
#define GetTempFileNameA XCEGetTempFileNameA
#define GetTempPathA XCEGetTempPathA

//////////////////////////////////////////////////////////////////////
// Tue Mar 26 2002

#define GetWindowTextLengthA XCEGetWindowTextLengthA
#define GetMenuItemInfoA XCEGetMenuItemInfoA
#define RegisterWindowMessageA XCERegisterWindowMessageA

//////////////////////////////////////////////////////////////////////
// Wed Apr 24 2002

#define SetClassLongA XCESetClassLongA
#define GetClassNameA XCEGetClassNameA
#define CreateFont XCECreateFontA
#define CreateFontA XCECreateFontA

#endif
