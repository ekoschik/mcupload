
#include "stdafx.h"
#include "MCuploader.h"


RECT rctextBack;
RECT rctextSettingsHeader;
RECT rctextReset;
RECT rctextResetButton;

HBRUSH hbrushReset;


BOOL Init_Settings(HWND hWnd)
{
    SetRect(&rctextBack, 340, 10, 400, 30);
    SetRect(&rctextSettingsHeader, 20, 20, 150, 50);
    SetRect(&rctextReset, 340, 50, 380, 70);
    SetRect(&rctextResetButton,
        rctextReset.left - 5,
        rctextReset.top - 5,
        rctextReset.right + 5,
        rctextReset.bottom + 5);

    hbrushReset = CreateSolidBrush(RGB(224, 56, 56));

    return TRUE;
}

VOID Draw_Settings(HWND hWnd, HDC hdc)
{
    SelectObject(hdc, hFontSmall);
    LPWSTR strBack = TEXT("Back");
    DrawText(hdc, strBack, wcslen(strBack), &rctextBack, DT_TOP | DT_LEFT);

    SelectObject(hdc, hFontHeader);
    LPWSTR strSettings = TEXT("Settings");
    DrawText(hdc, strSettings, wcslen(strSettings), &rctextSettingsHeader, DT_TOP | DT_LEFT);

    SelectObject(hdc, hFontNormal);
    LPWSTR strReset = TEXT("Reset");
    FillRect(hdc, &rctextResetButton, hbrushReset);
    DrawText(hdc, strReset, wcslen(strReset), &rctextReset, DT_TOP | DT_LEFT);



    //select screenshots directory




}


