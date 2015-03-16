
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>
#include <string>

WCHAR   ScreenshotDirPath[MAX_PATH];


VOID RetryAllFailed()
{
    for (std::string str : FailedList) {
        PendingList.push_back(str);
    }
    FailedList.clear();
    OffThreadProcessDirectoryChange();
    RefreshListView();
    InvalidateRect(hMainWnd, NULL, TRUE);
}

VOID IgnoreAllFailed()
{
    for (std::string str : FailedList) {
        AddFileToIgnoreList_Str(str);
    }
    FailedList.clear();
    RefreshListView();
    SwitchToSuccessList();
    InvalidateRect(hMainWnd, NULL, TRUE);
}

VOID UploadFile(SOCKET Socket, LPCWSTR filepath, LPCWSTR filename)
{ 
    if (UploadFile(filepath, filename, Socket)) {
        AddFileToSuccessList(filename);
    }
    else {
        AddFileToFailedList(filename);
    }

    InvalidateRect(hMainWnd, NULL, TRUE);

    RefreshListView();

}

VOID FlushPending()
{
    SOCKET Socket = GetSocket();
    
    while (Socket != NULL && 
           !bPaused &&
           PendingList.size() > 0) {

        //Get filename and filepath from Pending list
        std::string filename = PendingList[0].c_str();
        WCHAR wcharFilename[1000];
        mbstowcs((WCHAR*)&wcharFilename, 
            PendingList[0].c_str(), 1000);
        WCHAR wcharFilePath[1000];
        wsprintf((WCHAR*)wcharFilePath, _T("%s\\%s"),
            UD.screenshotdirectory.c_str(), wcharFilename);
        LPCWSTR wstrfilename = (LPCWSTR)&wcharFilename;
        LPCWSTR wstrfilepath = (LPCWSTR)&wcharFilePath;

        //Upload File, which puts file in success or failure list
        UploadFile(Socket, wstrfilepath, wstrfilename);

        PendingList.erase(PendingList.begin());
    }

    CloseConnection(Socket);

}

BOOL bLastConnectionSuccessfull;
VOID TestAndUploadFile(LPCWSTR filepath, LPCWSTR filename)
{
    //Test connection status
    SOCKET Socket = GetSocket();
    bLastConnectionSuccessfull = (Socket != NULL);
   
    if (!bLastConnectionSuccessfull || bPaused) {
        AddFileToPendingList(filename);
        return;
    }

    FlushPending();
    
    //Check already handled filenames
    if (IsFilenameInIgnoreList(filename) ||
        IsFilenameInFailedList(filename) ||
        IsFilenameInSuccessList(filename)) {
        return;
    }

    UploadFile(Socket, filepath, filename);

    CloseConnection(Socket);
}


VOID ProcessDirectoryChange()
{
    if (LOGINVIEW) {
        return;
    }

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    WCHAR sPath[MAX_PATH];
    
    // File mask
    wsprintf(sPath, TEXT("%s\\*.*"), UD.screenshotdirectory.c_str());

    // Call FindFirstFile on screenshot director to loop over all files
    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        MessageBox(NULL,
            _T("FindFirstFile function failed."),
            _T("Error"), MB_OK);
        return;
    }

    do {
        // Skip "." and ".."
        if (wcscmp(fdFile.cFileName, TEXT(".")) == 0
            || wcscmp(fdFile.cFileName, TEXT("..")) == 0) {
            continue;
        }            
        
        //Skip directories
        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        //Check file for uploading
        wsprintf(sPath, TEXT("%s\\%s"), UD.screenshotdirectory.c_str(), fdFile.cFileName);
        TestAndUploadFile(sPath, fdFile.cFileName);

    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    return;
}

//Syntax Glue
DWORD WINAPI OffThreadProcessDirectoryChange1(_In_ LPVOID lpParameter) {
    ProcessDirectoryChange();
    return 0;
}

//Calling Process Directory Change from the UI 
//thread when logging in creates a single use 
//worker thread
VOID OffThreadProcessDirectoryChange()
{
    if (CreateThread(NULL, 0,
        OffThreadProcessDirectoryChange1,
        NULL, 0, NULL) == NULL) {
        Error(_T("OffThreadProcessDirectoryChange failed."));
    }
}


//
// Worker Thread
//

DWORD WINAPI WatchDirectory(_In_ LPVOID lpParameter)
{

    DWORD dwWaitStatus;
    HANDLE dwChangeHandle;

    LPCTSTR screenshotdir = UD.screenshotdirectory.c_str();

    dwChangeHandle = FindFirstChangeNotification(
        screenshotdir,
        FALSE, // watch subtree 
        FILE_NOTIFY_CHANGE_FILE_NAME);

    if (dwChangeHandle == INVALID_HANDLE_VALUE || dwChangeHandle == NULL) {
        MessageBox(NULL,
            _T("FindFirstChangeNotification function failed."),
            _T("Error"), MB_OK);
        return -1;
    }

    //Read through directory once on startup
    ProcessDirectoryChange();

    while (TRUE) {
        dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE);

        switch (dwWaitStatus) {
        case WAIT_OBJECT_0:

            ProcessDirectoryChange(); 

            if (!FindNextChangeNotification(dwChangeHandle)) {
                MessageBox(NULL,
                    _T("FindNextChangeNotification failed."),
                    _T("Error"), MB_OK);
            }
            break;
        }
    }

    return 0;
}



//
// \user\.minecraft\screenshots
//


VOID OpenScreenshotsDirectory()
{
    ShellExecute(NULL,
        TEXT("open"),
        (LPWSTR)&ScreenshotDirPath,
        NULL,
        NULL,
        SW_SHOWDEFAULT);
}




//
// Start and Stop the worker thread for 
// watching the screenshots directory
//

HANDLE hThread;


BOOL StartWatchingDirectory()
{
    bLastConnectionSuccessfull = FALSE;
    InitUploadLists();

    hThread = CreateThread(NULL, 0,
                           WatchDirectory,
                           NULL, 0, NULL);

    if (hThread == NULL) {
        MessageBox(NULL,
            _T("CreateThread failed."),
            _T("Error"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL StopWatchingDirectory()
{
    TerminateThread(hThread, 0);
    return TRUE;
}
