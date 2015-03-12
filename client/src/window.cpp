
#include "stdafx.h"
#include "MCuploader.h"
#define IDD_EMAIL    100101

int window_width = 400;
int window_height = 250;

HWND hwndMain;
RECT rcWindow;
BOOL bSettingsView = FALSE;

#define LOGINVIEW       (!bUsernameSet)
#define SETTINGSVIEW    (bUsernameSet && bSettingsView)
#define NORMALVIEW      (!LOGINVIEW && !SETTINGSVIEW)


HBRUSH hbackground;
HFONT hFontHeader;
HFONT hFontNormal;
HFONT hFontSmall;

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


//
// Login View (login.cpp)
//

BOOL Init_Login(HWND hWnd);
VOID Draw_Login(HWND hWnd, HDC hdc);
VOID Login_Commit();
VOID HideEditControls();
extern RECT rcLoginEnterButtonFrame;

//
// Main View (mainview.cpp)
//

BOOL Init_MainView(HWND hWnd);
VOID Draw_MainView(HWND hWnd, HDC hdc);
extern RECT rctextSettings;
extern RECT rcScreenshotsDirectoryLink;

//
// Settings View (settings.cpp)
//

BOOL Init_Settings(HWND hWnd);
VOID Draw_Settings(HWND hWnd, HDC hdc);
extern RECT rctextBack;
extern RECT rctextResetButton;


//
// Paint Handler
//

VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);
    HideEditControls();

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
            //Switch to main view
            bSettingsView = FALSE;
            InvalidateRect(hwndMain, NULL, TRUE);
        }
        if (PtInRect(&rctextResetButton, pt)) {
            //Press the reset button
            ResetDataFiles();
        }
    } else {
        if (PtInRect(&rctextSettings, pt)) {
            //Switch to settings view
            bSettingsView = TRUE;
            InvalidateRect(hwndMain, NULL, TRUE);
        }
        if (PtInRect(&rcScreenshotsDirectoryLink, pt)) {
            OpenScreenshotsDirectory();
        }
    }
}

VOID KeyPressed(HWND hWnd, WPARAM wParam)
{
    if (wParam == VK_ESCAPE) {
        DestroyWindow(hWnd);
    }

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

    Init_Shared();
    Init_Login(hWnd);
    Init_Settings(hWnd);
    Init_MainView(hWnd);

    return TRUE;
}

