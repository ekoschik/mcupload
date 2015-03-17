#pragma once

#include "resource.h"

#include "Shlobj.h"
#include "Shlwapi.h"
#include "Windowsx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <string>

extern HINSTANCE hInst;
extern HWND hMainWnd;

__inline VOID Error(LPWSTR msg) {
    MessageBox(NULL, msg, _T("Error"), MB_OK);
}


VOID SetLoginEditControlsFromUD();
BOOL RefreshListView();
LRESULT NotifyHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID TogglePause();

//window.cpp
BOOL InitializeMainWindow(HWND hWnd);
VOID    DrawMainWindow(HWND hwnd, HDC hdc);
VOID MouseClick(POINT pt);
VOID KeyPressed(HWND hWnd, WPARAM wParam);
VOID GoToSettings();
VOID GoToSetup();
VOID GoToMainView();
extern WCHAR IniFilePath[MAX_PATH];
extern WCHAR ApplicationDirectoryPath[MAX_PATH];
extern int window_width;
extern int window_height;
extern HFONT hFontHeader;
extern HFONT hFontNormal;
extern HFONT hFontSmall;
extern BOOL bSettingsView;
extern BOOL bSetupView;
extern BOOL bPaused;
#define LOGINVIEW       (bSetupView)
#define SETTINGSVIEW    (!bSetupView && bSettingsView)
#define NORMALVIEW      (!LOGINVIEW && !SETTINGSVIEW)

//watch.cpp
BOOL    StartWatchingDirectory();
BOOL    StopWatchingDirectory();
LPCWSTR GetWatchedDirectory();
VOID    ProcessDirectoryChange();
extern BOOL bLastConnectionSuccessfull;
VOID OffThreadProcessDirectoryChange();
VOID IgnoreAllFailed();
VOID RetryAllFailed();

//upload.cpp
BOOL UploadFile(LPCWSTR filepath, LPCWSTR filename, SOCKET Socket);
SOCKET GetSocket();
unsigned short ReadPort();
const char* ReadIP();
VOID CloseConnection(SOCKET Socket);


//uploadlists.cpp
VOID    InitUploadLists();
BOOL    IsFilenameInIgnoreList(LPCWSTR filename);
BOOL    IsFilenameInSuccessList(LPCWSTR filename);
BOOL    IsFilenameInFailedList(LPCWSTR filename);
BOOL    IsFilenameInPendingList(LPCWSTR filename);
VOID    AddFileToPendingList(LPCWSTR filename);
VOID    AddFileToSuccessList(LPCWSTR filename);
VOID    AddFileToFailedList(LPCWSTR filename);
VOID    AddFileToIgnoreList(LPCWSTR filename);
VOID    AddFileToIgnoreList_Str(std::string file);
VOID    RemoveFileFromPending(LPCWSTR filename);
int     GetNumFailed();
int     GetNumSuccess();
extern std::vector<std::string> SuccessList;
extern std::vector<std::string> IgnoreList;
extern std::vector<std::string> PendingList;
extern std::vector<std::string> FailedList;
VOID DeleteAllUploadLists();

//dataini.cpp
BOOL    InitDataFile();
VOID    WriteDataToFile();
VOID    OpenScreenshotsDirectory();
std::string ToStr(LPCWSTR in);
extern WCHAR    IniFilePath[MAX_PATH];
extern WCHAR    ApplicationDirectoryPath[MAX_PATH];
//extern WCHAR    ScreenshotDirPath[MAX_PATH];

typedef struct myUserData {
    std::wstring username;
    std::wstring world;
    std::wstring servername;
    std::wstring port;
    std::wstring screenshotdirectory;

} USERDATA;
extern USERDATA UD;

enum KeyName {
    username,
    world,
    servername,
    port,
    screenshotdirectory
};



//base64.cpp
std::string base64_encode(
    unsigned char const* bytes_to_encode, 
    unsigned int in_len);

__inline std::string ToStr(LPCWSTR in)
{
#define RandomBufSize 1000
    CHAR buf[RandomBufSize];
    ZeroMemory(&buf, RandomBufSize);
    wcstombs((char*)&buf, in, wcslen(in));
    std::string out = buf;
    return out;
}


// Login View (login.cpp)
BOOL Init_Login(HWND hWnd);
VOID Draw_Login(HWND hWnd, HDC hdc);
VOID Login_Commit();
VOID HideEditControls();
extern RECT rcLoginEnterButtonFrame;
//extern HWND hLoginUsernameEditControl;
//extern HWND hLoginWorldEditControl;


// Main View (mainview.cpp)
BOOL Init_MainView(HWND hWnd);
VOID Draw_MainView(HWND hWnd, HDC hdc);
extern RECT rcScreenshotsDirectoryLink;
extern HWND hWndListView;
extern RECT rctextViewOnWeb;
extern RECT rcConnectionLight;
extern RECT rctextChangeName;
extern RECT rcSuccessList;
extern RECT rcFailedList;
VOID SwitchToSuccessList();
VOID SwitchToFailedList();
extern RECT rcRetryAll;
extern RECT rcIgnoreAll;
extern BOOL bSuccessList;

// Settings View (settings.cpp)
BOOL Init_Settings(HWND hWnd);
VOID Draw_Settings(HWND hWnd, HDC hdc);
extern RECT rctextBack;
extern RECT rctextResetButton;

