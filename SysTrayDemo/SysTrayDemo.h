#pragma once

#include "resource.h"

//Code sample from http://www.codeproject.com/Articles/18783/Example-of-a-SysTray-App-in-Win


//MainWindow.cpp
BOOL InitializeMainWindow(HINSTANCE hInst);
VOID OpenMainWindow(HWND hwnd);
VOID CloseMainWindow();
VOID DrawMainWindow(HWND hwnd, HDC hdc);



