#pragma once

#include "resource.h"

#include "Shlobj.h"
#include "Shlwapi.h"
#include "Windowsx.h"

extern HINSTANCE hInst;

//window.cpp
BOOL    InitializeMainWindow();
VOID    OpenMainWindow(HWND hwnd);
VOID    CloseMainWindow();
VOID    DrawMainWindow(HWND hwnd, HDC hdc);
BOOL    MainMenu_HandleWindowMessages(
            HWND, UINT, WPARAM, LPARAM);
extern WCHAR IniFilePath[MAX_PATH];
extern WCHAR ApplicationDirectoryPath[MAX_PATH];

//watch.cpp
BOOL    StartWatchingDirectory();
BOOL    StopWatchingDirectory();
LPCWSTR GetWatchedDirectory();

//upload.cpp
VOID    InitUpload();
VOID    UploadFile(LPCWSTR filepath);

//files.cpp
BOOL    GetScreenshotsDirectoryPath();
BOOL    InitDataFile();
BOOL    GetEmail();
VOID    SetEmail(LPWSTR email);
extern WCHAR    Email[MAX_PATH];
extern BOOL     bEmailSet;
extern WCHAR    IniFilePath[MAX_PATH];
extern WCHAR    ApplicationDirectoryPath[MAX_PATH];
extern WCHAR    ScreenshotDirPath[MAX_PATH];


