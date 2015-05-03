
#include "stdafx.h"
#include "MCuploader.h"
#include <Dwmapi.h>

#define BackgroundColor     RGB(211, 228, 245)
#define HeaderColor         RGB(180, 200, 219)
#define CommitButtonColor   RGB(152, 194, 219)

RECT rcMainArea;//(set in buttons.cpp)
RECT rcHeaderArea;
HBRUSH hbrBackground;
HBRUSH hbrHeader;

HFONT hfontRegular;
HFONT hfontHeader;

BOOL InHeaderArea(POINT pt) {
    return PtInRect(&rcHeaderArea, pt);
}

BOOL InMainDrawingArea(POINT pt) {
    return PtInRect(&rcMainArea, pt);
}

BOOL IsPaused() {
    return FALSE;
}

//login view
VOID ShowEditControls(BOOL bShow);
BOOL InitEditControls(HWND hWnd);
LPCWSTR strUsernameText = _T("UserName: ");
LPCWSTR strServerText = _T("Server: ");
LPCWSTR strPortText = _T("Port: ");
RECT rcUsernameText;
RECT rcServerText;
RECT rcPortText;
RECT rcUsernameEdit;
RECT rcServerEdit;
RECT rcPortEdit;
RECT rcCommitButton;
HBRUSH hbrCommitButton;
LPCWSTR strCommitButton = _T("GO");

//home view
RECT rcConnectionLight;
RECT rcServerPortText;
RECT rcPauseText;
RECT rcOpenWebText;
RECT rcOpenDirectoryText;
RECT rcChangeDirectoryText;
LPCWSTR strPause = _T("Pause");
LPCWSTR strOpenWeb = _T("Open On Web");
LPCWSTR strOpenDir = _T("Open Directory");
LPCWSTR strChangeDir = _T("Change Directory");
WCHAR strbufServerPort[1000];





//uploads, failed, activity
RECT rcListViewArea;
RECT rcHeaderText;
LPCWSTR strUploadsHeader = _T("Uploads");
LPCWSTR strFailedHeader = _T("Failed");
LPCWSTR strActivityHeader = _T("Activity");
RECT rcCountText;

VOID SetServerPortText()
{
    wsprintf((LPWSTR)&strbufServerPort, _T("%s:%s"),
        UD.servername.c_str(), UD.port.c_str());
}

VOID InitMainViewLayoutRects()
{

    int headerHeight = 35;
    int headerWidth = 100;
    int listViewBorder = 15;

    SetRect(&rcListViewArea,
        rcMainArea.left + listViewBorder,
        rcMainArea.top + headerHeight + listViewBorder,
        rcMainArea.right - listViewBorder,
        rcMainArea.bottom - listViewBorder);

    SetRect(&rcHeaderArea, rcMainArea.left, rcMainArea.top, 
        rcMainArea.right, rcMainArea.top + headerHeight);


    //Login
    int txtWidth = 75;
    int height = 20;
    int buf = 10; 
    int buttonwidth = 50;
    int buttonheight = 20;
    int buttonXIndent = 100;
    int x = rcMainArea.left + 15;
    int y = rcMainArea.top + 55;

    SetRect(&rcUsernameText, x, y, x + txtWidth, y + height);
    SetRect(&rcUsernameEdit, rcUsernameText.right + buf, 
        rcUsernameText.top, rcMainArea.right - buf, rcUsernameText.bottom);
    y += (height + buf);

    SetRect(&rcServerText, x, y, x + txtWidth, y + height);
    SetRect(&rcServerEdit, rcServerText.right + buf,
        rcServerText.top, rcMainArea.right - buf, rcServerText.bottom);
    y += (height + buf);
    
    SetRect(&rcPortText, x, y, x + txtWidth, y + height);
    SetRect(&rcPortEdit, rcPortText.right + buf,
        rcPortText.top, rcMainArea.right - buf, rcPortText.bottom);
    y += (height + buf);

    x += 80;
    y += 20;
    SetRect(&rcCommitButton,
        x, y, x + buttonwidth, y + buttonheight);

    hbrCommitButton = CreateSolidBrush(CommitButtonColor);


    //Uploads
    int offsetX = 10;
    CopyRect(&rcHeaderText, &rcHeaderArea);
    rcHeaderText.left += offsetX;

    int counttextsize = 30;
    CopyRect(&rcCountText, &rcHeaderArea);
    rcCountText.left = rcCountText.right - counttextsize;

    //Home

    int CL_width = 20;
    int CL_height = 20;

    SetRect(&rcConnectionLight, 
        rcHeaderArea.right - CL_width, 
        rcHeaderArea.top,
        rcHeaderArea.right, 
        rcHeaderArea.top + CL_height);

    int servertextwidth = 140;
    int servertextheight = 40;

    SetRect(&rcServerPortText, 
        rcHeaderArea.left + 20,
        rcHeaderArea.bottom + 5,
        rcHeaderArea.left + 20 + servertextwidth,
        rcHeaderArea.bottom + 5 + servertextheight);

    x = rcServerPortText.left + 20;
    y = rcServerPortText.bottom + 5;
    int itemX = 150, itemY = 30;
    SetRect(&rcPauseText, x, y, x + itemX, y + itemY);
    y += itemY;

    SetRect(&rcOpenWebText, x, y, x + itemX, y + itemY);
    y += itemY;

    SetRect(&rcOpenDirectoryText, x, y, x + itemX, y + itemY);
    y += itemY;

    SetRect(&rcChangeDirectoryText, x, y, x + itemX, y + itemY);
    y += itemY;

    SetServerPortText();
}




