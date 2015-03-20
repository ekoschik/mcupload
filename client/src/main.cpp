
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

    //
    // Drawing
    //
    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        DrawMainWindow(hWnd, BeginPaint(hWnd, &ps));
        EndPaint(hWnd, &ps);
        break;
    }
    
    //
    // Mouse and Keyboard Messages
    //
    case WM_LBUTTONDOWN:
    {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        MouseClick(pt, FALSE);//False to actually take action
        break;
    }

    case WM_KEYDOWN:
        KeyPressed(hWnd, wParam);
        break;

    case WM_MOUSEMOVE:
        break;


    //
    // Hiding when losing focus
    //
    case WM_ACTIVATE:
        if (wParam == WA_INACTIVE) {
            Hide();
        }
        break;

    //
    // Notify messages are for the ListView (mainview.cpp)
    //
    case WM_NOTIFY:
        NotifyHandler(lParam);
        break;
    

    //
    // Saving window placement
    //
    case WM_EXITSIZEMOVE:
        CommitWindowPlacement();
        break;

    case WM_MOVE:
        GetWindowRect(hMainWnd, &rcWindow);
        break;

    //
    // Moving the window by clicking and dragging the window
    //
    case WM_NCHITTEST:
    {
        LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hMainWnd, &pt);
        if (hit == HTCLIENT && !MouseClick(pt, TRUE)) {
            hit = HTCAPTION;
        }
        return hit;
    }

    case WM_DESTROY:
        break;
    
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

//Get the work rect of the monitor the last click was on
RECT rcMonitorBottomRight;
RECT GetBottomRightMonitorRect()
{
    //Get the monitor based on last click
    POINT ClickPoint;
    GetCursorPos(&ClickPoint);
    HMONITOR hmon = MonitorFromPoint(
        ClickPoint, MONITOR_DEFAULTTONEAREST);
    
    //Get Monitor Info
    MONITORINFOEX moninfo;
    moninfo.cbSize = sizeof(MONITORINFOEX);
    if (GetMonitorInfo(hmon, &moninfo) == 0) {
        MessageBox(NULL,
            _T("GetMonitorInfo failed."), _T("Error"), MB_OK);
        return { 0, 0, 0, 0 };
    }

    //Subtract window size to get rcMonitorBottomRight
    RECT rcMonitorWork = moninfo.rcWork;
    rcMonitorBottomRight = rcMonitorWork;
    rcMonitorBottomRight.left = rcMonitorBottomRight.right - window_width;
    rcMonitorBottomRight.top = rcMonitorBottomRight.bottom - window_height;
    
    return rcMonitorBottomRight;
}

VOID ResetLocation()
{
    SetWindowPos(hMainWnd, NULL, rcMonitorBottomRight.left, rcMonitorBottomRight.top,
        rcMonitorBottomRight.right - rcMonitorBottomRight.left,
        rcMonitorBottomRight.bottom - rcMonitorBottomRight.top, SWP_SHOWWINDOW);
    InvalidateRect(hMainWnd, NULL, TRUE);
}

VOID CreateMainWindow(HWND hWndParent)
{
    //Register Window
    LPCWSTR wndclass = TEXT("MainMenuWndClass");
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
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
    rcWindow = GetBottomRightMonitorRect();
    
    //Create Window
    hMainWnd = CreateWindow(
        wndclass, NULL,
        WS_POPUP/* | WS_SIZEBOX*/,
        rcWindow.left,
        rcWindow.top,
        rcWindow.right - rcWindow.left,
        rcWindow.bottom - rcWindow.top,
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




LRESULT CALLBACK WndProc(HWND hWnd, 
                         UINT message, 
                         WPARAM wParam, 
                         LPARAM lParam) 
{
	switch (message) {
    case WM_CREATE:
        CreateMainWindow(hWnd);
        break;

    //
    // Clicking on the shell tray icon
    //
	case WM_USER_SHELLICON: 
        switch (LOWORD(lParam))  {
        case WM_LBUTTONDOWN:
            Show();
            SetForegroundWindow(hMainWnd);
        break;

        case WM_RBUTTONDOWN:
            ActivatePopupMenu(hWnd);
			return TRUE;
		}
        break;

    //
    //Handle Popup Menu Related Window Messages
    //
	case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_EXIT:
            Shell_NotifyIcon(NIM_DELETE, &nidApp);
            DestroyWindow(hWnd);
            break;
        }
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

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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

