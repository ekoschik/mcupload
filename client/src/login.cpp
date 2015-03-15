
#include "stdafx.h"
#include "MCuploader.h"


RECT rcLoginHeader;

RECT rcLoginUsernameText;
RECT rcLoginServerText;
RECT rcLoginPortText;

RECT rcLoginUsernameEdit;
RECT rcLoginServerEdit;
RECT rcLoginPortEdit;

RECT rcLoginEnterButton;
RECT rcLoginEnterButtonFrame;
HWND UsernameEditControl;
HWND ServerEditControl;
HWND PortEditControl;
HBRUSH hbrushEnterButton;

WNDPROC OldEditWndProc;


VOID HideEditControls()
{
    ShowWindow(UsernameEditControl, SW_HIDE);
    ShowWindow(ServerEditControl, SW_HIDE);
    ShowWindow(PortEditControl, SW_HIDE);
}

extern VOID OffThreadProcessDirectoryChange();

VOID Login_Commit()
{
    WCHAR buf[1000];
    
    //Read data from edit controls and write to UD
    GetWindowText(UsernameEditControl, buf, MAX_PATH);
    UD.username = buf;
    
    GetWindowText(ServerEditControl, buf, MAX_PATH);
    UD.servername = buf;

    GetWindowText(PortEditControl, buf, MAX_PATH);
    UD.port = buf;

    //Write UD to data.ini
    WriteDataToFile();

    GoToMainView();

    //Logging in kicks off one directory sweep
    //  make sure happens after switching
    //  view to account for new user name
    OffThreadProcessDirectoryChange();

    HideEditControls();
}

VOID SetLoginEditControlsFromUD()
{
    SetWindowText(UsernameEditControl, UD.username.c_str());
    SetWindowText(ServerEditControl, UD.servername.c_str());
    SetWindowText(PortEditControl, UD.port.c_str());
}

INT_PTR CALLBACK EditControlWndProc(
    _In_  HWND hwndDlg,
    _In_  UINT uMsg,
    _In_  WPARAM wParam,
    _In_  LPARAM lParam)
{
    switch (uMsg) {
    case WM_KEYDOWN:
        //the whole tab and enter thing...
        if (wParam == VK_TAB && hwndDlg == UsernameEditControl) {
            SetFocus(ServerEditControl);
        } else if (wParam == VK_TAB && hwndDlg == ServerEditControl) {
            SetFocus(PortEditControl);
        } else if (wParam == VK_RETURN || wParam == VK_TAB) {
            Login_Commit();
        }
        break;
    }

    return OldEditWndProc(hwndDlg, uMsg, wParam, lParam);
}

BOOL bUsernameSet;
WCHAR Username[MAX_PATH];
WCHAR World[MAX_PATH];

