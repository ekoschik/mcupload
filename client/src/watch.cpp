
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>
#include <string>


VOID FlushPending()
{
    for (auto it = PendingList.begin(); it != PendingList.end(); ++it) {
        SOCKET Socket = GetSocket();
        if (Socket == NULL) {
            return;
        }

        //TODO: string->wstr and create filepath

        //if (UploadFile(filepath, filename, Socket)) {
        //    RemoveFileFromPending(filename);
        //    AddFileToSuccessList(filename);
        //}
    }
}

VOID TestAndUploadFile(LPCWSTR filepath, LPCWSTR filename)
{
    if (IsFilenameInIgnoreList(filename) ||
        IsFilenameInFailedList(filename) ||
        IsFilenameInPendingList(filename) ||
        IsFilenameInSuccessList(filename)) {
        return;
    }

    SOCKET Socket = GetSocket();
    if (Socket == NULL) {
        AddFileToPendingList(filename);
        return;
    }

    if (UploadFile(filepath, filename, Socket)) {
        AddFileToSuccessList(filename);
    } else {
        AddFileToFailedList(filename);
    }

    InvalidateRect(hMainWnd, NULL, TRUE);

    RefreshListView();
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

WCHAR   ScreenshotDirPath[MAX_PATH];

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
