#pragma once

#include "resource.h"

//Code sample from http://www.codeproject.com/Articles/18783/Example-of-a-SysTray-App-in-Win


VOID InitMainWindow(BOOL bFirstCreation);
VOID OnClose();
VOID DrawMainWindow(HWND hwnd, HDC hdc);
BOOL MainWindow_WndProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);
