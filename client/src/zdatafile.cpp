
#include "stdafx.h"
#include "zheader.h"
using namespace std;

//
// datafile.cpp
// Manages persistant data stored in %AppData%\mcupload
//

wstring directory_path;
const LPCWSTR DirRootName = L"\\mcupload";
const LPCWSTR DirMCScreenshotsRootName = L"\\.minecraft\\screenshots";
const LPCWSTR DataIniName = L"\\data.ini";
const LPCWSTR UploadLogName = L"\\uploadlog.txt";

wstring dataini_path;
wstring uploadlog_path;
wstring def_screenshots_path;


//
// data.ini
//
// stores key/ value data that is stored at runtime in UsrData.
// When these values are changed, the data must be flushed back
// to the file manually with WriteUserDataToFile.
//

DATA UsrData;
const DATA defaults = {
    L"localhost", 3000, //server/port
    L"NA",//worldname
    L"NA" //watchdirpath, becomes %appdata%\.minecraft\screenshots
};

LPCWSTR appname = _T("mcupload");

VOID WriteKeyValue(wstring key, wstring value)
{
    if (WritePrivateProfileString(
        appname,
        key.c_str(),
        value.c_str(),
        dataini_path.c_str()) == 0) {

        wstring errmsg = 
            L"WriteKeyValue failed for key: " + 
            key + 
            L", value: " 
            + value;
        Error(errmsg.c_str());
    }
}

wstring GetKeyValue(wstring key)
{
#define buflength 1000
    WCHAR wcharbuf[buflength];
    if (GetPrivateProfileString(
            appname, 
            key.c_str(),
            _T(""), 
            wcharbuf, 
            buflength, 
            dataini_path.c_str()) > 0) {
        
        return wcharbuf;
    }

   wstring errmsg = L"GetKeyValue failed for key: " + key;
    Error(errmsg.c_str());
    return L"";
}

VOID WriteUserDataToFile()
{
    WriteKeyValue(L"servername", UsrData.servername);
    WriteKeyValue(L"watchdirpath", UsrData.watchdirpath);
    WriteKeyValue(L"worldname", UsrData.worldname);

    WriteKeyValue(L"port", std::to_wstring(UsrData.port));
}
    
VOID PopulateUsrDataFromFile()
{
    UsrData.servername = GetKeyValue(L"servername");
    UsrData.watchdirpath = GetKeyValue(L"watchdirpath");
    UsrData.worldname = GetKeyValue(L"worldname");

    wstring port = GetKeyValue(L"port");
    UsrData.port = wcstoul(port.c_str(), NULL, 0);
}

VOID InitializeDataIni()
{
    //Populate UsrData With Defaults
    UsrData.servername = defaults.servername;
    UsrData.port = defaults.port;
    UsrData.worldname = defaults.worldname;
    UsrData.watchdirpath = def_screenshots_path;
    
    //Ensure AppData\Roaming\mcupload Directory
    if (CreateDirectory(directory_path.c_str(), NULL) != 0 &&
        GetLastError() != ERROR_ALREADY_EXISTS) {
        Error(L"Ensure \\mcupload directory in AppData Failed.");
        return;
    }

    //Ensure data.ini, populating UsrData if exists already
    WIN32_FIND_DATA fd;
    HANDLE handle = FindFirstFile(dataini_path.c_str(), &fd);
    if (handle == INVALID_HANDLE_VALUE) {
        //First launch (or data.ini is missing).
        //Populate with default values.
        WriteUserDataToFile();
    } else {
        //Data.ini found.  Populate UsrData.
        PopulateUsrDataFromFile();
        FindClose(handle);
    }


}


//
// uploadlog.txt 
//
// stores filenames that have already been sent to the server,
// to avoid sending the same file over and over again.
//

vector<wstring> UploadLog;

VOID AddFileNameToUploadLog(wstring filename)
{
    UploadLog.push_back(filename);

    ofstream hfile(uploadlog_path.c_str());
    CHAR buf[MAX_PATH];
    for (auto it = UploadLog.begin(); it != UploadLog.end(); ++it) {

        wstring wstr = it->c_str();
        string str(wstr.begin(), wstr.end());
        ZeroMemory(&buf, MAX_PATH);
        snprintf((char*)&buf, MAX_PATH, "%s\n", str.c_str());
        hfile.write(buf, strlen(buf));
    }
}

BOOL IsFileNameInUploadLog(wstring filename)
{
    for (auto it = UploadLog.begin(); it != UploadLog.end(); ++it) {
        if (it->compare(filename) == 0) {
            return FALSE;
        }
    }

    return TRUE;
}

VOID InitializeUploadLog()
{
    //Read \mcupload\uploadlog.txt into vector<wstring> UploadLog
    string line;
    ifstream hfile(uploadlog_path.c_str());
    while (getline(hfile, line)) {
        wstring wstr(line.begin(), line.end());
        UploadLog.push_back(wstr);
    }
}

__inline VOID ErrOnFail(BOOL b, int errcode) {
    static int TestFailCount = 0;
    if (!b) TestFailCount++;
    if (TestFailCount == 1) {
        wstring err = L"Upload Log Test Failed. errcode:";
        err += errcode;
        Error(err);
    }
}

VOID TestUploadLog()
{
    wstring a(L"a"), b(L"b"), c(L"c"), d(L"d");

#define ShouldBeIn(a)     ErrOnFail(!IsFileNameInUploadLog(a), errcode); errcode++
#define ShouldNotBeIn(a)  ErrOnFail(IsFileNameInUploadLog(a), errcode); errcode++
    int errcode = 0;

    ShouldNotBeIn(a); //0
    ShouldNotBeIn(b); //1

    AddFileNameToUploadLog(b);
    ShouldNotBeIn(a); 
    ShouldBeIn(b);    //3

    AddFileNameToUploadLog(c);
    ShouldBeIn(b);
    ShouldBeIn(c);
    ShouldNotBeIn(a);
    ShouldNotBeIn(d); //7

    AddFileNameToUploadLog(a);
    ShouldBeIn(a);
    ShouldBeIn(b);
    ShouldBeIn(c);
    ShouldNotBeIn(d); //11

}

VOID InitializeDataFiles()
{
    //Get \\%AppData%\\ file path
    WCHAR appdata_path[MAX_PATH];
    if (FAILED(SHGetFolderPath(
        NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE,
        NULL, 0, appdata_path))) {
        Error(L"Initializing Data File: SHGetFolderPath Failed");
        return;
    }

    //Initialize directory_path (AppData\Roaming\mcupload)
    directory_path = appdata_path;
    directory_path += DirRootName;

    //Initialize data.ini and uploadload.txt file paths
    dataini_path = directory_path + DataIniName;
    uploadlog_path = directory_path + UploadLogName;

    //Initialize default MC screenshots directory
    def_screenshots_path = appdata_path;
    def_screenshots_path += DirMCScreenshotsRootName;

    InitializeDataIni();

    TestUploadLog();

    InitializeUploadLog();

}