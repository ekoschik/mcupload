
#include "stdafx.h"
#include "SysTrayDemo.h"
#include <list>

WCHAR ScreenshotDirPath[MAX_PATH];
WCHAR LastUploadedFileName[MAX_PATH];
std::list<LPCWSTR> FilesToUpload;


VOID MarkFileAsUploaded(LPCWSTR filepath)
{

    //TODO: Remove file from queue and
    //  update last uploaded file time.
    //  To be called when upload of a
    //  file finishes

}

VOID CheckFile(LPCWSTR filepath, LPCWSTR filename)
{

    //TODO: Check against last uploaded file's
    //  time, and FilesToUpload queue to skip
    //  file if it has already been uploaded


    LPWSTR file = new WCHAR[MAX_PATH];
    wcscpy(file, filepath);
    
    FilesToUpload.push_back(file);

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

        //Check File
        wsprintf(sPath, TEXT("%s\\%s"), ScreenshotDirPath, fdFile.cFileName);
        CheckFile(sPath, fdFile.cFileName);

    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
}


//
// Worker Thread for monitoring directory
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


    while (TRUE) {
    
        dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE);

        switch (dwWaitStatus) {
        case WAIT_OBJECT_0:

            ProcessDirectoryChange();
            if (FindNextChangeNotification(dwChangeHandle) == FALSE) {
                MessageBox(NULL,
                    _T("FindNextChangeNotification failed."),
                    _T("Error"), MB_OK);
            }
            break;

        default:
            break;
        }
    }
    return 0;
}

//
// Getting the \user\.minecraft\screenshots dir path
//
BOOL GetScreenshotsDirectoryPath()
{
    if (FAILED(SHGetFolderPath(NULL,
        CSIDL_APPDATA | CSIDL_FLAG_CREATE,
        NULL, 0, ScreenshotDirPath))) {

        //couldn't get root path
        MessageBox(NULL,
            _T("SHGetFolderPath failed."),
            _T("Error"), MB_OK);
        return FALSE;
    }
    PathAppend(ScreenshotDirPath, TEXT("\\.minecraft\\screenshots"));
    return TRUE;
}

//
// Start and Stop the worker thread for 
// watching the screenshots directory
//

HANDLE hThread;

BOOL StartWatchingDirectory()
{
    GetScreenshotsDirectoryPath();

    //TODO: Get last uploaded file time
    //  from the data file

    hThread = CreateThread(NULL, 0, WatchDirectory, NULL, 0, NULL);

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

LPCWSTR GetWatchedDirectory()
{
    return (LPCWSTR)& ScreenshotDirPath;
}