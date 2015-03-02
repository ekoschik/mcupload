
#include "stdafx.h"
#include "SysTrayDemo.h"
#include "Shlobj.h"
#include "Shlwapi.h"

extern HINSTANCE hInst;
#define IDD_EMAIL    100101

HWND hwndMain;
HBRUSH hbackground;
HFONT hFont;

WCHAR IniFilePath[MAX_PATH];

HWND    hEmailEditControl;
WCHAR   Email[MAX_PATH];
BOOL    bEmailSet;
RECT    rcEdit;

RECT rcText;



//
// Writing to and reading from data.ini
//
__inline BOOL SetKey(LPCWSTR key, LPCWSTR val)
{
    return WritePrivateProfileString(
        TEXT("MCuploader"), key, val, IniFilePath);
}

__inline BOOL GetKey(LPCWSTR key, LPWSTR out)
{
    ZeroMemory(out, MAX_PATH);

    int ret = GetPrivateProfileString(
        TEXT("MCuploader"), key, 
        TEXT(""), //default value 
        out, MAX_PATH,
        IniFilePath);

    return ret > 0;
}

BOOL InitDataFile()
{
    //Initialize MCuploader Directory
    if (FAILED(SHGetFolderPath(NULL,
        CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
        NULL, 0, IniFilePath))) {

        //couldn't get root path
        MessageBox(NULL,
            _T("SHGetFolderPath failed."),
            _T("Error"), MB_OK);
        return FALSE;
    }
    PathAppend(IniFilePath, TEXT("\\MCuploader"));

    //Ensure directory
    if (CreateDirectory(IniFilePath, NULL) != 0 &&
        GetLastError() != ERROR_ALREADY_EXISTS) {

        //couldn't ensure directory existence
        MessageBox(NULL,
            _T("CreateDirectory failed."),
            _T("Error"), MB_OK);
        return FALSE;
    }

    //Initialize IniFilePath
    PathAppend(IniFilePath, TEXT("\\data.ini"));
}


//
// Get and Set the text from the email edit control
//  and save in Email
//
BOOL GetEmail()
{
    ZeroMemory(&Email, MAX_PATH);
    bEmailSet = GetKey(TEXT("email"), (LPWSTR)&Email);
    if (!bEmailSet) {
        //no data file exists
        wcscpy((LPWSTR)&Email, TEXT(""));
    }
    return bEmailSet;
}

VOID SetEmail(LPWSTR email)
{
    bEmailSet = SetKey(TEXT("email"), email);
    if (!bEmailSet) {
        return;
    }
    wcscpy((LPWSTR)&Email, email);
}


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

BOOL SetupEmailEditControl(HWND hWnd)
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
    
    //Subclass edit control
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

    ShowWindow(hEmailEditControl, SW_HIDE);

    GetEmail();

    //Draw background
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    FillRect(hdc, &rcClient, hbackground);


    if (bEmailSet) {

        //Draw Text
        WCHAR StrBuf[MAX_PATH];
        ZeroMemory(&StrBuf, sizeof(StrBuf));
        swprintf((LPWSTR)&StrBuf, TEXT("email: %s"), Email);
        DrawText(hdc, (LPWSTR)&StrBuf, wcslen(StrBuf), &rcText, DT_TOP | DT_LEFT);



    }
    else {

        //Draw Text
        LPWSTR str = TEXT("We need your email:");
        DrawText(hdc, str, wcslen(str), &rcText, DT_TOP | DT_LEFT);

        //Show email entry dialog box
        SetWindowPos(hEmailEditControl, NULL,
            rcEdit.left,
            rcEdit.top,
            rcEdit.right - rcEdit.left,
            rcEdit.bottom - rcEdit.top,
            SWP_SHOWWINDOW);

    }
}


//
// On Opening and Closing the Main Window
//
VOID OpenMainWindow(HWND hWnd)
{
    hwndMain = hWnd;

    if (!SetupEmailEditControl(hWnd)){
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
BOOL InitializeMainWindow(HINSTANCE hInst)
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
