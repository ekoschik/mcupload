
#include "stdafx.h"
#include "MCuploader.h"
#define IDD_EMAIL    100101

int window_width = 400;
int window_height = 250;

HWND hwndMain;
RECT rcWindow;
BOOL bSettingsView = FALSE;
BOOL bSetupView = FALSE;

VOID GoToSettings() {
    bSettingsView = TRUE;
    bSetupView = FALSE;
    InvalidateRect(hMainWnd, NULL, TRUE);
}
VOID GoToSetup() {
    bSettingsView = FALSE;
    bSetupView = TRUE;
    SetLoginEditControlsFromUD();
    InvalidateRect(hMainWnd, NULL, TRUE);
}
VOID GoToMainView() {
    bSettingsView = FALSE;
    bSetupView = FALSE;
    InvalidateRect(hMainWnd, NULL, TRUE);
}

HBRUSH hbackground;
HFONT hFontHeader;
HFONT hFontNormal;
HFONT hFontSmall;

VOID OpenWorldPageInBrowser()
{
    WCHAR ScreenshotsWebPage[2000] = { 0 };
    wsprintf((LPWSTR)&ScreenshotsWebPage, L"http://%s:%s/screenshots", 
        UD.servername.c_str(), UD.port.c_str());

    LPWSTR webpage = (LPWSTR)&ScreenshotsWebPage;

    ShellExecute(NULL, _T("open"), webpage,
        NULL, NULL, SW_SHOWNORMAL);
}



//
// Login View (login.cpp)
//

BOOL Init_Login(HWND hWnd);
VOID Draw_Login(HWND hWnd, HDC hdc);
VOID Login_Commit();
VOID HideEditControls();
extern RECT rcLoginEnterButtonFrame;
//extern HWND hLoginUsernameEditControl;
//extern HWND hLoginWorldEditControl;
//
// Main View (mainview.cpp)
//

BOOL Init_MainView(HWND hWnd);
VOID Draw_MainView(HWND hWnd, HDC hdc);
extern RECT rctextSettings;
extern RECT rcScreenshotsDirectoryLink;
extern HWND hWndListView;
extern RECT rctextViewOnWeb;

//
// Settings View (settings.cpp)
//

BOOL Init_Settings(HWND hWnd);
VOID Draw_Settings(HWND hWnd, HDC hdc);
extern RECT rctextBack;
extern RECT rctextResetButton;

//
// Initialization
//

VOID Init_Shared()
{
    hbackground = CreateSolidBrush(RGB(199, 192, 237));

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

BOOL InitializeMainWindow(HWND hWnd)
{
    hwndMain = hWnd;

    if (!InitDataFile()) {
        return FALSE;
    }

    Init_Shared();
    Init_Login(hWnd);
    Init_Settings(hWnd);
    Init_MainView(hWnd);

    return TRUE;
}




//
// Paint Handler
//

VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);

    HideEditControls();
    ShowWindow(hWndListView, SW_HIDE);


    //Draw background, save RECT to rcWindow
    GetClientRect(hWnd, &rcWindow);
    FillRect(hdc, &rcWindow, hbackground);


    if (LOGINVIEW) {
        Draw_Login(hWnd, hdc);
    }
    else if (SETTINGSVIEW) {
        Draw_Settings(hWnd, hdc);
    }
    else {
        Draw_MainView(hWnd, hdc);
    }


}


//
// Handle Window Messages
//

VOID MouseClick(POINT pt)
{
    if (LOGINVIEW) {
        if (PtInRect(&rcLoginEnterButtonFrame, pt)) {
            Login_Commit();
        }
    } else if (SETTINGSVIEW) {
        if (PtInRect(&rctextBack, pt)) {
            GoToMainView();
        }
        if (PtInRect(&rctextResetButton, pt)) {
            GoToSetup();
        }
    } else {
        if (PtInRect(&rctextSettings, pt)) {
            GoToSettings();
        }
        if (PtInRect(&rcScreenshotsDirectoryLink, pt)) {
            OpenScreenshotsDirectory();
        }
        if (PtInRect(&rctextViewOnWeb, pt)) {
            OpenWorldPageInBrowser();
        }
    }
}

VOID KeyPressed(HWND hWnd, WPARAM wParam)
{
    if (wParam == VK_ESCAPE) {
        DestroyWindow(hWnd);
    }

}
