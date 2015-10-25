//
#include "stdafx.h"
//#include "MCuploader.h"
//
//#define ID_LISTVIEW     1000
//#define MAX_LISTSIZE 100
//#define MAX_STR 1000
//
//HWND hWndListView;
//RECT rcListView;
//
//typedef struct myLISTVIEWITEM {
//    BOOL bValid;
//    WCHAR strName[MAX_STR];
//} LISTVIEWITEM;
//LISTVIEWITEM ListViewBuffer[MAX_LISTSIZE];
//
//
//LRESULT HandleListViewMessages(LPARAM lParam)
//{
//    LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
//    NM_LISTVIEW *pNm = (NM_LISTVIEW *)lParam;
//    LISTVIEWITEM* pListViewItem = (LISTVIEWITEM*)(pLvdi->item.lParam);
//
//    switch (pLvdi->hdr.code) {
//
//    //List views can ask for an items display name
//    case LVN_GETDISPINFO:
//        pLvdi->item.pszText = pListViewItem->strName;
//        break;
//
//    }
//    return 0;
//}
//
//
//std::vector<std::string> tmpstrlist;
//
//int GetListViewSize()
//{
//    return tmpstrlist.size();
//}
//
//BOOL RefreshListView()
//{
//    //Mark all buffer locations as not valid
//    for (int i = 0; i < MAX_LISTSIZE; ++i) {
//        ListViewBuffer[i].bValid = FALSE;
//    }
//
//    //Delete all items in the list view
//    if (!ListView_DeleteAllItems(hWndListView)) {
//        return FALSE;
//    }
//
//    //leave list empty and hidden if not on upload/failed view
//    if (gSelectedView != Uploads && gSelectedView != Failed) {
//        ShowWindow(hWndListView, SW_HIDE);
//        return TRUE;
//    }
//
//    std::vector<std::string> * list = 
//    //    (gSelectedView == Uploads) ? &SuccessList : &FailedList;
//    &tmpstrlist;
//    
//    //Loop over each item in the list
//    int index = 0;
//    LV_ITEM lvI;
//    lvI.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
//    lvI.state = 0;
//    lvI.stateMask = 0;
//    lvI.iSubItem = 0;
//    lvI.pszText = LPSTR_TEXTCALLBACK;
//    lvI.cchTextMax = MAX_STR;
//    for (std::string str : *list) {
//
//        // copy text from list
//        LISTVIEWITEM* plvi = &ListViewBuffer[index];//start at 0
//        plvi->bValid = TRUE;
//        mbstowcs(plvi->strName, str.c_str(), MAX_STR);
//        
//        // pass pointer to item as lParam
//        lvI.lParam = (LPARAM)plvi;
//
//        // LV_ITEM index starts at 1
//        lvI.iItem = ++index;
//
//        if (ListView_InsertItem(hWndListView, &lvI) == -1)
//            return FALSE;
//    }
//
//    ShowWindow(hWndListView, SW_SHOW);
//    return TRUE;
//}
//
//BOOL InitListView(HWND hWnd, RECT rc)
//{
//    CopyRect(&rcListView, &rc);
//
//    hWndListView = CreateWindow(WC_LISTVIEW,
//        L"",
//        WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_NOCOLUMNHEADER,
//        rcListView.left, rcListView.top,
//        rcListView.right - rcListView.left,
//        rcListView.bottom - rcListView.top,
//        hWnd, (HMENU)ID_LISTVIEW, hInst, NULL);
//
//    if (!hWndListView) {
//        return FALSE;
//    }
//
//
//    LV_COLUMN lvC;
//    lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
//    lvC.fmt = LVCFMT_LEFT;  // left align the column
//
//    // width of the column, in pixels
//    lvC.cx = rcListView.right - rcListView.left - 40;
//
//    lvC.pszText = _T("FileName");
//    lvC.iSubItem = 0;
//    if (ListView_InsertColumn(hWndListView, 0, &lvC) == -1)
//        return FALSE;
//
//
//    RefreshListView();
//
//    std::string str = "item1";
//    tmpstrlist.push_back(str);
//    str = "item2";
//    tmpstrlist.push_back(str);
//    str = "item3";
//    tmpstrlist.push_back(str);
//
//    return TRUE;
//}
//
//
//
