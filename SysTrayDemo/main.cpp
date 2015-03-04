
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

HWND hMainWnd = NULL;

LRESULT CALLBACK MainMenuWndProc(HWND hWnd,
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam)
{
    PAINTSTRUCT ps;
    static BOOL bFirstCreation = TRUE;
    switch (message) {
    case WM_CREATE:
        OpenMainWindow(hWnd);
        break;

    //TODO: catch when losing focus and close

    case WM_PAINT:
        DrawMainWindow(hWnd, BeginPaint(hWnd, &ps));
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        break;

    }

    if (MainMenu_HandleWindowMessages(hWnd, message, wParam, lParam)) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

RECT GetMonitorRect()
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

VOID ActivateWindow(HWND hWnd)
{ 
    const int height = 300;
    const int width = 500;

    if (hMainWnd != NULL) {
        DestroyWindow(hMainWnd);
        hMainWnd = NULL;
        return;
    }

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
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = wndclass;
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    RECT rcwork = GetMonitorRect();
    rcwork.top = rcwork.bottom - height;
    rcwork.left = rcwork.right - width;
    rcwork.top -= 5;
    rcwork.left -= 5;
    rcwork.right -= 5;
    rcwork.bottom -= 5;

    hMainWnd = CreateWindow(
        wndclass, NULL,
        WS_POPUP,
        rcwork.left,
        rcwork.top,
        rcwork.right - rcwork.left,
        rcwork.bottom - rcwork.top,
        hWnd, NULL, hInst, NULL);
    
    ShowWindow(hMainWnd, SW_SHOW);
    SetActiveWindow(hMainWnd);
    SetWindowPos(hMainWnd, HWND_TOPMOST, 0,0,0,0,
        SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    
}

VOID ActivatePopupMenu(HWND hWnd)
{

    POINT lpClickPoint;
    GetCursorPos(&lpClickPoint);
    hPopMenu = CreatePopupMenu();

    //Exit
    InsertMenu(hPopMenu, 0xFFFFFFFF, 
        MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

    SetForegroundWindow(hWnd);
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

	case WM_USER_SHELLICON: 
        switch (LOWORD(lParam))  {
        case WM_LBUTTONDOWN:
            ActivateWindow(hWnd);
            return TRUE;
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

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance,
        MAKEINTRESOURCE(IDC_MCUPLOADICON));

    InitializeMainWindow(hInstance);

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

