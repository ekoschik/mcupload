
#include "stdafx.h"
#include "zheader.h"

//
// window.cpp
// Drawing functions for the main window
//

RECT rcWindow, rcClient;
HBRUSH hbrBackground;


RECT rcNav;
int NavWidth = 100;
int ButtonHeight = 40;
int SmallButtonHeight = 30;
int iDragAreaHeight = 25;

// Layout Rects
RECT rcDraggable;
RECT rcPause;
Button Button_World =    { L"World" ,    { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, FALSE, FALSE, 0 };
Button Button_Uploads =  { L"Uploads" ,  { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, FALSE, FALSE, 1 };
Button Button_Activity = { L"Activity" , { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, FALSE, FALSE, 2 };

//Button Resources
#define BackgroundColor     RGB(0, 0, 54)
#define UnselectedColor     RGB(166, 201, 237)
#define SelectedColor       RGB(90, 147, 209)
#define HoverColor          RGB(150, 187, 227)
HFONT  hFontButton;
HBRUSH hbrNavBackground;
HBRUSH hbrButtonNotSelected;
HBRUSH hbrButtonSelected;
HBRUSH hbrButtonHover;

#define DragAreaColor           RGB(102, 102, 153) //grey
#define PauseColor_ON           RGB(0, 255, 0)     //green
#define PauseColor_PAUSED       RGB(255, 255, 0)   //yellow
#define PauseColor_NOTCONNECTED RGB(255, 0, 0)     //red
HBRUSH hbrDragArea;
HBRUSH hbrPause;

VOID CalcIndentRect(RECT* rcDst, RECT* rcSrc, int indent)
{
    rcDst->top = rcSrc->top + indent;
    rcDst->left = rcSrc->left + indent;
    rcDst->right = rcSrc->right - indent;
    rcDst->bottom = rcSrc->bottom - indent;
}

VOID SetWindowLayout()
{
    //Reset rcWindow, rcClient
    GetWindowRect(hwnd, &rcWindow);
    GetClientRect(hwnd, &rcClient);

    //rcNav, right side of window and NavWidth wide
    CopyRect(&rcNav, &rcClient);
    rcNav.left = rcNav.right - NavWidth;

    //Set each button's rcPos
    RECT rc = rcClient;
    rc.left = rc.right - NavWidth;
    rc.bottom = rc.top + ButtonHeight;
    CopyRect(&Button_World.rcPos, &rc);

    rc.top += ButtonHeight;
    rc.bottom += ButtonHeight;
    CopyRect(&Button_Uploads.rcPos, &rc);

    rc.top += ButtonHeight;
    rc.bottom += ButtonHeight;
    CopyRect(&Button_Activity.rcPos, &rc);

    //Set each button's rcPosIndent
    int indent = 3;
    CalcIndentRect(&Button_World.rcPosIndent, &Button_World.rcPos, indent);
    CalcIndentRect(&Button_Uploads.rcPosIndent, &Button_Uploads.rcPos, indent);
    CalcIndentRect(&Button_Activity.rcPosIndent, &Button_Activity.rcPos, indent);


    //Set rcPause as bottom right corner
    SetRect(&rcPause,
            rcClient.right - SmallButtonHeight,
            rcClient.bottom - SmallButtonHeight,
            rcClient.right, rcClient.bottom);

    //Set rcDraggable as the top area of client area

    CopyRect(&rcDraggable, &rcClient);
    rcDraggable.right = rcNav.left;
    rcDraggable.bottom = rcDraggable.top + iDragAreaHeight;


}

__inline HBRUSH BrushForButton(Button* b) {
    return (b->bSelected) ? hbrButtonSelected :
        (b->bHover) ? hbrButtonHover :
        hbrButtonNotSelected;
}

__inline VOID DrawButton(HDC hdc, Button* b) {
    HBRUSH hbr = (b->bSelected) ? hbrButtonSelected :
                 (b->bHover) ? hbrButtonHover :
                 hbrButtonNotSelected;

    FillRect(hdc, (&b->rcPosIndent), hbr);
    
    DrawText(hdc, 
             b->text.c_str(), 
             b->text.length(), 
             &b->rcPosIndent, 
             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

VOID DrawMainWindow(HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);

    FillRect(hdc, &rcClient, hbrBackground);

    //Draw Navigation Area
    FillRect(hdc, &rcNav, hbrNavBackground);

    SelectFont(hdc, hFontButton);
    DrawButton(hdc, &Button_World);
    DrawButton(hdc, &Button_Uploads);
    DrawButton(hdc, &Button_Activity);



    FillRect(hdc, &rcDraggable, hbrDragArea);
    FillRect(hdc, &rcPause, hbrPause);
    


}

VOID InitializeDrawingResources()
{
    hbrBackground = CreateSolidBrush(RGB(0, 0, 100));


    //Setup Button Resources
    hFontButton = CreateFont(25, 0, 0, 0,
                             FW_DONTCARE, FALSE, FALSE, FALSE,
                             0, 0, 0, 0, 0,
                             TEXT("Segoe UI"));

    hbrNavBackground = CreateSolidBrush(BackgroundColor);
    hbrButtonNotSelected = CreateSolidBrush(UnselectedColor);
    hbrButtonSelected = CreateSolidBrush(SelectedColor);
    hbrButtonHover = CreateSolidBrush(HoverColor);

    hbrDragArea = CreateSolidBrush(DragAreaColor);
    hbrPause = CreateSolidBrush(PauseColor_ON);
    
        
}
