
#include "stdafx.h"
#include "MCuploader.h"


RECT rctextSettings;
RECT rctextUsername;
RECT rctextWorld;
RECT rcUploadListArea;


BOOL Init_MainView(HWND hWnd)
{
    SetRect(&rctextSettings, 340, 10, 400, 30);
    SetRect(&rctextUsername, 20, 20, 150, 50);
    SetRect(&rctextWorld, 20, 50, 150, 80);

    SetRect(&rcUploadListArea, 10, 80, 200, 240);
    return TRUE;
}

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


    //open website button


    //Uploaded scrolling area
    Rectangle(hdc, rcUploadListArea.left, rcUploadListArea.top, rcUploadListArea.right, rcUploadListArea.bottom);
    SelectObject(hdc, hFontNormal);
    if (UploadedFilesList.size() > 0) {
        //std::string strLastFile = UploadedFilesList.back();
        //std::wstring wstrLastFile(strLastFile.begin(), strLastFile.end());
        //DrawText(hdc, wstrLastFile.c_str(), wstrLastFile.size(), &rcUploadListArea, DT_TOP | DT_LEFT);
    }



    int i = 1;
}


