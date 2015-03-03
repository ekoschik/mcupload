
#include "stdafx.h"
#include "SysTrayDemo.h"
#include <list>
#include <fstream>
#include <vector>
#include <string>

WCHAR ScreenshotDirPath[MAX_PATH];
WCHAR UploadedDirPath[MAX_PATH];

VOID MarkUploaded(LPCWSTR lastfile);
BOOL IsInUploadedList(LPCWSTR filename);

VOID TestAndUploadFile(LPCWSTR filepath, LPCWSTR filename)
{
    //Skip files that have already been uploaded
    if (IsInUploadedList(filename)) {
        return;
    }

    //Upload
    UploadFile(filepath);

    //Mark as uploaded
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

    wcscpy((LPWSTR)&UploadedDirPath, ScreenshotDirPath);
    PathAppend(UploadedDirPath, TEXT("\\uploaded"));

    CreateDirectory(UploadedDirPath, NULL);

    return TRUE;
}

//
// Keep a list of uploaded files, and remember between
//  runs by writing and read to uploaded.txt
//
extern WCHAR ApplicationDirectoryPath[MAX_PATH];
WCHAR UploadedDataFilePath[MAX_PATH];
std::vector<std::string> UploadedFilesList;

VOID LoadAlreadyUploaded()
{
    wcscpy((LPWSTR)&UploadedDataFilePath, ApplicationDirectoryPath);
    PathAppend(UploadedDataFilePath, TEXT("\\uploaded.txt"));

    std::ifstream hFile(UploadedDataFilePath);

    std::string line;
    while (std::getline(hFile, line))
        UploadedFilesList.push_back(line);
}

std::string ToStr(LPCWSTR in)
{
    CHAR buf[MAX_PATH];
    ZeroMemory(&buf, MAX_PATH);
    wcstombs((char*)&buf, in, wcslen(in));
    std::string out = buf;
    return out;
}

VOID MarkUploaded(LPCWSTR lastfile)
{
    std::string file = ToStr(lastfile);
    UploadedFilesList.push_back(file);

    std::ofstream hFile(UploadedDataFilePath);
    CHAR buf[MAX_PATH];
    for (auto it = UploadedFilesList.begin();
        it != UploadedFilesList.end(); ++it) {

        ZeroMemory(&buf, MAX_PATH);
        sprintf((char*)&buf, "%s\n", it->c_str());

        hFile.write(buf, strlen(buf));
    }    
}

BOOL IsInUploadedList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    for (auto it = UploadedFilesList.begin();
        it != UploadedFilesList.end(); ++it) {
        if (it->compare(file) == 0) {
            return TRUE;
        }
    }
    return FALSE;
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
