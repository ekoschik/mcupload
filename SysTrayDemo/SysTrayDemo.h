#pragma once

#include "resource.h"

#include "Shlobj.h"
#include "Shlwapi.h"
#include "Windowsx.h"
//Code sample from http://www.codeproject.com/Articles/18783/Example-of-a-SysTray-App-in-Win


//MainWindow.cpp
BOOL InitializeMainWindow(HINSTANCE hInst);
VOID OpenMainWindow(HWND hwnd);
VOID CloseMainWindow();
VOID DrawMainWindow(HWND hwnd, HDC hdc);
BOOL MainMenu_HandleWindowMessages(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

//watch.cpp
BOOL StartWatchingDirectory();
BOOL StopWatchingDirectory();
LPCWSTR GetWatchedDirectory();




