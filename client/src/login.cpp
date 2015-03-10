
#include "stdafx.h"
#include "MCuploader.h"


extern HFONT hFontHeader;
extern HFONT hFontNormal;
extern HFONT hFontSmall;

RECT rcLoginHeader;
RECT rcLoginUsernameText;
RECT rcLoginWorldText;
RECT rcLoginUsernameEdit;
RECT rcLoginWorldEdit;
RECT rcLoginEnterButton;
RECT rcLoginEnterButtonFrame;
HWND hLoginUsernameEditControl;
HWND hLoginWorldEditControl;
HBRUSH hbrushEnterButton;

WNDPROC OldUsernameEditWndProc;
WNDPROC OldWorldEditWndProc;


VOID Login_Commit()
{
    WCHAR username[MAX_PATH];
    WCHAR world[MAX_PATH];
    GetWindowText(hLoginUsernameEditControl, username, MAX_PATH);
    GetWindowText(hLoginWorldEditControl, world, MAX_PATH);
}

INT_PTR CALLBACK EmailEditControlWndProc(
    _In_  HWND hwndDlg,
    _In_  UINT uMsg,
    _In_  WPARAM wParam,
    _In_  LPARAM lParam)
{
    switch (uMsg) {
    case WM_KEYDOWN:
        //the whole tab and enter thing...
        if (wParam == VK_TAB && hwndDlg == hLoginUsernameEditControl) {
            SetFocus(hLoginWorldEditControl);
        }
        else if (wParam == VK_RETURN || wParam == VK_TAB) {
            Login_Commit();
        }
        break;
    }

    return (hwndDlg == hLoginUsernameEditControl) ?
        OldUsernameEditWndProc(hwndDlg, uMsg, wParam, lParam) :
        OldWorldEditWndProc(hwndDlg, uMsg, wParam, lParam);
}
BOOL Init_Login(HWND hWnd)
{
    //Create edit control and hide them
    hLoginUsernameEditControl = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, 0, 0, hWnd, NULL, hInst, NULL);
    if (hLoginUsernameEditControl == NULL) {
        return FALSE;
    }

    hLoginWorldEditControl = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, 0, 0, hWnd, NULL, hInst, NULL);
    if (hLoginWorldEditControl == NULL) {
        return FALSE;
    }

    SetWindowPos(hLoginUsernameEditControl, NULL,
        rcLoginUsernameEdit.left,
        rcLoginUsernameEdit.top,
        rcLoginUsernameEdit.right - rcLoginUsernameEdit.left,
        rcLoginUsernameEdit.bottom - rcLoginUsernameEdit.top,
        SWP_HIDEWINDOW);

    SetWindowPos(hLoginWorldEditControl, NULL,
        rcLoginWorldEdit.left,
        rcLoginWorldEdit.top,
        rcLoginWorldEdit.right - rcLoginWorldEdit.left,
        rcLoginWorldEdit.bottom - rcLoginWorldEdit.top,
        SWP_HIDEWINDOW);

    //Subclass username edit control to commit on enter/tab
    OldUsernameEditWndProc =
        (WNDPROC)SetWindowLongPtr(hLoginUsernameEditControl,
        GWLP_WNDPROC, (LONG_PTR)EmailEditControlWndProc);
    OldWorldEditWndProc =
        (WNDPROC)SetWindowLongPtr(hLoginWorldEditControl,
        GWLP_WNDPROC, (LONG_PTR)EmailEditControlWndProc);

    hbrushEnterButton = CreateSolidBrush(RGB(105, 107, 255));

    int offsetX = 60;
    int offsetY = 50;
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
    int usernametxtwidth = 110;
    int usernametxtheight = 20;
    SetRect(&rcLoginUsernameText,
        offsetX,
        rcLoginHeader.bottom + buf,
        offsetX + usernametxtwidth,
        rcLoginHeader.bottom + buf + usernametxtheight);

    //World Text
    int worldtxtwidth = 100;
    int worldtxtheight = 20;
    SetRect(&rcLoginWorldText,
        offsetX,
        rcLoginUsernameText.bottom + buf,
        offsetX + worldtxtwidth,
        rcLoginUsernameText.bottom + buf + worldtxtheight);

    //Username Edit Control
    int usernameeditwidth = 100;
    int usernameeditheight = 20;
    SetRect(&rcLoginUsernameEdit,
        rcLoginUsernameText.right + buf,
        rcLoginUsernameText.top,
        rcLoginUsernameText.right + buf + usernameeditwidth,
        rcLoginUsernameText.bottom);

    //World Edit Control
    int worldeditwidth = 100;
    int worldeditheight = 20;
    SetRect(&rcLoginWorldEdit,
        rcLoginWorldText.right + buf,
        rcLoginWorldText.top,
        rcLoginWorldText.right + buf + worldeditwidth,
        rcLoginWorldText.bottom);

    ////Enter Button
    int buttonwidth = 50;
    int buttonheight = 20;
    SetRect(&rcLoginEnterButton,
        rcLoginWorldText.left,
        rcLoginWorldText.bottom + buf,
        rcLoginWorldText.left + buttonwidth,
        rcLoginWorldText.bottom + buf + buttonheight);
    SetRect(&rcLoginEnterButtonFrame,
        rcLoginEnterButton.left - 5,
        rcLoginEnterButton.top - 5,
        rcLoginEnterButton.right + 5,
        rcLoginEnterButton.bottom + 5);

    return FALSE;
}

VOID Draw_Login(HWND hWnd, HDC hdc)
{
    LPWSTR strHeader = TEXT("Hey there, looks like you're new.");
    LPWSTR strUsername = TEXT("MC username:");
    LPWSTR strWorldName = TEXT("World Name:");
    LPWSTR strEnter = TEXT("Enter");

    SelectObject(hdc, hFontHeader);
    DrawText(hdc, strHeader, wcslen(strHeader), &rcLoginHeader, DT_TOP | DT_LEFT);

    SelectObject(hdc, hFontNormal);
    DrawText(hdc, strUsername, wcslen(strUsername), &rcLoginUsernameText, DT_TOP | DT_LEFT);
    DrawText(hdc, strWorldName, wcslen(strWorldName), &rcLoginWorldText, DT_TOP | DT_LEFT);


    FillRect(hdc, &rcLoginEnterButtonFrame, hbrushEnterButton);
    DrawText(hdc, strEnter, wcslen(strEnter), &rcLoginEnterButton, DT_TOP | DT_LEFT);

    SetWindowPos(hLoginUsernameEditControl, NULL,
        rcLoginUsernameEdit.left,
        rcLoginUsernameEdit.top,
        rcLoginUsernameEdit.right - rcLoginUsernameEdit.left,
        rcLoginUsernameEdit.bottom - rcLoginUsernameEdit.top,
        SWP_SHOWWINDOW);

    SetWindowPos(hLoginWorldEditControl, NULL,
        rcLoginWorldEdit.left,
        rcLoginWorldEdit.top,
        rcLoginWorldEdit.right - rcLoginWorldEdit.left,
        rcLoginWorldEdit.bottom - rcLoginWorldEdit.top,
        SWP_SHOWWINDOW);

    SetFocus(hLoginUsernameEditControl);
}


