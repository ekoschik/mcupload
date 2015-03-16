
#include "stdafx.h"
#include "MCuploader.h"

#define ID_LISTVIEW     1000

RECT rctextSettings;
RECT rctextUsername;
RECT rctextWorld;
RECT rcNumUploaded;

RECT rctextViewOnWeb;
HBRUSH hbrViewOnWeb;

RECT rcListView;
HBRUSH hbrListViewBackground;
HWND hWndListView;

RECT rcConnectionLight;
HBRUSH hbrConnectionRed;
HBRUSH hbrConnectionYellow;
HBRUSH hbrConnectionGreen;

RECT rctextChangeName;

#define MAX_LISTSIZE 100
#define MAX_STR 1000

typedef struct myLISTVIEWITEM {
    BOOL bValid;
    WCHAR strName[MAX_STR];
} LISTVIEWITEM;
LISTVIEWITEM ListViewBuffer[MAX_LISTSIZE];


BOOL RefreshListView();

//
// Switching between the success and failure list
//

BOOL bSuccessList;
RECT rcSuccessList;
RECT rcFailedList;
RECT rcRetryAll;
RECT rcIgnoreAll;
HBRUSH hbrSucess_Selected;
HBRUSH hbrSucess_NotSelected;
HBRUSH hbrFailed_Selected;
HBRUSH hbrFailed_NotSelected;
VOID SwitchToSuccessList() {
    bSuccessList = TRUE;
    RefreshListView();
    InvalidateRect(hMainWnd, NULL, TRUE);
}
VOID SwitchToFailedList() {
    bSuccessList = FALSE;
    RefreshListView();
    InvalidateRect(hMainWnd, NULL, TRUE);
}



BOOL bPaused;
VOID TogglePause() {
    if (bLastConnectionSuccessfull) {
        bPaused = !bPaused;

        if (!bPaused) {
            OffThreadProcessDirectoryChange();
        }
    }
    InvalidateRect(hMainWnd, &rcConnectionLight, TRUE);
}
HBRUSH GetConnectionStateBrush() {
    //Red if not connected, yellow if paused, green if all is well
    return (!bLastConnectionSuccessfull) ? hbrConnectionRed : 
           (bPaused) ?  hbrConnectionYellow : hbrConnectionGreen;
}



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

    std::vector<std::string> * list = bSuccessList ?
        &SuccessList : &FailedList;

    //Loop over each item in the list
    int index = 0; 
    LV_ITEM lvI;
    lvI.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
    lvI.state = 0;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.pszText = LPSTR_TEXTCALLBACK;
    lvI.cchTextMax = MAX_STR;
    for (std::string str : *list) {
        //Designate a ListViewItem buffer and copy the char* value from SuccessList
        LISTVIEWITEM* plvi = &ListViewBuffer[index];//start at 0
        plvi->bValid = TRUE;
        mbstowcs(plvi->strName, str.c_str(), MAX_STR);
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

    SetRect(&rctextViewOnWeb, 250, 190, 390, 210);
    hbrViewOnWeb = CreateSolidBrush(RGB(209, 216, 89));
    hbrListViewBackground = CreateSolidBrush(RGB(255, 255, 255));

    SetRect(&rcConnectionLight, 315, 10, 330, 25);
    hbrConnectionRed = CreateSolidBrush(RGB(255, 48, 48));
    hbrConnectionYellow = CreateSolidBrush(RGB(247, 255, 18));
    hbrConnectionGreen = CreateSolidBrush(RGB(0, 212, 0));

    SetRect(&rctextChangeName, 200, 10, 310, 30);

    SetRect(&rcSuccessList, 240, 70, 290, 90);
    SetRect(&rcFailedList, 240, 100, 290, 120);
    hbrSucess_Selected = CreateSolidBrush(RGB(64, 194, 64));
    hbrSucess_NotSelected = CreateSolidBrush(RGB(159, 212, 159));
    hbrFailed_Selected = CreateSolidBrush(RGB(227, 79, 45));
    hbrFailed_NotSelected = CreateSolidBrush(RGB(222, 140, 122));
    SetRect(&rcRetryAll, 240, 125, 300, 140);
    SetRect(&rcIgnoreAll, 240, 145, 300, 165);

    bSuccessList = TRUE;

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
    

    //View On Web Button
    SelectObject(hdc, hFontNormal);
    WCHAR strViewOnWeb[100];
    wsprintf((LPWSTR)&strViewOnWeb, TEXT("View On Web"));
    FillRect(hdc, &rctextViewOnWeb, hbrViewOnWeb);
    DrawText(hdc, strViewOnWeb, wcslen(strViewOnWeb), &rctextViewOnWeb, DT_VCENTER | DT_CENTER);
    
    SelectBrush(hdc, GetConnectionStateBrush());
    Ellipse(hdc, rcConnectionLight.left, rcConnectionLight.top, rcConnectionLight.right, rcConnectionLight.bottom);

    //Change Name/ Server
    SelectObject(hdc, hFontSmall);
    LPWSTR strChangeName = TEXT("Change Name/ Server");
    DrawText(hdc, strChangeName, wcslen(strChangeName), &rctextChangeName, DT_TOP | DT_LEFT);



    //Success (and Failed list buttons)
    WCHAR strSuccess[100];
    wsprintf((LPWSTR)&strSuccess, TEXT("Success"));
    FillRect(hdc, &rcSuccessList, bSuccessList ? 
        hbrSucess_Selected : hbrSucess_NotSelected);
    DrawText(hdc, strSuccess, wcslen(strSuccess), &rcSuccessList, DT_VCENTER | DT_CENTER);
    
    if (FailedList.size() >  0) {

        LPRECT lprcSelected = bSuccessList ? &rcSuccessList : &rcFailedList;
        DrawEdge(hdc, lprcSelected, BDR_RAISEDINNER, BF_RECT);

        WCHAR strFailed[100];
        wsprintf((LPWSTR)&strFailed, TEXT("Failed"));
        FillRect(hdc, &rcFailedList, bSuccessList ?
        hbrFailed_NotSelected : hbrFailed_Selected);
        DrawText(hdc, strFailed, wcslen(strFailed), &rcFailedList, DT_VCENTER | DT_CENTER);

        SelectObject(hdc, hFontSmall);
        WCHAR strRetryAll[100];
        WCHAR strIgnoreAll[100];
        wsprintf((LPWSTR)&strRetryAll, TEXT("Retry All"));
        wsprintf((LPWSTR)&strIgnoreAll, TEXT("Ignore All"));
        DrawText(hdc, strRetryAll, wcslen(strRetryAll), &rcRetryAll, DT_VCENTER | DT_CENTER);
        DrawText(hdc, strIgnoreAll, wcslen(strIgnoreAll), &rcIgnoreAll, DT_VCENTER | DT_CENTER);
    }

}


