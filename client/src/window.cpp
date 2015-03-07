
#include "stdafx.h"
#include "MCuploader.h"

#define IDD_EMAIL    100101

HWND hwndMain;
HBRUSH hbackground;
HFONT hFont;


HWND    hEmailEditControl;
RECT    rcEdit;

RECT rcText;
RECT rcX;
RECT rcScreemshotDir;



//
// Edit Control
//
WNDPROC OldEditWndProc;
INT_PTR CALLBACK EmailEditControlWndProc(
    _In_  HWND hwndDlg,
    _In_  UINT uMsg,
    _In_  WPARAM wParam,
    _In_  LPARAM lParam)
{
    switch (uMsg) {

    //Set email, and repaint on ENTER or TAB
    case WM_KEYDOWN:
        if (wParam == VK_RETURN || wParam == VK_TAB) {

            WCHAR tmpbuf[MAX_PATH];
            if (GetWindowText(hEmailEditControl, tmpbuf, MAX_PATH)) {
                SetEmail((LPWSTR)&tmpbuf);
                InvalidateRect(hwndMain, NULL, TRUE);
            }
        }
        break;
    }

    return OldEditWndProc(hwndDlg, uMsg, wParam, lParam);
}

BOOL SetupEditControl(HWND hWnd)
{
    //Create edit control
    hEmailEditControl = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, 0, 0, hWnd, NULL, hInst, NULL);
    if (hEmailEditControl == NULL) {
        return FALSE;
    }

    //Set edit control position
    SetWindowPos(hEmailEditControl, NULL,
        rcEdit.left,
        rcEdit.top,
        rcEdit.right - rcEdit.left,
        rcEdit.bottom - rcEdit.top,
        SWP_SHOWWINDOW);
    
    //Subclass edit control to commit on enter/tab
    OldEditWndProc = 
        (WNDPROC)SetWindowLongPtr(hEmailEditControl, 
            GWLP_WNDPROC, (LONG_PTR)EmailEditControlWndProc);


    return TRUE;
}


//
// Paint Handler
//
VOID DrawMainWindow(HWND hWnd, HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, hFont);

    //Hide the edit control by default
    ShowWindow(hEmailEditControl, SW_HIDE);

    //Draw background
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    FillRect(hdc, &rcClient, hbackground);


    if (bEmailSet) {

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
    else {

        //Draw Text
        LPWSTR str = TEXT("We need your email: (enter to save)");
        DrawText(hdc, str, wcslen(str), &rcText, DT_TOP | DT_LEFT);

        //Show email entry dialog box
        SetWindowPos(hEmailEditControl, NULL,
            rcEdit.left,
            rcEdit.top,
            rcEdit.right - rcEdit.left,
            rcEdit.bottom - rcEdit.top,
            SWP_SHOWWINDOW);

        SetFocus(hEmailEditControl);

    }



    BOOL bShowWorkingOnUpload = FALSE;
    BOOL bShowFinished = FALSE;
    BOOL bShowFailed = FALSE;





    static HBRUSH hbrOrange = CreateSolidBrush(RGB(255, 187, 0));
    static HBRUSH hbrRed = CreateSolidBrush(RGB(255, 0, 0));
    static HBRUSH hbrGreed = CreateSolidBrush(RGB(0, 255, 0));

    if (bShowWorkingOnUpload || bShowFinished || bShowFailed) {
        int indicatorheight = 10;
        RECT rcGreenIndicator;
        SetRect(&rcGreenIndicator, 
                rcClient.left, 
                rcClient.bottom - indicatorheight,
                rcClient.right,
                rcClient.bottom);


        HBRUSH hbr = hbrGreed;
        //if (bShowWhbrGreedorkingOnUpload) {
        //    hbr = hbrOrange;
        //}
        FillRect(hdc, &rcGreenIndicator, hbr);
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
        //clicking on the [change] invalidates the email address
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (PtInRect(&rcX, pt)) {
            bEmailSet = FALSE;
            InvalidateRect(hwndMain, NULL, TRUE);
        }
        break;

    }

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
    }
    return TRUE;
}


//
// Opening and Closing the Main Window
//
VOID OpenMainWindow(HWND hWnd)
{
    hwndMain = hWnd;

    GetEmail();

    if (!SetupEditControl(hWnd)){
        MessageBox(NULL,
            _T("Creating Edit Control failed."),
            _T("Error"), MB_OK);
    }
}

VOID CloseMainWindow()
{
    
}


//
// Single time application setup for Main Window
//
BOOL InitializeMainWindow()
{
    //Initialize resources
    hbackground = CreateSolidBrush(RGB(250, 218, 90));
    hFont = CreateFont(20, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Times New Roman"));

    //Initialize file path
    if (!InitDataFile()) {
        return FALSE;
    }


    //Initialize layout rects
    SetRect(&rcText, 50, 50, 500, 70);
    SetRect(&rcEdit, 50, 80, 300, 100);

    return TRUE;
}
