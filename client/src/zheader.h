#pragma once

#include "Windowsx.h"
#include "Commctrl.h"
#include <string>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <Shlobj.h >

__inline VOID Error(std::wstring msg) {
	MessageBox(NULL, msg.c_str(), _T("Error"), MB_OK);
}

#define	WM_USER_SHELLICON		WM_USER + 1
#define IDI_MCUPLOADICON        107
#define IDC_MCUPLOADICON        109
#define IDS_APPTOOLTIP          104

extern NOTIFYICONDATA nid;
extern HWND tray_hwnd;
extern HWND hwnd;

//
// Drawing
//

typedef struct t_MainButton {
    std::wstring text;
    RECT         rcPos;
    RECT         rcPosIndent;
    BOOL         bHover;
    BOOL         bSelected;
    UINT         index;
} Button;

#define NUMBUTTONS 3;
extern Button Button_World;
extern Button Button_Uploads;
extern Button Button_Activity;
extern RECT rcDraggable;
extern RECT rcPause;
extern Button* CurrentSelection;
VOID InitializeDrawingResources();
VOID DrawMainWindow(HDC hdc);
VOID SetWindowLayout();



//
// Window
//

BOOL InitializeMainWindow(HWND hwnd);
VOID Show();
VOID Hide();
VOID WindowSizeChanged();
VOID Click(POINT pt);
VOID DblClick(POINT pt);
VOID Hover(POINT pt);
VOID Wheel(POINT pt, int delta);
BOOL NCHitTest(POINT pt);
VOID KeepWindowEntirelyOnMonitor();



//
// Msgs
//

LRESULT CALLBACK MainWindowWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

LRESULT CALLBACK TrayWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);


//
// Popup Window
//

VOID ShowPopupMenu(HWND hwndParent);
VOID HandlePopupMessage(INT msg);


//
// Data Files
//
typedef struct t_Data {
    std::wstring   servername;
    unsigned long port;
    std::wstring   worldname;
    std::wstring   watchdirpath;
} DATA;
extern DATA UsrData;
VOID InitializeDataFiles();
VOID WriteUserDataToFile();
BOOL IsFileNameInUploadLog(std::wstring filename);
VOID AddFileNameToUploadLog(std::wstring filename);

