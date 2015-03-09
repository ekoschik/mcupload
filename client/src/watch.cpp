
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string> SuccessfullyUploadedFiles;

VOID TestAndUploadFile(LPCWSTR filepath, LPCWSTR filename)
{
    //Skip files that have already been uploaded
    if (IsInUploadedList(filename)) {
        return;
    }

    //Upload
    if (UploadFile(filepath, filename)) {
        SuccessfullyUploadedFiles.push_back(ToStr(filepath));
    }

    //Mark as uploaded to not upload again
    MarkUploaded(filename);
}


VOID ProcessDirectoryChange()
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    WCHAR sPath[MAX_PATH];
    
    // File mask
    wsprintf(sPath, TEXT("%s\\*.*"), ScreenshotDirPath);

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
        wsprintf(sPath, TEXT("%s\\%s"), ScreenshotDirPath, fdFile.cFileName);
        TestAndUploadFile(sPath, fdFile.cFileName);

    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    return;
}



//
// Worker Thread
//

DWORD WINAPI WatchDirectory(_In_ LPVOID lpParameter)
{

    DWORD dwWaitStatus;
    HANDLE dwChangeHandle;

    dwChangeHandle = FindFirstChangeNotification(
        (LPCWSTR)&ScreenshotDirPath,
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
// Start and Stop the worker thread for 
// watching the screenshots directory
//

HANDLE hThread;


BOOL StartWatchingDirectory()
{
    GetScreenshotsDirectoryPath();

    LoadAlreadyUploaded();

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
