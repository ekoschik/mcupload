
#include "stdafx.h"
#include "zheader.h"

//
// msgs.cpp
// Handle messages for both tray and main window,
// calling appropriate function to handle action
//


LRESULT CALLBACK MainWindowWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;


	switch (message) {

	case WM_CREATE:
		InitializeMainWindow(hwnd);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawMainWindow(hdc);
		EndPaint(hwnd, &ps);
		break;
		
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE) {
			Hide();
		}
		break;

    case WM_EXITSIZEMOVE:
        KeepWindowEntirelyOnMonitor();
        break;

    case WM_SIZE:
	//case WM_WINDOWPOSCHANGED:
		WindowSizeChanged();
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
    case WM_MOUSEMOVE:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		if (message == WM_LBUTTONDOWN) {
			Click(pt);
		} else if (message == WM_LBUTTONDBLCLK) {
			DblClick(pt);
		} else if (message == WM_MOUSEWHEEL) {
			Wheel(pt, GET_WHEEL_DELTA_WPARAM(wParam));
        } else if (message == WM_MOUSEMOVE) {
            Hover(pt);
        }
		break;
	}
	

	case WM_NCHITTEST:
	{
		LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
		if (hit == HTCLIENT) {
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			if (NCHitTest(pt)) {
				return HTCAPTION;
			}
		}
		return hit;
	}



	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}



//
// Tray Window
//
LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {

	case WM_USER_SHELLICON: 
		switch (LOWORD(lParam)) {

		//window messages sent to the tray icon

		case WM_RBUTTONDOWN:
			ShowPopupMenu(hwnd);
			break;

		case WM_LBUTTONDOWN:
			Show();
			break;

		case WM_MOUSEMOVE:
		case WM_MOUSELEAVE:
		{
			//POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			
			//TODO: Show/ Hide a Temporary Notification Window ('peek window')
			//		when mouse hovers over tray 

		}


		}
		break;


	case WM_COMMAND:
		HandlePopupMessage(LOWORD(wParam));
		break;

	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}