
#include "stdafx.h"
#include "MCuploader.h"

#define MAX_LOADSTRING 100
#define	WM_USER_SHELLICON WM_USER + 1

HINSTANCE hInst;
NOTIFYICONDATA nidApp;
HMENU hPopMenu;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
TCHAR szApplicationToolTip[MAX_LOADSTRING];

HWND hMainWnd;

RECT rcTargetArea;


BOOL bHidden = FALSE;
VOID Hide() {
    if (bHidden) return;
    bHidden = TRUE;
    ShowWindow(hMainWnd, SW_HIDE);
}
VOID Show()
{
    if (!bHidden) return;
    bHidden = FALSE;
    ShowWindow(hMainWnd, SW_SHOW);
}
VOID ToggleShowHide() 
{
    if (bHidden) Show(); else Hide();
}

POINT lp;

LRESULT CALLBACK MainMenuWndProc(HWND hWnd,
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        break;

    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        DrawMainWindow(hWnd, BeginPaint(hWnd, &ps));
        EndPaint(hWnd, &ps);
        break;
    }
    
    case WM_LBUTTONDOWN:
    {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        MouseClick(pt);

        //Show();
        //SetFocus(hMainWnd);

        break;
    }

    case WM_KEYDOWN:
        KeyPressed(hWnd, wParam);
        break;

    case WM_ACTIVATE:
    {
        if (wParam == WA_INACTIVE) {
            Hide();
        }
        break;
    }

    case WM_NOTIFY:
        return(NotifyHandler(hWnd, message, wParam, lParam));
        break;
    
    case WM_MOUSEMOVE:
    {

        break;
    }

    case WM_DESTROY:
        break;
    
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

RECT GetClickPointMonitorRect()
{
    POINT ClickPoint;
    GetCursorPos(&ClickPoint);
    HMONITOR hmon = MonitorFromPoint(
        ClickPoint, MONITOR_DEFAULTTONEAREST);
    
    MONITORINFOEX moninfo;
    moninfo.cbSize = sizeof(MONITORINFOEX);
    if (GetMonitorInfo(hmon, &moninfo) == 0) {
        MessageBox(NULL,
            _T("GetMonitorInfo failed."), _T("Error"), MB_OK);
        return { 0, 0, 0, 0 };
    }
    
    return moninfo.rcWork;
}

VOID CreateMainWindow(HWND hWndParent)
{
    //Register Window
    LPCWSTR wndclass = TEXT("MainMenuWndClass");
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = MainMenuWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = wndclass;
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    //Setup Window Work Area
    RECT rcMon = GetClickPointMonitorRect();
    RECT rcwork;
    CopyRect(&rcwork, &rcMon);
    rcwork.top = rcwork.bottom - window_height;
    rcwork.left = rcwork.right - window_width;
    rcwork.top -= 5;
    rcwork.left -= 5;
    rcwork.right -= 5;
    rcwork.bottom -= 5;

    //for knowing if the cursor is in the bottom right corner
    CopyRect(&rcTargetArea, &rcwork);
    rcTargetArea.bottom = rcMon.bottom;
    rcTargetArea.right = rcMon.right;


    //Create Window
    hMainWnd = CreateWindow(
        wndclass, NULL,
        WS_POPUP,
        rcwork.left,
        rcwork.top,
        rcwork.right - rcwork.left,
        rcwork.bottom - rcwork.top,
        hWndParent, NULL, hInst, NULL);

    InitializeMainWindow(hMainWnd);

    //Show Window
    Show();

    SetActiveWindow(hMainWnd);
    SetWindowPos(hMainWnd, HWND_TOPMOST, 0,0,0,0,
        SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    
}

VOID ActivatePopupMenu(HWND hWnd)
{
    hPopMenu = CreatePopupMenu();

    //'Exit' option
    InsertMenu(hPopMenu, 0xFFFFFFFF, 
        MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

    SetForegroundWindow(hWnd);

    POINT lpClickPoint;
    GetCursorPos(&lpClickPoint);
    TrackPopupMenu(hPopMenu, 
        TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, 
        lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
}

//Handle Popup Menu Related Window Messages
VOID MenuWndProc(HWND hWnd,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    switch (LOWORD(wParam)) {
    case IDM_EXIT:
        Shell_NotifyIcon(NIM_DELETE, &nidApp);
        DestroyWindow(hWnd);
        break;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, 
                         UINT message, 
                         WPARAM wParam, 
                         LPARAM lParam) 
{
	switch (message) {
    case WM_CREATE:
        CreateMainWindow(hWnd);
        break;

	case WM_USER_SHELLICON: 
        switch (LOWORD(lParam))  {
        case WM_LBUTTONDOWN:
        {

            ToggleShowHide();
            if (!bHidden) {
                SetFocus(hMainWnd);
            }
        }
        break;

        //right click shows the popup menu
        case WM_RBUTTONDOWN:
            ActivatePopupMenu(hWnd);
			return TRUE;
		}
		break;

	case WM_COMMAND:
        MenuWndProc(hWnd, message, wParam, lParam);
        break;

    case WM_DESTROY:
        StopWatchingDirectory();
        
		PostQuitMessage(0);
		break;
	}

    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MCUPLOADICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ZeroMemory(&nidApp, sizeof(NOTIFYICONDATA));
    nidApp.cbSize = sizeof(NOTIFYICONDATA);
    nidApp.hWnd = hWnd;
    nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nidApp.uID = IDI_MCUPLOADICON;
    nidApp.hIcon = LoadIcon(hInstance, 
        (LPCTSTR)MAKEINTRESOURCE(IDI_MCUPLOADICON));
    nidApp.uCallbackMessage = WM_USER_SHELLICON;
    
    LoadString(hInstance, IDS_APPTOOLTIP, nidApp.szTip, MAX_LOADSTRING);
    
    Shell_NotifyIcon(NIM_ADD, &nidApp);

    return TRUE;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadString(hInstance,
        IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance,
        IDC_MCUPLOADICON, szWindowClass, MAX_LOADSTRING);

    InitCommonControls();

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance,
        MAKEINTRESOURCE(IDC_MCUPLOADICON));

    hInst = hInstance;

    StartWatchingDirectory();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

