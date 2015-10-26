
#include "stdafx.h"
#include "zheader.h"


//
// window.cpp
// Functions for the main window
//



int defWidth = 440, defHeight = 230;
Button* CurrentSelection = NULL;


VOID Show() {
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
}

VOID Hide() {
	ShowWindow(hwnd, SW_HIDE);
}

VOID Repaint() {
    InvalidateRect(hwnd, NULL, TRUE);
}


Button* ButtonHitTest(POINT pt)
{
    if (PtInRect(&Button_World.rcPos, pt)) {
        return &Button_World;
    } else if (PtInRect(&Button_Uploads.rcPos, pt)) {
        return &Button_Uploads;
    } else if (PtInRect(&Button_Activity.rcPos, pt)) {
        return &Button_Activity;
    }
    return NULL;
}

VOID SelectButton(Button* selection)
{
    if (selection && selection != CurrentSelection) {
        Button_World.bSelected =
            Button_Uploads.bSelected =
            Button_Activity.bSelected = FALSE;

        selection->bSelected = TRUE;

        CurrentSelection = selection;

        Repaint();
    }
}

VOID HoverButton(Button* selection)
{
    if (selection && !selection->bHover) {
        Button_World.bHover =
            Button_Uploads.bHover =
            Button_Activity.bHover = FALSE;

        selection->bHover = TRUE;

        Repaint();
    }
}

VOID Click(POINT pt)
{
    //Check clicking on a button
    SelectButton(ButtonHitTest(pt));


}

VOID Hover(POINT pt)
{
    //Check hovering over a button
    HoverButton(ButtonHitTest(pt));

}

RECT GetCursorMonitorWorkArea()
{
    POINT ptClick;
    GetCursorPos(&ptClick);
    HMONITOR hmon = MonitorFromPoint(
        ptClick, MONITOR_DEFAULTTONEAREST);

    MONITORINFOEX moninfo;
    moninfo.cbSize = sizeof(MONITORINFOEX);
    if (GetMonitorInfo(hmon, &moninfo) == 0) {
        Error(L"GetMonitorInfo failed.");
        return{ 0 };
    }

    return moninfo.rcWork;
}

VOID KeepWindowEntirelyOnMonitor()
{
    //TODO: why doesn't the window go all the
    //      way to the bottom right corner!!!
    //      BORDERS!!!



    //Get intersection of window and monitor work area,
    //and return if nothing needs to be done.
    RECT rcWorkArea = GetCursorMonitorWorkArea();
    RECT rcWnd, rcIntersect;
    GetWindowRect(hwnd, &rcWnd);
    IntersectRect(&rcIntersect, &rcWorkArea, &rcWnd);
    if (EqualRect(&rcIntersect, &rcWnd)) {
        return;
    }

    //Start with the current window rect
    RECT rcNew = rcWnd;
    int height = rcWnd.bottom - rcWnd.top;
    int width = rcWnd.right - rcWnd.left;

    //nudge down
    if (rcWnd.top < rcIntersect.top) {
        rcNew.top = rcIntersect.top;
        rcNew.bottom = rcNew.top + height;
    }

    //nudge right
    if (rcWnd.left < rcIntersect.left) {
        rcNew.left = rcIntersect.left;
        rcNew.right = rcNew.left + width;
    }

    //nudge left
    if (rcWnd.right > rcIntersect.right) {
        rcNew.right = rcIntersect.right;
        rcNew.left = rcNew.right - width;
    }

    //nudge up
    if (rcWnd.bottom > rcIntersect.bottom) {
        rcNew.bottom = rcIntersect.bottom;
        rcNew.top = rcNew.bottom - height;
    }

    MoveWindow(hwnd,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top, TRUE);
}

VOID ResetPlacement()
{
    //Move window to bottom right corner with default width and height

    RECT rcWorkArea = GetCursorMonitorWorkArea();

    SetWindowPos(hwnd,
                 NULL,
                 rcWorkArea.right - defWidth,
                 rcWorkArea.bottom - defHeight,
                 defWidth,
                 defHeight,
                 SWP_SHOWWINDOW);
}

VOID WindowSizeChanged()
{
    //Reset Drawing Layout
    SetWindowLayout();

    Repaint();
}


VOID DblClick(POINT pt)
{
	ResetPlacement();
}

VOID ScrollButtons(BOOL bPositiveDirection)
{
    //Determine which button to switch to
    int nextSelection = CurrentSelection->index;
    nextSelection += (bPositiveDirection ? 1 : -1);
    if (nextSelection < 0) {
        nextSelection += NUMBUTTONS;
    }
    nextSelection %= NUMBUTTONS;

    //Call Select Button
    if (nextSelection == 0) {
        SelectButton(&Button_World);
    } else if (nextSelection == 1) {
        SelectButton(&Button_Uploads);
    } else if (nextSelection == 2) {
        SelectButton(&Button_Activity);
    }

}

VOID Wheel(POINT pt, int delta)
{
    ScreenToClient(hwnd, &pt);

    if (ButtonHitTest(pt)) {

        //Accumulate wheel deltas
        static int delta_overflow = 0;
        delta_overflow += delta;

        //If we're over one delta, process one scroll
        if (abs(delta_overflow) >= WHEEL_DELTA) {
            ScrollButtons(delta_overflow > 0);
        }

        //keep delta counter in valid range
        delta_overflow %= WHEEL_DELTA;
    }

}

BOOL NCHitTest(POINT pt)
{
	ScreenToClient(hwnd, &pt);

    //rcDraggable represents the Non Client area of the window
	if (PtInRect(&rcDraggable, pt)) {
		return TRUE;
	}


	return FALSE;
}



BOOL InitializeMainWindow(HWND _hwnd)
{
	hwnd = _hwnd;

    InitializeDataFiles();

    InitializeDrawingResources();

    SelectButton(&Button_World);
	
    //Move the window to the bottom corner
    ResetPlacement();

	Hide();

	return TRUE;
}

