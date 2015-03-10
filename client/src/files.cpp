
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>
#include <vector>


//
// MarkUploaded(filename) and IsInUploadedList(filepath)
//  writes to app directory \ uploaded.txt
//

WCHAR UploadedDataFilePath[MAX_PATH];
std::vector<std::string> UploadedFilesList;

VOID LoadAlreadyUploaded()
{
    wcscpy((LPWSTR)&UploadedDataFilePath, ApplicationDirectoryPath);
    PathAppend(UploadedDataFilePath, TEXT("\\uploaded.txt"));

    //create file if there isn't one?
    //WIN32_FIND_DATA fd;
    //if (FindFirstFile(UploadedDataFilePath, &fd) == NULL) {
    //    int i = 2;
    //}

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

VOID MarkUploaded(LPCWSTR filename)
{
    //add to list
    std::string file = ToStr(filename);
    UploadedFilesList.push_back(file);

    //write to file
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
    //read file and populate list
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
//  data.ini
//

WCHAR   IniFilePath[MAX_PATH];
WCHAR   ApplicationDirectoryPath[MAX_PATH];

BOOL InitDataFile()
{
    //Initialize MCuploader Directory
    if (FAILED(SHGetFolderPath(NULL,
        CSIDL_APPDATA | CSIDL_FLAG_CREATE,
        NULL, 0, IniFilePath))) {

        //couldn't get root path
        MessageBox(NULL,
            _T("SHGetFolderPath failed."),
            _T("Error"), MB_OK);
        return FALSE;
    }

    //Save App Directory Path
    PathAppend(IniFilePath, TEXT("\\MCuploader"));
    wcscpy((LPWSTR)&ApplicationDirectoryPath, IniFilePath);

    //Ensure directory
    if (CreateDirectory(IniFilePath, NULL) != 0 &&
        GetLastError() != ERROR_ALREADY_EXISTS) {

        //couldn't ensure directory existence
        MessageBox(NULL,
            _T("CreateDirectory failed."),
            _T("Error"), MB_OK);
        return FALSE;
    }

    //Initialize IniFilePath
    PathAppend(IniFilePath, TEXT("\\data.ini"));
}

BOOL SetKey(LPCWSTR key, LPCWSTR val)
{
    return WritePrivateProfileString(
        TEXT("MCuploader"), key, val, IniFilePath);
}

BOOL GetKey(LPCWSTR key, LPWSTR out)
{
    ZeroMemory(out, MAX_PATH);

    int ret = GetPrivateProfileString(
        TEXT("MCuploader"), key,
        TEXT(""), //default value 
        out, MAX_PATH,
        IniFilePath);

    return ret > 0;
}


//
// Getting the \user\.minecraft\screenshots dir path
//

WCHAR   ScreenshotDirPath[MAX_PATH];

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



