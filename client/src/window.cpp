
#include "stdafx.h"
#include "MCuploader.h"


int window_width = 400;
int window_height = 250;

RECT rcWindow;

HWND hwndMain;
BOOL bSettingsView = FALSE;
BOOL bSetupView = FALSE;

HBRUSH hbackground;
HFONT hFontHeader;
HFONT hFontNormal;
HFONT hFontSmall;

VOID HideEverything() {
    HideEditControls();
    ShowWindow(hWndListView, SW_HIDE);
}

VOID GoToSettings() {
    HideEverything();
    bSettingsView = TRUE;
    bSetupView = FALSE;
    InvalidateRect(hMainWnd, NULL, TRUE);
}
VOID GoToSetup() {
    HideEverything();
    bSettingsView = FALSE;
    bSetupView = TRUE;
    SetLoginEditControlsFromUD();
    InvalidateRect(hMainWnd, NULL, TRUE);
}
VOID GoToMainView() {
    HideEverything();
    bSettingsView = FALSE;
    bSetupView = FALSE;
    InvalidateRect(hMainWnd, NULL, TRUE);
}

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
    Init_MainView(hWnd);

    return TRUE;
}




//
// Paint Handler
//

VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);



    //Draw background, save RECT to rcWindow
    GetClientRect(hWnd, &rcWindow);
    FillRect(hdc, &rcWindow, hbackground);


    if (LOGINVIEW) {
        Draw_Login(hWnd, hdc);
    } else {
        Draw_MainView(hWnd, hdc);
    }


}




//
// Handle Window Messages
//

BOOL MouseClick(POINT pt, BOOL bHitTest)
{
    BOOL ret = FALSE;
    if (LOGINVIEW) {
        if (PtInRect(&rcLoginEnterButtonFrame, pt)) {
            if (!bHitTest) Login_Commit();
            ret = TRUE;
        }
    } else {
        if (PtInRect(&rcScreenshotsDirectoryLink, pt)) {
            if (!bHitTest) OpenScreenshotsDirectory();
            ret = TRUE;
        }
        if (PtInRect(&rctextViewOnWeb, pt)) {
            if (!bHitTest) OpenWorldPageInBrowser();
            ret = TRUE;
        }
        if (PtInRect(&rcConnectionLight, pt)) {
            if (!bHitTest) TogglePause();
            ret = TRUE;
        }
        if (PtInRect(&rctextChangeName, pt)) {
            if (!bHitTest) GoToSetup();
            ret = TRUE;
        }
        if (PtInRect(&rcSuccessList, pt)) {
            if (!bHitTest) SwitchToSuccessList();
            ret = TRUE;
        }
        if (FailedList.size() >  0) {
            if (PtInRect(&rcFailedList, pt)) {
                if (!bHitTest) SwitchToFailedList();
                ret = TRUE;
            }
            if (PtInRect(&rcRetryAll, pt)) {
                if (!bHitTest) RetryAllFailed();
                ret = TRUE;
            }
            if (PtInRect(&rcIgnoreAll, pt)) {
                if (!bHitTest) IgnoreAllFailed();
                ret = TRUE;
            }
        }
        if (PtInRect(&rcSnap, pt)) {
            if (!bHitTest) ResetLocation();
            ret = TRUE;
        }
    }
    return ret;
}
VOID KeyPressed(HWND hWnd, WPARAM wParam)
{
    if (wParam == VK_ESCAPE) {
        DestroyWindow(hWnd);
    }

}
