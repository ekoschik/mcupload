
#include "stdafx.h"
#include "MCuploader.h"


int window_width = 350;
int window_height = 225;
RECT rcWindow;
HWND hwndMain;












HBRUSH hbackground;

BOOL bSettingsView = FALSE;
BOOL bSetupView = FALSE;

VOID HideEverything() {
    //HideEditControls();
    //ShowWindow(hWndListView, SW_HIDE);
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
    //SetLoginEditControlsFromUD();
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

}

BOOL InitializeMainWindow(HWND hWnd)
{
    hwndMain = hWnd;
    hbackground = CreateSolidBrush(RGB(199, 192, 237));

    if (!InitDataFile()) {
        return FALSE;
    }

    //Keep this order b/c buttons sets main area rect
    InitButtons();
    InitMainView();
    
    return TRUE;
}




//
// Paint Handler
//

VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);

    
    DrawMainButtons(hWnd, hdc);
    DrawMainView(hWnd, hdc);

}




//
// Handle Window Messages
//

BOOL MouseClick(POINT pt, BOOL bHitTest)
{
    BOOL ret = FALSE;
    //if (IsInLoginView()) {
    //    if (PtInRect(&rcLoginEnterButtonFrame, pt)) {
    //        if (!bHitTest) Login_Commit();
    //        ret = TRUE;
    //    }
    //} else {
    //    if (PtInRect(&rcScreenshotsDirectoryLink, pt)) {
    //        if (!bHitTest) OpenScreenshotsDirectory();
    //        ret = TRUE;
    //    }
    //    if (PtInRect(&rctextViewOnWeb, pt)) {
    //        if (!bHitTest) OpenWorldPageInBrowser();
    //        ret = TRUE;
    //    }
    //    if (PtInRect(&rcConnectionLight, pt)) {
    //        if (!bHitTest) TogglePause();
    //        ret = TRUE;
    //    }
    //    if (PtInRect(&rctextChangeName, pt)) {
    //        if (!bHitTest) GoToSetup();
    //        ret = TRUE;
    //    }
    //    if (PtInRect(&rcSuccessList, pt)) {
    //        if (!bHitTest) SwitchToSuccessList();
    //        ret = TRUE;
    //    }
    //    if (FailedList.size() >  0) {
    //        if (PtInRect(&rcFailedList, pt)) {
    //            if (!bHitTest) SwitchToFailedList();
    //            ret = TRUE;
    //        }
    //        if (PtInRect(&rcRetryAll, pt)) {
    //            if (!bHitTest) RetryAllFailed();
    //            ret = TRUE;
    //        }
    //        if (PtInRect(&rcIgnoreAll, pt)) {
    //            if (!bHitTest) IgnoreAllFailed();
    //            ret = TRUE;
    //        }
    //    }
    //    if (PtInRect(&rcSnap, pt)) {
    //        if (!bHitTest) ResetLocation();
    //        ret = TRUE;
    //    }
    //}
    return ret;
}

VOID KeyPressed(HWND hWnd, WPARAM wParam)
{
    if (wParam == VK_ESCAPE) {
        Hide();
    }

}
