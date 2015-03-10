
#include "stdafx.h"
#include "MCuploader.h"


RECT rctextSettings;


BOOL Init_MainView(HWND hWnd)
{
    SetRect(&rctextSettings, 340, 10, 400, 30);
    return TRUE;
}

VOID Draw_MainView(HWND hWnd, HDC hdc)
{
    SelectObject(hdc, hFontSmall);
    LPWSTR strSettings = TEXT("Settings");
    DrawText(hdc, strSettings, wcslen(strSettings), &rctextSettings, DT_TOP | DT_LEFT);

}


