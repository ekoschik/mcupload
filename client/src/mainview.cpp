
#include "stdafx.h"
#include "MCuploader.h"
#include <string>

RECT rctextSettings;
RECT rctextUsername;
RECT rctextWorld;
RECT rcUploadListArea;
RECT rcNumUploaded;
RECT rcScreenshotsDirectoryLink;

HBRUSH hbrushNumUploaded;
HBRUSH hbrushScreenshotsLink;

BOOL Init_MainView(HWND hWnd)
{
    SetRect(&rctextSettings, 340, 10, 400, 30);
    SetRect(&rctextUsername, 20, 20, 150, 50);
    SetRect(&rctextWorld, 20, 50, 150, 80);

    SetRect(&rcUploadListArea, 10, 80, 200, 240);

    SetRect(&rcNumUploaded, 250, 218, 390, 240);
    hbrushNumUploaded = CreateSolidBrush(RGB(55, 191, 60));

    SetRect(&rcScreenshotsDirectoryLink, 300, 70, 390, 90);
    hbrushScreenshotsLink = CreateSolidBrush(RGB(133, 37, 250));

    return TRUE;
}

extern int failedcount;

VOID Draw_MainView(HWND hWnd, HDC hdc)
{
    //settings in the top right to go back
    SelectObject(hdc, hFontSmall);
    LPWSTR strSettings = TEXT("Settings");
    DrawText(hdc, strSettings, wcslen(strSettings), &rctextSettings, DT_TOP | DT_LEFT);
    
    //header with Username and World
    SelectObject(hdc, hFontHeader);
    DrawText(hdc, Username, wcslen(Username), &rctextUsername, DT_TOP | DT_LEFT);
    DrawText(hdc, World, wcslen(World), &rctextWorld, DT_TOP | DT_LEFT);

    //open directory button
    SelectObject(hdc, hFontNormal);
    LPWSTR strScreenshotsDir = TEXT("Screenshots Dir");
    FillRect(hdc, &rcScreenshotsDirectoryLink, hbrushScreenshotsLink);
    DrawText(hdc, strScreenshotsDir, wcslen(strScreenshotsDir), &rcScreenshotsDirectoryLink, DT_TOP | DT_LEFT);

    //open website button


    //counter for number of successfully uploaded screenshots
    int numuploaded = UploadedFilesList.size();
    WCHAR strSizeCounter[100];
    wsprintf((LPWSTR)&strSizeCounter, TEXT("# Uploaded: %d [%d]"), numuploaded, failedcount);
    //FillRect(hdc, &rcNumUploaded, hbrushNumUploaded);
    SelectObject(hdc, hFontNormal);
    DrawText(hdc, strSizeCounter, wcslen(strSizeCounter), &rcNumUploaded, DT_TOP | DT_LEFT);
    


    //Uploaded scrolling area
    //Rectangle(hdc, rcUploadListArea.left, rcUploadListArea.top, rcUploadListArea.right, rcUploadListArea.bottom);
    //SelectObject(hdc, hFontNormal);
    //if (UploadedFilesList.size() > 0) {
    //    //std::string strLastFile = UploadedFilesList.back();
    //    //std::wstring wstrLastFile(strLastFile.begin(), strLastFile.end());
    //    //DrawText(hdc, wstrLastFile.c_str(), wstrLastFile.size(), &rcUploadListArea, DT_TOP | DT_LEFT);
    //}


    int i = 1;
}


