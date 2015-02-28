
#include "stdafx.h"
#include "SysTrayDemo.h"

//Code sample from http://www.codeproject.com/Articles/18783/Example-of-a-SysTray-App-in-Win

#define MAX_LOADSTRING 100
#define	WM_USER_SHELLICON WM_USER + 1

HINSTANCE hInst;
NOTIFYICONDATA nidApp;
HMENU hPopMenu;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
TCHAR szApplicationToolTip[MAX_LOADSTRING];
BOOL bDisable = FALSE;


VOID ActivateMenu(HWND hWnd)
{
    UINT uFlag = MF_BYPOSITION | MF_STRING;

    POINT lpClickPoint;
    GetCursorPos(&lpClickPoint);
    hPopMenu = CreatePopupMenu();

    if (bDisable == TRUE)
    {
        uFlag |= MF_GRAYED;
    }
    InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TEST2, _T("Test 2")); // Test 2
    InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TEST1, _T("Test 1")); // Test 1				
    InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, _T("SEP"));
    if (bDisable == TRUE)
    {
        InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_ENABLE, _T("Enable"));
    }
    else
    {
        InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_DISABLE, _T("Disable"));
    }
    InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, _T("SEP"));
    InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

    SetForegroundWindow(hWnd);
    TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);

}


LRESULT CALLBACK MenuWndProc(HWND hWnd,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    int wmId = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);

    switch (wmId)
    {
    case IDM_TEST1:
        MessageBox(NULL, _T("This is a test for menu Test 1"), _T("Test 1"), MB_OK);
        break;
    case IDM_TEST2:
        MessageBox(NULL, _T("This is a test for menu Test 2"), _T("Test 2"), MB_OK);
        break;
    case IDM_DISABLE:
        bDisable = TRUE;
        break;
    case IDM_ENABLE:
        bDisable = FALSE;
        break;
    case IDM_EXIT:
        Shell_NotifyIcon(NIM_DELETE, &nidApp);
        DestroyWindow(hWnd);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

VOID Paint(HDC hdc, HWND hWnd)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, 
                         UINT message, 
                         WPARAM wParam, 
                         LPARAM lParam) 
{
	switch (message) {

	case WM_USER_SHELLICON: 
		switch(LOWORD(lParam))  {   
			case WM_RBUTTONDOWN: 
			case WM_LBUTTONDOWN:
                ActivateMenu(hWnd);
				return TRUE;
		}
		break;

	case WM_COMMAND:
        return MenuWndProc(hWnd, message, wParam, lParam);

	case WM_PAINT:
    {
        PAINTSTRUCT ps;
        Paint(BeginPaint(hWnd, &ps), hWnd);
		EndPaint(hWnd, &ps);
		break;
    }

    case WM_DESTROY:
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSTRAYDEMO));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SYSTRAYDEMO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
    nidApp.uID = IDI_SYSTRAYDEMO;
    nidApp.hIcon = LoadIcon(hInstance, 
        (LPCTSTR)MAKEINTRESOURCE(IDI_SYSTRAYDEMO));
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
        IDC_SYSTRAYDEMO, szWindowClass, MAX_LOADSTRING);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance,
        MAKEINTRESOURCE(IDC_SYSTRAYDEMO));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

