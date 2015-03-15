
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>


//
//  data.ini
//

WCHAR   IniFilePath[MAX_PATH];
WCHAR   ApplicationDirectoryPath[MAX_PATH];
USERDATA UD;

LPCWSTR keynames[] = {
    _T("username"), 
    _T("world"), 
    _T("servername"), 
    _T("port"), 
    _T("screenshotdirectory"),
};


LPCWSTR appname = _T("mcupload");

LPWSTR DefaultScreenshotDir()
{
    WCHAR wcharbuf[1000];
    if (FAILED(SHGetFolderPath(NULL,
        CSIDL_APPDATA | CSIDL_FLAG_CREATE,
        NULL, 0, wcharbuf))) {
        Error(_T("SHGetFolderPath failed."));
    }
    PathAppend(wcharbuf, TEXT("\\.minecraft\\screenshots"));
    return wcharbuf;
}

VOID InitDefaultValues()
{
    UD.username = _T("");
    UD.world = _T("defworld");
    UD.servername = _T("localhost");
    UD.port = _T("3000");

    WCHAR wcharbuf[1000];
    if (FAILED(SHGetFolderPath(NULL,
        CSIDL_APPDATA | CSIDL_FLAG_CREATE,
        NULL, 0, wcharbuf))) {
        Error(_T("SHGetFolderPath failed."));
    }
    PathAppend(wcharbuf, TEXT("\\.minecraft\\screenshots"));
    UD.screenshotdirectory = wcharbuf;
}

VOID WriteDataToFile()
{
    WritePrivateProfileString(appname, keynames[username], UD.username.c_str(), IniFilePath);
    WritePrivateProfileString(appname, keynames[world], UD.world.c_str(), IniFilePath);
    WritePrivateProfileString(appname, keynames[servername], UD.servername.c_str(), IniFilePath);
    WritePrivateProfileString(appname, keynames[port], UD.port.c_str(), IniFilePath);
    WritePrivateProfileString(appname, keynames[screenshotdirectory], UD.screenshotdirectory.c_str(), IniFilePath);

}

BOOL ReadContentsFromFile()
{
    WCHAR wcharbuf[1000];
    int ret;
    BOOL bFile = FALSE;

    if (GetPrivateProfileString(
        appname, keynames[username],
        _T(""), wcharbuf, 1000, IniFilePath) > 0) {
        UD.username = wcharbuf;
        bFile = TRUE;
    }
    if (GetPrivateProfileString(
        appname, keynames[world],
        _T(""), wcharbuf, 1000, IniFilePath) > 0) {
        UD.world = wcharbuf;
        bFile = TRUE;
    }
    if (GetPrivateProfileString(
        appname, keynames[servername],
        _T(""), wcharbuf, 1000, IniFilePath) > 0) {
        UD.servername = wcharbuf;
        bFile = TRUE;
    }
    if (GetPrivateProfileString(
        appname, keynames[port],
        _T(""), wcharbuf, 1000, IniFilePath) > 0) {
        UD.port = wcharbuf;
        bFile = TRUE;
    }
    if (GetPrivateProfileString(
        appname, keynames[screenshotdirectory],
        _T(""), wcharbuf, 1000, IniFilePath) > 0) {
        UD.screenshotdirectory = wcharbuf;
        bFile = TRUE;
    }

    return bFile;
}
//
//
//BOOL SetKey(LPCWSTR key, LPCWSTR val)
//{
//    return WritePrivateProfileString(
//        TEXT("MCuploader"), key, val, IniFilePath);
//}
//
//BOOL GetKey(LPCWSTR key, LPWSTR out)
//{
//    ZeroMemory(out, MAX_PATH);
//
//    int ret = GetPrivateProfileString(
//        TEXT("MCuploader"), key,
//        TEXT(""), //default value 
//        out, MAX_PATH,
//        IniFilePath);
//
//    return ret > 0;
//}
//
//VOID ResetDataIni()
//{
//
//
//
//}

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
    InitDefaultValues();
    if (!ReadContentsFromFile()) {
        GoToSetup();
    }

    return TRUE;
}





