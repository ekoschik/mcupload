
#include "stdafx.h"
#include "MCuploader.h"


RECT rctextBack;


BOOL Init_Settings(HWND hWnd)
{
    SetRect(&rctextBack, 340, 10, 400, 30);
    return TRUE;
}

VOID Draw_Settings(HWND hWnd, HDC hdc)
{
    SelectObject(hdc, hFontSmall);
    LPWSTR strBack = TEXT("Back");
    DrawText(hdc, strBack, wcslen(strBack), &rctextBack, DT_TOP | DT_LEFT);
}


