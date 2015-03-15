
#include "stdafx.h"
#include "MCuploader.h"

#define ID_LISTVIEW     1000

RECT rctextSettings;
RECT rctextUsername;
RECT rctextWorld;
RECT rcNumUploaded;

RECT rcListView;
HBRUSH hbrListViewBackground;
HWND hWndListView;

#define MAX_LISTSIZE 100
#define MAX_STR 1000

typedef struct myLISTVIEWITEM {
    BOOL bValid;
    WCHAR strName[MAX_STR];
} LISTVIEWITEM;
LISTVIEWITEM ListViewBuffer[MAX_LISTSIZE];

LRESULT NotifyHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //Cast Params
    LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
    NM_LISTVIEW *pNm = (NM_LISTVIEW *)lParam;
    LISTVIEWITEM* pListViewItem = (LISTVIEWITEM*)(pLvdi->item.lParam);

    if (wParam != ID_LISTVIEW)
        return 0L;

    switch (pLvdi->hdr.code)
    {
    //List views can ask for an items display name
    case LVN_GETDISPINFO: 
        pLvdi->item.pszText = pListViewItem->strName;
        break;
        
    }
}

BOOL RefreshListView()
{
    //Mark all buffer locations as not valid
    for (int i = 0; i < MAX_LISTSIZE; ++i) {
        ListViewBuffer[i].bValid = FALSE;
    }

    //Send a clear message to the list view
    if (!ListView_DeleteAllItems(hWndListView)) {
        return FALSE;
    }

    //Loop over each item in the success list
    int index = 0; 
    LV_ITEM lvI;
    lvI.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
    lvI.state = 0;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.pszText = LPSTR_TEXTCALLBACK;
    lvI.cchTextMax = MAX_STR;
    for (auto it = SuccessList.begin(); 
        it != SuccessList.end(); ++it) {

        //Designate a ListViewItem buffer and copy the char* value from SuccessList
        LISTVIEWITEM* plvi = &ListViewBuffer[index];//start at 0
        plvi->bValid = TRUE;
        mbstowcs(plvi->strName, SuccessList[index].c_str(), MAX_STR);
        lvI.lParam = (LPARAM)plvi;

        //LV_ITEM index starts at 1
        lvI.iItem = ++index;

        if (ListView_InsertItem(hWndListView, &lvI) == -1)
            return FALSE;

    }

    

    return TRUE;
}

BOOL Init_ListView(HWND hWnd)
{

    hWndListView = CreateWindow(WC_LISTVIEW,
        L"",
        WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_NOCOLUMNHEADER,
        rcListView.left, rcListView.top,
        rcListView.right - rcListView.left,
        rcListView.bottom - rcListView.top,
        hWnd, (HMENU)ID_LISTVIEW, hInst, NULL);

    if (!hWndListView) {
        return FALSE;
    }


    LV_COLUMN lvC;
    lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvC.fmt = LVCFMT_LEFT;  // left align the column

    // width of the column, in pixels
    lvC.cx = rcListView.right - rcListView.left - 40;

    lvC.pszText = _T("FileName");
    lvC.iSubItem = 0;
    if (ListView_InsertColumn(hWndListView, 0, &lvC) == -1)
        return FALSE;


    RefreshListView();


    return TRUE;
}


BOOL Init_MainView(HWND hWnd)
{
    SetRect(&rctextSettings, 340, 10, 400, 30);
    SetRect(&rctextUsername, 20, 20, 150, 50);
    SetRect(&rctextWorld, 20, 50, 150, 80);

    SetRect(&rcListView, 10, 50, 230, 240);

    SetRect(&rcNumUploaded, 250, 218, 390, 240);

    hbrListViewBackground = CreateSolidBrush(RGB(255, 255, 255));

    Init_ListView(hWnd);

    return TRUE;
}



VOID Draw_MainView(HWND hWnd, HDC hdc)
{
    ShowWindow(hWndListView, SW_SHOW);
    UpdateWindow(hWndListView);

    //settings in the top right to go back
    SelectObject(hdc, hFontSmall);
    LPWSTR strSettings = TEXT("Settings");
    DrawText(hdc, strSettings, wcslen(strSettings), &rctextSettings, DT_TOP | DT_LEFT);
    
    //header with Username and World
    SelectObject(hdc, hFontHeader);
    DrawText(hdc, UD.username.c_str(), UD.username.size(), &rctextUsername, DT_TOP | DT_LEFT);
    
    //counter for number of successfully uploaded screenshots
    WCHAR strSizeCounter[100];
    wsprintf((LPWSTR)&strSizeCounter, TEXT("# Uploaded: %d [%d]"), GetNumSuccess(), GetNumFailed());
    SelectObject(hdc, hFontNormal);
    DrawText(hdc, strSizeCounter, wcslen(strSizeCounter), &rcNumUploaded, DT_TOP | DT_LEFT);
    

    int i = 1;
}


