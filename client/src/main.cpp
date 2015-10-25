#include "stdafx.h"
//#include "MCuploader.h"
//
//#define MAX_LOADSTRING 100
//#define	WM_USER_SHELLICON WM_USER + 1
//
//HINSTANCE hInst;
//NOTIFYICONDATA nidApp;
//HMENU hPopMenu;
//TCHAR szTitle[MAX_LOADSTRING];
//TCHAR szWindowClass[MAX_LOADSTRING];
//TCHAR szApplicationToolTip[MAX_LOADSTRING];
//
//HWND hMainWnd;
//
//BOOL bHidden = FALSE;
//VOID Hide() 
//{
//    if (bHidden) return;
//    bHidden = TRUE;
//    ShowWindow(hMainWnd, SW_HIDE);
//}
//
//VOID Show()
//{
//    if (!bHidden) return;
//    bHidden = FALSE;
//    ShowWindow(hMainWnd, SW_SHOW);
//}
//
//
//VOID ResetLocation()
//{
//    //Get the monitor based on last click
//    POINT ClickPoint;
//    GetCursorPos(&ClickPoint);
//    HMONITOR hmon = MonitorFromPoint(
//        ClickPoint, MONITOR_DEFAULTTONEAREST);
//
//    //Get Monitor Info
//    MONITORINFOEX moninfo;
//    moninfo.cbSize = sizeof(MONITORINFOEX);
//    if (GetMonitorInfo(hmon, &moninfo) == 0) {
//        Error(_T("GetMonitorInfo failed."));
//        return;
//    }
//
//    //Subtract window size to get rcMonitorBottomRight
//    RECT rcMonitorWork = moninfo.rcWork;
//    RECT rcMonitorBottomRight = rcMonitorWork;
//    rcMonitorBottomRight.left = rcMonitorBottomRight.right - window_width;
//    rcMonitorBottomRight.top = rcMonitorBottomRight.bottom - window_height;
//
//    //SetWindowPos
//    SetWindowPos(hMainWnd, NULL,
//        rcMonitorBottomRight.left, rcMonitorBottomRight.top,
//        rcMonitorBottomRight.right - rcMonitorBottomRight.left,
//        rcMonitorBottomRight.bottom - rcMonitorBottomRight.top,
//        SWP_SHOWWINDOW);
//
//    InvalidateRect(hMainWnd, NULL, TRUE);
//}
//
//POINT lp;
//LRESULT CALLBACK MainMenuWndProc(HWND hWnd,
//                                 UINT message,
//                                 WPARAM wParam,
//                                 LPARAM lParam)
//{
//    switch (message) {
//
//    //
//    // Drawing
//    //
//    case WM_PAINT: 
//    {
//        PAINTSTRUCT ps;
//        HDC hdc = BeginPaint(hWnd, &ps);
//        SetBkMode(hdc, TRANSPARENT);
//        DrawMainWindow(hWnd, hdc);
//        EndPaint(hWnd, &ps);
//        break;
//    }
//    
//    //
//    // Mouse and Keyboard Messages
//    //
//    case WM_LBUTTONDOWN:
//    {
//        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
//        ButtonMouseHandler(pt, TRUE); //TRUE for bClick
//        MainViewMouseHandler(pt, TRUE);
//        break;
//    }
//
//    case WM_KEYDOWN:
//        KeyPressed(hWnd, wParam);
//        break;
//
//    case WM_MOUSEMOVE:
//    {
//        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
//        ButtonMouseHandler(pt, FALSE); //FALSE for bClick
//        MainViewMouseHandler(pt, TRUE);
//        break;
//    }
//
//    case WM_MOUSEWHEEL:
//    {
//        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
//        ScreenToClient(hMainWnd, &pt); 
//        if (!InMainDrawingArea(pt)) {
//            ScrollButtons(GET_WHEEL_DELTA_WPARAM(wParam));
//        }
//        break;
//    }
//
//    //
//    // Hiding when losing focus
//    //
//    case WM_ACTIVATE:
//        if (wParam == WA_INACTIVE) {
//            Hide();
//        }
//        break;
//
//    //
//    // Notify messages are for the ListView
//    //
//    case WM_NOTIFY:
//        HandleListViewMessages(lParam);
//        break;
//    
//
//    case WM_MOVE:
//        GetWindowRect(hMainWnd, &rcWindow);
//        break;
//
//    //
//    // Moving the window by clicking and dragging the window
//    //
//    case WM_NCHITTEST:
//    {
//        LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
//        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
//        ScreenToClient(hMainWnd, &pt);
//        if (hit == HTCLIENT && 
//            !IsInLoginView() && 
//            InHeaderArea(pt)) {
//            
//            hit = HTCAPTION;
//        }
//        return hit;
//    }
//
//    case WM_DESTROY:
//        break;
//    
//    }
//
//    return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//
////
//// Right click popup menu
////
//VOID ShowRightClickMenu(HWND hWnd)
//{
//    hPopMenu = CreatePopupMenu();
//
//    //'Exit' option
//    InsertMenu(hPopMenu, 0xFFFFFFFF, 
//        MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));
//
//    SetForegroundWindow(hWnd);
//
//    POINT lpClickPoint;
//    GetCursorPos(&lpClickPoint);
//    TrackPopupMenu(hPopMenu, 
//        TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, 
//        lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
//}
//
//
////
//// Tray window WndProc
////
//LRESULT CALLBACK WndProc(HWND hWnd, 
//                         UINT message, 
//                         WPARAM wParam, 
//                         LPARAM lParam) 
//{
//	switch (message) {
//
//    //
//    // Clicking on the shell tray icon
//    //
//	case WM_USER_SHELLICON: 
//        switch (LOWORD(lParam))  {
//        case WM_LBUTTONDOWN:
//            Show();
//            SetForegroundWindow(hMainWnd);
//        break;
//
//        case WM_RBUTTONDOWN:
//            ShowRightClickMenu(hWnd);
//			return TRUE;
//		}
//        break;
//
//    //
//    //Handle Popup Menu Related Window Messages
//    //
//	case WM_COMMAND:
//        switch (LOWORD(wParam)) {
//        case IDM_EXIT:
//            Shell_NotifyIcon(NIM_DELETE, &nidApp);
//            DestroyWindow(hWnd);
//            break;
//        }
//        break;
//
//    case WM_DESTROY:
//        StopWatchingDirectory();        
//		PostQuitMessage(0);
//		break;
//	}
//
//    return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
////
//// Initialization
////
//BOOL InitInstance(HINSTANCE hInstance)
//{
//    hInst = hInstance;
//    LPWSTR WindowClass = _TEXT("WndClass");
//    LPWSTR WindowTitle = _TEXT("Title");
//
//    //Register Tray Window Class
//    WNDCLASSEX wcex;
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
//    wcex.lpfnWndProc = WndProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = 0;
//    wcex.hInstance = hInstance;
//    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MCUPLOADICON));
//    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = NULL;
//    wcex.lpszClassName = WindowClass;
//    wcex.hIconSm = NULL;
//    if (RegisterClassEx(&wcex) == 0) {
//        return FALSE;
//    }
//
//    //Create Tray Window
//    HWND hWnd = CreateWindow(WindowClass, 
//        WindowTitle, 
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, 
//        CW_USEDEFAULT,
//        CW_USEDEFAULT, 
//        CW_USEDEFAULT,
//        NULL, 
//        NULL, 
//        hInstance, 
//        NULL);
//    if (hWnd == NULL) {
//        return FALSE;
//    }
//
//    //Register Shell Notify Icon
//    ZeroMemory(&nidApp, sizeof(NOTIFYICONDATA));
//    nidApp.cbSize = sizeof(NOTIFYICONDATA);
//    nidApp.hWnd = hWnd;
//    nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//    nidApp.uID = IDI_MCUPLOADICON;
//    nidApp.hIcon = LoadIcon(hInstance, 
//        (LPCTSTR)MAKEINTRESOURCE(IDI_MCUPLOADICON));
//    nidApp.uCallbackMessage = WM_USER_SHELLICON;
//    LoadString(hInstance, IDS_APPTOOLTIP, nidApp.szTip, MAX_LOADSTRING);
//    if (!Shell_NotifyIcon(NIM_ADD, &nidApp)) {
//        return FALSE;
//    }
//
//    //Register Main Window
//    LPCWSTR wndclass = TEXT("MainWndClass");
//    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style = CS_DBLCLKS;
//    wcex.lpfnWndProc = MainMenuWndProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = 0;
//    wcex.hInstance = hInst;
//    wcex.hIcon = NULL;
//    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = NULL;
//    wcex.lpszClassName = wndclass;
//    wcex.hIconSm = NULL;
//    if (RegisterClassEx(&wcex) == 0) {
//        return FALSE;
//    }
//
//    //Create Window
//    hMainWnd = CreateWindow(
//        wndclass, NULL,
//        WS_POPUP/* | WS_SIZEBOX*/,
//        0,0,0,0,
//        hWnd, NULL, hInst, NULL);
//
//
//    //Finish initialization in window.cpp
//    InitializeMainWindow(hMainWnd);
//
//    //Show Window
//    ResetLocation();
//    Show();
//
//    SetActiveWindow(hMainWnd);
//    
//    return TRUE;
//}
//
//int APIENTRY _tWinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPTSTR    lpCmdLine,
//    int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//    UNREFERENCED_PARAMETER(nCmdShow);
//
//    InitCommonControls();
//
//    if (!InitInstance(hInstance)) {
//        return FALSE;
//    }
//
//    StartWatchingDirectory();
//
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        DispatchMessage(&msg);
//    }
//
//    return (int)msg.wParam;
//}
//
