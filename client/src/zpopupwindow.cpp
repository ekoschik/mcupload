
#include "stdafx.h"
#include "zheader.h"

//
// popupwindow.cpp
// Manages the popup menu created when tray
// receives a right click message
//

#define IDM_EXIT				105
HMENU hPopupMenu;

VOID HandlePopupMessage(INT msg)
{
	if (msg == IDM_EXIT) {
		DestroyWindow(tray_hwnd);
	}
}

VOID ShowPopupMenu(HWND hwndParent)
{
	hPopupMenu = CreatePopupMenu();

	//'Exit' option
	InsertMenu(hPopupMenu, 0xFFFFFFFF,
		MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));


	//TODO: start/ stop watching, 
	//		open world in browser, 
	//		toggle keep window visible
	//		toggle show notifications
    //      toggle dragging area
    //      toggle resizable window style for main window


	POINT lpClickPoint;
	GetCursorPos(&lpClickPoint);

	TrackPopupMenu(hPopupMenu,
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN,
		lpClickPoint.x, lpClickPoint.y, 0, hwndParent, NULL);
}