BOOL Init_Login(HWND hWnd)
{
    //Create edit controls
    UsernameEditControl = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, 0, 0, hWnd, NULL, hInst, NULL);
    if (UsernameEditControl == NULL) {
        return FALSE;
    }

    ServerEditControl = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER ,
        0, 0, 0, 0, hWnd, NULL, hInst, NULL);
    if (ServerEditControl == NULL) {
        return FALSE;
    }

    PortEditControl = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, 0, 0, hWnd, NULL, hInst, NULL);
    if (PortEditControl == NULL) {
        return FALSE;
    }

    HideEditControls();

    //Subclass edit controls to commit on enter/tab
    OldEditWndProc =
        (WNDPROC)SetWindowLongPtr(UsernameEditControl,
        GWLP_WNDPROC, (LONG_PTR)EditControlWndProc);
    (WNDPROC)SetWindowLongPtr(ServerEditControl,
        GWLP_WNDPROC, (LONG_PTR)EditControlWndProc);
    (WNDPROC)SetWindowLongPtr(PortEditControl,
        GWLP_WNDPROC, (LONG_PTR)EditControlWndProc);

    //Create Brushes and things
    hbrushEnterButton = CreateSolidBrush(RGB(105, 107, 255));

    //Set RECTs

    int offsetX = 90;
    int offsetY = 30;
    int buf = 10;

    //Header
    int headerwidth = 300;
    int headerheight = 30;
    SetRect(&rcLoginHeader,
        offsetX,
        offsetY,
        offsetX + headerwidth,
        offsetY + headerheight);

    //Username Text
    int usernametxtwidth = 100;
    int usernametxtheight = 20;
    SetRect(&rcLoginUsernameText,
        offsetX,
        rcLoginHeader.bottom + buf,
        offsetX + usernametxtwidth,
        rcLoginHeader.bottom + buf + usernametxtheight);

    //Server Text
    int worldtxtwidth = 100;
    int worldtxtheight = 20;
    SetRect(&rcLoginServerText,
        offsetX,
        rcLoginUsernameText.bottom + buf,
        offsetX + worldtxtwidth,
        rcLoginUsernameText.bottom + buf + worldtxtheight);
   
    //Port Text
    int porttxtwidth = 100;
    int porttxtheight = 20;
    SetRect(&rcLoginPortText,
        offsetX,
        rcLoginServerText.bottom + buf,
        offsetX + porttxtwidth,
        rcLoginServerText.bottom + buf + porttxtheight);

    //Username Edit Control
    int usernameeditwidth = 100;
    int usernameeditheight = 20;
    SetRect(&rcLoginUsernameEdit,
        rcLoginUsernameText.right + buf,
        rcLoginUsernameText.top,
        rcLoginUsernameText.right + buf + usernameeditwidth,
        rcLoginUsernameText.bottom);

    //Server Edit Control
    int servereditwidth = 170;
    int servereditheight = 20;
    SetRect(&rcLoginServerEdit,
        rcLoginServerText.right + buf,
        rcLoginServerText.top,
        rcLoginServerText.right + buf + servereditwidth,
        rcLoginServerText.bottom);

    //Port Edit Control
    int porteditwidth = 100;
    int porteditheight = 20;
    SetRect(&rcLoginPortEdit,
        rcLoginPortText.right + buf,
        rcLoginPortText.top,
        rcLoginPortText.right + buf + porteditwidth,
        rcLoginPortText.bottom);

    

    //Enter Button
    int buttonSpace = 20;
    int buttonwidth = 50;
    int buttonheight = 20;
    SetRect(&rcLoginEnterButton,
        rcLoginPortText.left,
        rcLoginPortText.bottom + buf + buttonSpace,
        rcLoginPortText.left + buttonwidth,
        rcLoginPortText.bottom + buf + buttonSpace + buttonheight);
    SetRect(&rcLoginEnterButtonFrame,
        rcLoginEnterButton.left - 5,
        rcLoginEnterButton.top - 5,
        rcLoginEnterButton.right + 5,
        rcLoginEnterButton.bottom + 5);

    return FALSE;
}

extern RECT rcWindow; 
extern HBRUSH hbackground;
VOID Draw_Login(HWND hWnd, HDC hdc)
{

    FillRect(hdc, &rcWindow, hbackground);

    LPWSTR strHeader = TEXT("Please Login");
    LPWSTR strUsername = TEXT("username:");
    LPWSTR strServer = TEXT("server:");
    LPWSTR strPort = TEXT("port:");
    LPWSTR strEnter = TEXT("Enter");

    SelectObject(hdc, hFontHeader);
    DrawText(hdc, strHeader, wcslen(strHeader), &rcLoginHeader, DT_TOP | DT_LEFT);

    SelectObject(hdc, hFontNormal);
    DrawText(hdc, strUsername, wcslen(strUsername), &rcLoginUsernameText, DT_TOP | DT_LEFT);
    DrawText(hdc, strServer, wcslen(strServer), &rcLoginServerText, DT_TOP | DT_LEFT);
    DrawText(hdc, strPort, wcslen(strPort), &rcLoginPortText, DT_TOP | DT_LEFT);
    

    FillRect(hdc, &rcLoginEnterButtonFrame, hbrushEnterButton);
    DrawText(hdc, strEnter, wcslen(strEnter), &rcLoginEnterButton, DT_TOP | DT_LEFT);

    SetWindowPos(UsernameEditControl, NULL,
        rcLoginUsernameEdit.left,
        rcLoginUsernameEdit.top,
        rcLoginUsernameEdit.right - rcLoginUsernameEdit.left,
        rcLoginUsernameEdit.bottom - rcLoginUsernameEdit.top,
        SWP_SHOWWINDOW);

    SetWindowPos(ServerEditControl, NULL,
        rcLoginServerEdit.left,
        rcLoginServerEdit.top,
        rcLoginServerEdit.right - rcLoginServerEdit.left,
        rcLoginServerEdit.bottom - rcLoginServerEdit.top,
        SWP_SHOWWINDOW);

    SetWindowPos(PortEditControl, NULL,
        rcLoginPortEdit.left,
        rcLoginPortEdit.top,
        rcLoginPortEdit.right - rcLoginPortEdit.left,
        rcLoginPortEdit.bottom - rcLoginPortEdit.top,
        SWP_SHOWWINDOW);


    
    //Set focus to username edit control
    SetFocus(UsernameEditControl);
}


