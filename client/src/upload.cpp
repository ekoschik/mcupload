
#include "stdafx.h"
#include "MCuploader.h"

#include <iostream>
#include <winsock2.h>
#include <string>
#include <fstream>
#include <string>
#include "windows.h"
#include "stdio.h"

using namespace std;

#define PORT       3000
#define IP         "127.0.0.1"
#define RECEIVER   "/upload"
#define HOST       "localhost"


VOID InitUpload() { }

string readFile(LPCWSTR filepath) {
    string fileName = ToStr(filepath);
    string fileContents;
    ifstream tmp(fileName.c_str());
    getline(tmp, fileContents);
    tmp.close();
    return fileContents;
}

string ConstructBody(string email, string filename, string filedata) {
    string body;

    body.append("{");

    body.append("\"email\":\"");
    body.append(email.c_str());
    body.append("\",");

    body.append("\"filename\":\"");
    body.append(filename.c_str());
    body.append("\",");

    body.append("\"filedata\":");
    body.append(filedata.c_str());
    
    body.append("}");

    return body;
}

string ConstructHeader(int bodysize)
{
    char header[1024];
    ZeroMemory(&header, 1024);
    sprintf(header,
        "POST %s HTTP 1.1\r\n"
        "Host: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: Keep-Alive\r\n"
        "Content-Type:application/json\r\n"
        "Accept: application/json\r\n\r\n",
        RECEIVER, IP, bodysize);
    return string(header);
}

BOOL UploadFile(LPCWSTR filepath, LPCWSTR filename)
{
    //WSAStartup()
    SOCKET dataSock;
    WSADATA wsaData;
    if(WSAStartup(0x0202, &wsaData) != 0) {
        return FALSE;
    }

    //socket()
    SOCKADDR_IN target;
    target.sin_family = AF_INET;
    target.sin_port = htons(PORT);
    target.sin_addr.s_addr = inet_addr(IP);
    dataSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (dataSock == INVALID_SOCKET) {
        return FALSE;
    }

    //connect()
    if (connect(dataSock, (SOCKADDR*)&target, sizeof(target)) != 0) {
        return FALSE;
    }

    //Read file, and encode in base64
    string filedata = readFile(filepath);
    string filedata64 = base64_encode(
        (unsigned char*)filedata.c_str(),
        strlen(filedata.c_str()));

    //Construct Body and Header
    string body = ConstructBody(ToStr(Email),
                                ToStr(filename),
                                filedata64.c_str());
    string header = ConstructHeader(body.size());

    //Send Data
    int p = send(dataSock, header.c_str(), header.size(), 0);
    int k = send(dataSock, body.c_str(), strlen(body.c_str()), 0);

    if (p == SOCKET_ERROR || k == SOCKET_ERROR) {
        return FALSE;
    }


    //wait for response?

    closesocket(dataSock);
    WSACleanup();
}

