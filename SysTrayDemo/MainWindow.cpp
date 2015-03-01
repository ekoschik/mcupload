
#include "stdafx.h"
#include "SysTrayDemo.h"

HBRUSH hbackground;

VOID InitMainWindow(BOOL bFirstCreation)
{
    if (bFirstCreation) {
        hbackground = CreateSolidBrush(RGB(250, 218, 90));
    }
    
}

VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    RECT rc;
    GetClientRect(hWnd, &rc);
    FillRect(hdc, &rc, hbackground);

    rc.top = rc.bottom / 2 - 5;
    rc.left = rc.right / 2 - 20;

    LPWSTR str = TEXT("Hello World!");
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, str, wcslen(str), &rc, DT_TOP | DT_LEFT);

}

BOOL MainWindow_WndProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message) {



    }

    return TRUE;
}

VOID OnClose()
{

}