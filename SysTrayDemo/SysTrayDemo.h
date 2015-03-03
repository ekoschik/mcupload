#pragma once

#include "resource.h"

#include "Shlobj.h"
#include "Shlwapi.h"
#include "Windowsx.h"

extern HINSTANCE hInst;

//MainWindow.cpp
BOOL InitializeMainWindow(HINSTANCE hInst);
VOID OpenMainWindow(HWND hwnd);
VOID CloseMainWindow();
VOID DrawMainWindow(HWND hwnd, HDC hdc);
extern WCHAR IniFilePath[MAX_PATH];
BOOL MainMenu_HandleWindowMessages(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

//watch.cpp
BOOL StartWatchingDirectory();
BOOL StopWatchingDirectory();
LPCWSTR GetWatchedDirectory();
extern WCHAR ScreenshotDirPath[MAX_PATH];

//upload.cpp
VOID UploadFile(LPCWSTR filepath);


