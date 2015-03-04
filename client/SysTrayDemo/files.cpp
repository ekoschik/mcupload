
#include "stdafx.h"
#include "MCuploader.h"

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
//  Email and bEmailSet
//

WCHAR   Email[MAX_PATH];
BOOL    bEmailSet;  //false before user enters an email

BOOL GetEmail()
{
    WCHAR email[MAX_PATH];
    ZeroMemory(&email, MAX_PATH);
    bEmailSet = GetKey(TEXT("email"), (LPWSTR)&email);
    if (bEmailSet) {
        wcscpy((LPWSTR)&Email, email);
    }
    return bEmailSet;
}

VOID SetEmail(LPWSTR email)
{
    bEmailSet = SetKey(TEXT("email"), email);
    if (bEmailSet) {
        wcscpy((LPWSTR)&Email, email);
    }
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



