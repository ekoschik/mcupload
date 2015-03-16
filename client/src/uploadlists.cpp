
#include "stdafx.h"
#include "MCuploader.h"
#include <list>
#include <fstream>

LPCWSTR filename_success = _T("\\success.txt");
LPCWSTR filename_ignore = _T("\\ignore.txt");
WCHAR   filpath_success[MAX_PATH];
WCHAR   filpath_ignore[MAX_PATH];
std::vector<std::string> SuccessList;
std::vector<std::string> IgnoreList;
std::vector<std::string> PendingList;
std::vector<std::string> FailedList;


VOID InitUploadLists()
{
    //success.txt && SuccessList
    std::string line;
    wcscpy((LPWSTR)&filpath_success, ApplicationDirectoryPath);
    PathAppend(filpath_success, filename_success);
    std::ifstream hFile_success(filpath_success);
    SuccessList.clear();
    while (std::getline(hFile_success, line))
        SuccessList.push_back(line);

    //ignore.txt && IgnoreList
    wcscpy((LPWSTR)&filpath_ignore, ApplicationDirectoryPath);
    PathAppend(filpath_ignore, filename_ignore);
    std::ifstream hFile_ignore(filpath_ignore);
    IgnoreList.clear();
    while (std::getline(hFile_ignore, line))
        IgnoreList.push_back(line);


    RefreshListView();

}

VOID DeleteAllUploadLists()
{
    DeleteFile(filpath_success);
    DeleteFile(filpath_ignore);
    SuccessList.clear();
    IgnoreList.clear();
    PendingList.clear();
    FailedList.clear();
}

BOOL IsFilenameInIgnoreList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    for (auto it = IgnoreList.begin();
        it != IgnoreList.end(); ++it) {
        if (it->compare(file) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IsFilenameInSuccessList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    for (auto it = SuccessList.begin();
        it != SuccessList.end(); ++it) {
        if (it->compare(file) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IsFilenameInFailedList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    for (auto it = FailedList.begin();
        it != FailedList.end(); ++it) {
        if (it->compare(file) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IsFilenameInPendingList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    for (auto it = PendingList.begin();
        it != PendingList.end(); ++it) {
        if (it->compare(file) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

VOID AddFileToSuccessList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    SuccessList.push_back(file);

    std::ofstream hFile(filpath_success);
    CHAR buf[MAX_PATH];
    for (auto it = SuccessList.begin();
        it != SuccessList.end(); ++it) {
        ZeroMemory(&buf, MAX_PATH);
        sprintf((char*)&buf, "%s\n", it->c_str());
        hFile.write(buf, strlen(buf));
    }

}


VOID AddFileToIgnoreList_Str(std::string file)
{
    IgnoreList.push_back(file);

    std::ofstream hFile(filpath_ignore);
    CHAR buf[MAX_PATH];
    for (auto it = IgnoreList.begin();
        it != IgnoreList.end(); ++it) {
        ZeroMemory(&buf, MAX_PATH);
        sprintf((char*)&buf, "%s\n", it->c_str());
        hFile.write(buf, strlen(buf));
    }
}
VOID AddFileToIgnoreList(LPCWSTR filename)
{
    AddFileToIgnoreList_Str(ToStr(filename));
}

VOID AddFileToPendingList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    PendingList.push_back(file);
}

VOID AddFileToFailedList(LPCWSTR filename)
{
    std::string file = ToStr(filename);
    FailedList.push_back(file);
}

VOID RemoveFileFromPending(LPCWSTR filename)
{

}

int GetNumSuccess() {
    return SuccessList.size();
}

int GetNumFailed() {
    return FailedList.size();
}