VOID InitMainView()
{
    hbrBackground = CreateSolidBrush(BackgroundColor);
    hbrHeader = CreateSolidBrush(HeaderColor);

    hfontRegular = CreateFont(20, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Segoe UI"));

    hfontHeader = CreateFont(28, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Arial Black"));


    InitMainViewLayoutRects();
    InitListView(hMainWnd, rcListViewArea);
    InitEditControls(hMainWnd);

}


VOID DrawMainView(HWND hWnd, HDC hdc)
{
    ShowEditControls(FALSE);
    FillRect(hdc, &rcMainArea, hbrBackground);

    if (gSelectedView == Login)  {

        //Login View
        ShowEditControls(TRUE);

        SelectFont(hdc, hfontRegular);
        DrawText(hdc, strUsernameText,
            wcslen(strUsernameText), &rcUsernameText, 
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        DrawText(hdc, strServerText,
            wcslen(strServerText), &rcServerText,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
        DrawText(hdc, strPortText,
            wcslen(strPortText), &rcPortText,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
        FillRect(hdc, &rcCommitButton, hbrCommitButton);
        DrawText(hdc, strCommitButton,
            wcslen(strCommitButton), &rcCommitButton,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);


       
    } 
    else {

        //Draw the header area
        FillRect(hdc, &rcHeaderArea, hbrHeader);

        LPCWSTR headerText = UD.username.c_str();;
        if (gSelectedView == Uploads) {
            headerText = strUploadsHeader;
        } else if (gSelectedView == Activity) {
            headerText = strActivityHeader;
        } else if (gSelectedView == Failed) {
            headerText = strFailedHeader;
        }

        SelectFont(hdc, hfontHeader);
        DrawText(hdc, headerText,
            wcslen(headerText), &rcHeaderText,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);


        if (gSelectedView == Home) {


            SelectFont(hdc, hfontRegular);

            DrawText(hdc, strbufServerPort, 
                wcslen(strbufServerPort), &rcServerPortText, 
                DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, strPause,
                wcslen(strPause), &rcPauseText,
                DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, strOpenWeb,
                wcslen(strOpenWeb), &rcOpenWebText,
                DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, strOpenDir,
                wcslen(strOpenDir), &rcOpenDirectoryText,
                DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, strChangeDir,
                wcslen(strChangeDir), &rcChangeDirectoryText,
                DT_VCENTER | DT_SINGLELINE);




        }
        else if (gSelectedView == Activity) {

            //Fill list view area with grey
            static HBRUSH hbrwhite = CreateSolidBrush(RGB(176, 176, 176));
            FillRect(hdc, &rcListViewArea, hbrwhite);

            //Coming soon text
            SelectFont(hdc, hfontRegular);
            const LPCWSTR strComingSoonText = _T("coming soon?");
            DrawText(hdc, strComingSoonText,
                wcslen(strComingSoonText), &rcListViewArea,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        else if (gSelectedView == Uploads || gSelectedView == Failed) {

            //List count in top right
            WCHAR strbuf[100];
            wsprintf((LPWSTR)&strbuf, _T("%d"), GetListViewSize());
            LPWSTR strListCount = (LPWSTR)&strbuf;
            DrawText(hdc, strListCount, wcslen(strListCount), 
                &rcCountText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        
    }
    



}


BOOL MainViewMouseHandler(POINT pt, BOOL bClick)
{
    return TRUE;
}


//
//Edit Controls
//

HWND edit_username;
HWND edit_server;
HWND edit_port;

VOID ShowEditControls(BOOL bShow)
{
    ShowWindow(edit_username, bShow ? SW_SHOW : SW_HIDE);
    ShowWindow(edit_server, bShow ? SW_SHOW : SW_HIDE);
    ShowWindow(edit_port, bShow ? SW_SHOW : SW_HIDE);
}

VOID CommitEditControlData()
{
    WCHAR buf[1000];

    //Read data from edit controls
    GetWindowText(edit_username, buf, MAX_PATH);
    UD.username = buf;

    GetWindowText(edit_server, buf, MAX_PATH);
    UD.servername = buf;

    GetWindowText(edit_port, buf, MAX_PATH);
    UD.port = buf;

    //Write UD to data.ini
    WriteDataToFile();

    //Switch view, (which unpauses uploades)
    gSelectedView = Home;

    //Delete lists (server/ name have changed)
    DeleteAllUploadLists();

    //Logging in kicks off one directory sweep
    //  (which needs to happen after switching to main view)
    OffThreadProcessDirectoryChange();
}

VOID SetEditControlDataFromUserData()
{
    SetWindowText(edit_username, UD.username.c_str());
    SetWindowText(edit_server, UD.servername.c_str());
    SetWindowText(edit_port, UD.port.c_str());

    SetServerPortText();
}

VOID HandleTabEnter(HWND hwndDlg, WPARAM wParam)
{
    //Tab moves to next control, Enter Commits
    if (hwndDlg == edit_username && wParam == VK_TAB) {
        SetFocus(edit_server);
    }
    else if (hwndDlg == edit_server && wParam == VK_TAB) {
        SetFocus(edit_port);
    }
    else {
        CommitEditControlData();
    }
}

WNDPROC OldEditWndProc;
INT_PTR CALLBACK EditControlWndProc(
    HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_KEYDOWN:
        if (wParam == VK_TAB || wParam == VK_RETURN) {
            HandleTabEnter(hwndDlg, wParam);
        }
        break;
    }

    return OldEditWndProc(hwndDlg, uMsg, wParam, lParam);
}

BOOL InitEditControls(HWND hWnd)
{
    edit_username = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        rcUsernameEdit.left, rcUsernameEdit.top,
        rcUsernameEdit.right - rcUsernameEdit.left,
        rcUsernameEdit.bottom - rcUsernameEdit.top,
        hWnd, NULL, hInst, NULL);
    if (edit_username == NULL) {
        return FALSE;
    }

    edit_server = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        rcServerEdit.left, rcServerEdit.top,
        rcServerEdit.right - rcServerEdit.left,
        rcServerEdit.bottom - rcServerEdit.top,
        hWnd, NULL, hInst, NULL);
    if (edit_server == NULL) {
        return FALSE;
    }

    edit_port = CreateWindow(TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        rcPortEdit.left, rcPortEdit.top,
        rcPortEdit.right - rcPortEdit.left,
        rcPortEdit.bottom - rcPortEdit.top,
        hWnd, NULL, hInst, NULL);
    if (edit_port == NULL) {
        return FALSE;
    }

    //Subclass to handle tab and enter keys
    OldEditWndProc =
        (WNDPROC)SetWindowLongPtr(edit_username,
        GWLP_WNDPROC, (LONG_PTR)EditControlWndProc);
    (WNDPROC)SetWindowLongPtr(edit_server,
        GWLP_WNDPROC, (LONG_PTR)EditControlWndProc);
    (WNDPROC)SetWindowLongPtr(edit_port,
        GWLP_WNDPROC, (LONG_PTR)EditControlWndProc);

    //Set text and hide
    SetEditControlDataFromUserData();
    ShowEditControls(FALSE);

    return TRUE;
}



