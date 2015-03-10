
#include "stdafx.h"
#include "MCuploader.h"
#define IDD_EMAIL    100101

int window_width = 400;
int window_height = 250;

HWND hwndMain;
RECT rcWindow;
BOOL bSettingsView = FALSE;


HBRUSH hbackground;
HFONT hFontHeader;
HFONT hFontNormal;
HFONT hFontSmall;

VOID Init_Shared()
{
    hbackground = CreateSolidBrush(RGB(250, 218, 90));

    hFontHeader = CreateFont(25, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Times New Roman"));

    hFontNormal = CreateFont(20, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Times New Roman"));

    hFontSmall = CreateFont(15, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Times New Roman"));

}



//
// Login View (login.cpp)
//

BOOL Init_Login(HWND hWnd);
VOID Draw_Login(HWND hWnd, HDC hdc);
VOID Login_Commit();
extern RECT rcLoginEnterButtonFrame;






VOID Draw_Settings(HWND hWnd, HDC hdc)
{

}

RECT rcText;
RECT rcX;
RECT rcScreemshotDir;

VOID Draw_NormalView(HWND hWnd, HDC hdc)
{

    //Draw Email Value
    WCHAR StrBuf[MAX_PATH];
    ZeroMemory(&StrBuf, sizeof(StrBuf));
    swprintf((LPWSTR)&StrBuf, TEXT("email: %s"), Email);
    DrawText(hdc, (LPWSTR)&StrBuf, wcslen(StrBuf), &rcText, DT_TOP | DT_LEFT);

    //Draw [change] area
    int x = rcText.left + 20;
    int y = rcText.top + 20;
    SetRect(&rcX, x, y, x + 63, y + 23);
    ZeroMemory(&StrBuf, sizeof(StrBuf));
    swprintf((LPWSTR)&StrBuf, TEXT("[change]"));
    DrawText(hdc, (LPWSTR)&StrBuf, wcslen(StrBuf), &rcX, DT_TOP | DT_LEFT);

    //Draw Screenshots Directory
    SetRect(&rcScreemshotDir, rcText.left,
        y + 40, rcText.left + wcslen(ScreenshotDirPath) * 10, y + 60);
    DrawText(hdc, ScreenshotDirPath,
        wcslen(ScreenshotDirPath), &rcScreemshotDir, DT_TOP | DT_LEFT);


}

VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);

    //Draw background, save RECT to rcWindow
    GetClientRect(hWnd, &rcWindow);
    FillRect(hdc, &rcWindow, hbackground);

    //Only one view, for now...
    Draw_Login(hWnd, hdc);

}


//
// Handle Messages
//

VOID MouseClick(POINT pt)
{
    if (PtInRect(&rcLoginEnterButtonFrame, pt)) {
        Login_Commit();
    }


}

VOID KeyPressed(HWND hWnd, WPARAM wParam)
{
    if (wParam == VK_ESCAPE) {
        DestroyWindow(hWnd);
    }

}



BOOL MainMenu_HandleWindowMessages(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message) {

    case WM_LBUTTONDOWN:
    {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        MouseClick(pt);
        break;
    }

    case WM_KEYDOWN:
        KeyPressed(hWnd, wParam);
        break;

    }
    return TRUE;
}



//
// Initialization
//

BOOL InitializeMainWindow(HWND hWnd)
{
    hwndMain = hWnd;
    
    if (!InitDataFile()) {
        return FALSE;
    }

    GetEmail();


    Init_Shared();
    Init_Login(hWnd);

    
    return TRUE;
}

