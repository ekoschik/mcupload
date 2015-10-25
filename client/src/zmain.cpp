
#include "stdafx.h"
#include "zheader.h"

//
// main.cpp
// Creates the tray and main window, and pumps messages
//

NOTIFYICONDATA nid;
HWND tray_hwnd;
HWND hwnd;

BOOL Init(HINSTANCE hInstance)
{
    LPWSTR TrayWndClass = _TEXT("TrayWndClass");
    LPWSTR MainWindowWndClass = _TEXT("MainWndClass");
	LPWSTR TrayTitle = _TEXT("TrayTitle");
	LPWSTR MainWindowTitle = _TEXT("TrayTitle");
	LPWSTR AppTooltip = _TEXT("tooltip");

    //Register Tray Window Class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = TrayWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MCUPLOADICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TrayWndClass;
    wcex.hIconSm = NULL;
    if (RegisterClassEx(&wcex) == 0) {
		Error(L"Registering Tray Window Failed");
        return FALSE;
    }

    //Create Tray Window
    tray_hwnd = CreateWindow(
		TrayWndClass,
		TrayTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
        CW_USEDEFAULT,
        CW_USEDEFAULT, 
        CW_USEDEFAULT,
        NULL, 
        NULL, 
		hInstance,
        NULL);

    if (tray_hwnd == NULL) {
		Error(L"Creating Tray Window Failed");
        return FALSE;
    }

	ShowWindow(tray_hwnd, SW_HIDE);

    //Register Shell Notify Icon
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = tray_hwnd;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uID = IDI_MCUPLOADICON;
    nid.hIcon = LoadIcon(hInstance, 
        (LPCTSTR)MAKEINTRESOURCE(IDI_MCUPLOADICON));
    nid.uCallbackMessage = WM_USER_SHELLICON;
	wcscpy_s(nid.szTip, AppTooltip);
	if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
		Error(L"Registering Shell Notify Window Failed");
        return FALSE;
    }

    //Register Main Window
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = MainWindowWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = MainWindowWndClass;
    wcex.hIconSm = NULL;
    if (RegisterClassEx(&wcex) == 0) {
		Error(L"Registering Main Window Failed");
        return FALSE;
    }

    //Create Window
    hwnd = CreateWindow(
		MainWindowWndClass,
		MainWindowTitle,
        WS_POPUP | WS_SIZEBOX,
        0,0,0,0,
        NULL, 
		NULL, 
		hInstance, 
		NULL);

	if (hwnd == NULL) {
		Error(L"Creating Main Window Failed");
		return FALSE;
	}

	ShowWindow(hwnd, SW_HIDE);

    return TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	InitCommonControls();

	if (!Init(hInstance)) {
		return FALSE;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

