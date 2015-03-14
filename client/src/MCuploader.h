#pragma once

#include "resource.h"

#include "Shlobj.h"
#include "Shlwapi.h"
#include "Windowsx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

extern HINSTANCE hInst;
extern HWND hMainWnd;

__inline VOID Error(LPWSTR msg) {
    MessageBox(NULL, msg, _T("Error"), MB_OK);
}

//window.cpp
BOOL InitializeMainWindow(HWND hWnd);
VOID    DrawMainWindow(HWND hwnd, HDC hdc);
VOID MouseClick(POINT pt);
VOID KeyPressed(HWND hWnd, WPARAM wParam);
extern WCHAR Username[MAX_PATH];
extern WCHAR World[MAX_PATH];

extern WCHAR IniFilePath[MAX_PATH];
extern WCHAR ApplicationDirectoryPath[MAX_PATH];
extern int window_width;
extern int window_height;
extern HFONT hFontHeader;
extern HFONT hFontNormal;
extern HFONT hFontSmall;

//watch.cpp
BOOL    StartWatchingDirectory();
BOOL    StopWatchingDirectory();
LPCWSTR GetWatchedDirectory();
VOID    ProcessDirectoryChange();

//upload.cpp
VOID    InitUpload();
BOOL UploadFile(LPCWSTR filepath, LPCWSTR filename, SOCKET Socket);
extern std::vector<std::string> UploadedFilesList;
SOCKET GetSocket();

//uploadlists.cpp
VOID    InitUploadLists();
BOOL    IsFilenameInIgnoreList(LPCWSTR filename);
BOOL    IsFilenameInSuccessList(LPCWSTR filename);
BOOL    IsFilenameInFailedList(LPCWSTR filename);
VOID    AddFileToPendingList(LPCWSTR filename);
VOID    AddFileToSuccessList(LPCWSTR filename);
VOID    AddFileToFailedList(LPCWSTR filename);
VOID    AddFileToIgnoreList(LPCWSTR filename);
int GetNumFailed();
int GetNumSuccess();

//dataini.cpp
BOOL    GetScreenshotsDirectoryPath();
BOOL    InitDataFile();
BOOL    GetKey(LPCWSTR key, LPWSTR out);
BOOL    SetKey(LPCWSTR key, LPCWSTR val);
VOID    ResetDataIni();
VOID    OpenScreenshotsDirectory();
std::string ToStr(LPCWSTR in);
extern BOOL     bUsernameSet;
extern WCHAR    IniFilePath[MAX_PATH];
extern WCHAR    ApplicationDirectoryPath[MAX_PATH];
extern WCHAR    ScreenshotDirPath[MAX_PATH];

//base64.cpp
std::string base64_encode(
    unsigned char const* bytes_to_encode, 
    unsigned int in_len);

__inline std::string ToStr(LPCWSTR in)
{
#define RandomBufSize 1000
    CHAR buf[RandomBufSize];
    ZeroMemory(&buf, RandomBufSize);
    wcstombs((char*)&buf, in, wcslen(in));
    std::string out = buf;
    return out;
}
