
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>


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

    return TRUE;
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




VOID ResetDataIni()
{
    //Delete data.ini
    if (!DeleteFile(IniFilePath)) {
        Error(TEXT("Remove data.iniFailed"));
    }

    bUsernameSet = FALSE;

    InvalidateRect(hMainWnd, NULL, TRUE);

}