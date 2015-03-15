
#include "stdafx.h"
#include "MCuploader.h"


RECT rctextBack;
RECT rctextSettingsHeader;
RECT rctextReset;
RECT rctextResetButton;
RECT rcScreenshotsDirectoryLink;

HBRUSH hbrushReset;
HBRUSH hbrushScreenshotsLink;


BOOL Init_Settings(HWND hWnd)
{
    SetRect(&rctextBack, 340, 10, 400, 30);
    SetRect(&rctextSettingsHeader, 20, 20, 150, 50);
    SetRect(&rctextReset, 270, 50, 380, 70);
    SetRect(&rctextResetButton,
        rctextReset.left - 5,
        rctextReset.top - 5,
        rctextReset.right + 5,
        rctextReset.bottom + 5);

    hbrushReset = CreateSolidBrush(RGB(224, 56, 56));

    SetRect(&rcScreenshotsDirectoryLink, 300, 70, 390, 90);
    hbrushScreenshotsLink = CreateSolidBrush(RGB(133, 37, 250));

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
    LPWSTR strReset = TEXT("Change Name");
    FillRect(hdc, &rctextResetButton, hbrushReset);
    DrawText(hdc, strReset, wcslen(strReset), &rctextReset, DT_TOP | DT_LEFT);


    //open screenshots directory button
    //SelectObject(hdc, hFontNormal);
    //LPWSTR strScreenshotsDir = TEXT("Screenshots Dir");
    //FillRect(hdc, &rcScreenshotsDirectoryLink, hbrushScreenshotsLink);
    //DrawText(hdc, strScreenshotsDir, wcslen(strScreenshotsDir), &rcScreenshotsDirectoryLink, DT_TOP | DT_LEFT);


    //select screenshots directory




}


