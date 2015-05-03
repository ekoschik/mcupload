
#include "stdafx.h"
#include "MCuploader.h"

#define NUMBUTTONS 5

#define UnselectedColor     RGB(166, 201, 237)
#define SelectedColor       RGB(90, 147, 209)
#define HoverColor          RGB(150, 187, 227)


SelectedView gSelectedView;

BOOL IsInLoginView() {
    return gSelectedView == Login;
}

typedef struct t_MainButton {
    std::wstring text;
    RECT rcClientPosition;
    BOOL bHover;
    BOOL bSelected;
} MainButton;

MainButton Buttons[NUMBUTTONS] = {
    _T("Login"), {}, FALSE, FALSE,
    _T("Home"), {}, FALSE, FALSE,
    _T("Uploads"), {}, FALSE, FALSE,
    _T("Failed"), {}, FALSE, FALSE,
    _T("Activity"), {}, FALSE, FALSE
};

VOID SetButtonLayoutRects()
{
    int buttonheight = window_height / NUMBUTTONS - 10;
    int buttonwidth = window_width / 4;

    int bwidth = window_width / 4;
    int bheight = window_height / 5;

    RECT rc = { 0, 0, bwidth, bheight };

    for (int i = 0; i < NUMBUTTONS; i++) {

        SetRect(&Buttons[i].rcClientPosition,
            rc.left, rc.top, rc.right, rc.bottom);

        rc.top += bheight;
        rc.bottom += bheight;

    }

    SetRect(&rcMainArea,
        buttonwidth, 0, window_width, window_height);

}


//
// Drawing Resources
//

HFONT hFontButton;
HBRUSH hbrButtonNotSelected;
HBRUSH hbrButtonSelected;
HBRUSH hbrButtonHover;


VOID InitDrawingResources()
{
    hbrButtonNotSelected = CreateSolidBrush(UnselectedColor);
    hbrButtonSelected = CreateSolidBrush(SelectedColor);
    hbrButtonHover = CreateSolidBrush(HoverColor);

    hFontButton = CreateFont(25, 0, 0, 0,
        FW_DONTCARE, FALSE, FALSE, FALSE,
        0, 0, 0, 0, 0,
        TEXT("Segoe UI"));


}


//
// Painting
//

VOID DrawMainButtons(HWND hWnd, HDC hdc)
{

    //Draw Each Button
    for (int i = 0; i < NUMBUTTONS; ++i) {

        FillRect(hdc, &Buttons[i].rcClientPosition,
            Buttons[i].bSelected ? hbrButtonSelected :
            Buttons[i].bHover ? hbrButtonHover :
            hbrButtonNotSelected);

        SelectObject(hdc, hFontButton);
        DrawText(hdc, Buttons[i].text.c_str(),
            Buttons[i].text.size(), &Buttons[i].rcClientPosition,
            DT_VCENTER | DT_CENTER | DT_SINGLELINE);

    }

}



VOID InitButtons()
{
    InitDrawingResources();
    SetButtonLayoutRects();

}

VOID GotoView(SelectedView sel) 
{
    for (int j = 0; j < NUMBUTTONS; ++j) {
        Buttons[j].bSelected = FALSE;
    }
    Buttons[sel].bSelected = TRUE;

    gSelectedView = sel;

    //show (and populate)/ hide the listview
    RefreshListView();

    InvalidateRect(hMainWnd, NULL, TRUE);
}

SelectedView NextViewDown[] =
{ Home, Uploads, Failed, Activity, Login };
SelectedView NextViewUp[] =
{ Activity, Login, Home, Uploads, Failed };
VOID ScrollButtons(int delta) {
    static int wheel_delta = 0;
    wheel_delta += delta;
    if (wheel_delta >= 120 || wheel_delta <= -120) {
        GotoView(wheel_delta > 0 ? 
            NextViewUp[gSelectedView] : NextViewDown[gSelectedView]);
        wheel_delta = 0;
    }
}


//
// Handle Window Messages
//

BOOL ButtonMouseHandler(POINT pt, BOOL bClick)
{
    BOOL repaint = FALSE;

    for (int i = 0; i < NUMBUTTONS; ++i) {

        // If we're over a button, set the bHover
        // (or bClick) as FALSE for all buttons
        // and TRUE for this button
        if (PtInRect(&Buttons[i].rcClientPosition, pt)) {
            if (bClick) {
                if (!Buttons[i].bSelected) {
                    repaint = TRUE;

                    GotoView((SelectedView)i);
                }
            } else {
                if (!Buttons[i].bHover) {
                    repaint = TRUE;
                    for (int j = 0; j < NUMBUTTONS; ++j) {
                        Buttons[j].bHover = FALSE;
                    }
                    Buttons[i].bHover = TRUE;
                }
            }

            if (repaint) {
                InvalidateRect(hMainWnd, NULL, TRUE);
            }
            
            return TRUE;
        }

    }
    return FALSE;
}



