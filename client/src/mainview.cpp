
#include "stdafx.h"
#include "MCuploader.h"
#include <string>

RECT rctextSettings;
RECT rctextUsername;
RECT rctextWorld;
RECT rcUploadListArea;
RECT rcNumUploaded;

HBRUSH hbrListAreaBackground;
HWND hWndListView;

BOOL Init_ListView(HWND hWnd)
{
    hWndListView = CreateWindow(WC_LISTVIEW,
        L"",
        WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
        rcUploadListArea.left, rcUploadListArea.top,
        rcUploadListArea.right - rcUploadListArea.left,
        rcUploadListArea.bottom - rcUploadListArea.top,
        hWnd, NULL, hInst, NULL);





    //TODO : Create Listview, populated with SuccessList




    return TRUE;
}


BOOL Init_MainView(HWND hWnd)
{
    SetRect(&rctextSettings, 340, 10, 400, 30);
    SetRect(&rctextUsername, 20, 20, 150, 50);
    SetRect(&rctextWorld, 20, 50, 150, 80);

    SetRect(&rcUploadListArea, 10, 50, 230, 240);

    SetRect(&rcNumUploaded, 250, 218, 390, 240);

    hbrListAreaBackground = CreateSolidBrush(RGB(255, 255, 255));

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
    //DrawText(hdc, World, wcslen(World), &rctextWorld, DT_TOP | DT_LEFT);

    //counter for number of successfully uploaded screenshots
    WCHAR strSizeCounter[100];
    wsprintf((LPWSTR)&strSizeCounter, TEXT("# Uploaded: %d [%d]"), GetNumSuccess(), GetNumFailed());
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


