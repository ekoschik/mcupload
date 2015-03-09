
#include "stdafx.h"
#include "MCuploader.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include "windows.h"
#include "stdio.h"

VOID InitUpload() { }

/*
    Well, this is what seemed to work:
    http://stackoverflow.com/questions/1011339/how-do-you-make-a-http-request-with-c
*/

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define PORT       3000
#define IP         "localhost"
#define RECEIVER   "/upload"

string getFile(LPCWSTR filepath) 
{
    std::ifstream input(filepath, std::ios::binary);
    string buffer((
        std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    string filedata64 = base64_encode(
        (unsigned char*)buffer.c_str(),
        buffer.size());

    return filedata64;
}

string getBody(string email, string filename, string filedata) {
    string body;
    body.append("{\"email\":\"");
    body.append(email.c_str());
    body.append("\",\"filename\":\"");
    body.append(filename.c_str());
    body.append("\",\"filedata\":\"");
    body.append(filedata.c_str());
    body.append("\"}");
    return body;
}

string getHeader(int bodysize)
{
    char header[1024];
    ZeroMemory(&header, 1024);
    sprintf(header,
        "POST %s HTTP/1.1\r\n"
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
    //Setup Connection
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed.\n";
        system("pause");
        return FALSE;
    }
    SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname(IP);
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(PORT);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
        cout << "Could not connect";
        system("pause");
        return FALSE;
    }

    //Setup Data To Be Sent
    string filedata = getFile(filepath);
    string body = getBody(ToStr(Email), ToStr(filename), filedata);
	//string body = "{ \"user\": \"brian\", \"file\": \"abcdefg\"}";
    string header = getHeader(body.size());

    //Send Request

    //why is THIS not working:
    send(Socket, header.c_str(), header.size(), 0);
    send(Socket, body.c_str(), body.size(), 0);

    //...but THIS is:  ?
    //char* test_header = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";    
    //send(Socket, test_header, strlen(test_header), 0);


    char buffer[10000];
    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0){
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
            cout << buffer[i];
            i += 1;
        }
    }
    closesocket(Socket);
    WSACleanup();

    system("pause");
    return TRUE;
}





//
//VOID InitUpload() { }
//
//string readFile(LPCWSTR filepath) 
//{
//    std::ifstream input(filepath, std::ios::binary);
//    string buffer((
//        std::istreambuf_iterator<char>(input)),
//        (std::istreambuf_iterator<char>()));
//    input.close();
//    return buffer;
//}
//
//string ConstructBody(string email, string filename, string filedata) {
//    string body;
//
//    body.append("{");
//
//    body.append("\"email\":\"");
//    body.append(email.c_str());
//    body.append("\",");
//
//    body.append("\"filename\":\"");
//    body.append(filename.c_str());
//    body.append("\",");
//
//    body.append("\"filedata\":");
//    body.append(filedata.c_str());
//    
//    body.append("}");
//
//    return body;
//}
//
//string ConstructHeader(int bodysize)
//{
//    char header[1024];
//    ZeroMemory(&header, 1024);
//    sprintf(header,
//        "POST %s HTTP 1.1\r\n"
//        "Host: %s\r\n"
//        "Content-Length: %d\r\n"
//        "Connection: Keep-Alive\r\n"
//        "Content-Type:application/json\r\n"
//        "Accept: application/json\r\n\r\n",
//        RECEIVER, IP, bodysize);
//    return string(header);
//}
//
//int err;
//BOOL UploadFile(LPCWSTR filepath, LPCWSTR filename)
//{
//    //WSAStartup()
//    WSADATA wsaData;
//    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (iResult != NO_ERROR) {
//        return FALSE;
//    }
//
//    //socket()
//    SOCKET dataSock = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
//    if (dataSock == INVALID_SOCKET) {
//        err = WSAGetLastError();
//        WSACleanup();
//        return FALSE;
//    }
//
//    //connect()
//
//    SOCKADDR_IN target;
//    struct hostent *host;
//    host = gethostbyname("localhost");
//    target.sin_addr.s_addr = *((unsigned long*)host->h_addr);
//    target.sin_family = AF_INET;
//    target.sin_port = htons(PORT);
//    iResult = connect(dataSock, 
//        (SOCKADDR*)&target, sizeof(target));
//    if (iResult == SOCKET_ERROR) {
//        err = WSAGetLastError();
//        closesocket(dataSock);
//        WSACleanup();
//        return FALSE;
//    }
//
//    //Read file, and encode in base64
//    string filedata = readFile(filepath);
//    string filedata64 = base64_encode(
//        (unsigned char*)filedata.c_str(),
//        strlen(filedata.c_str()));
//
//    //Construct Body and Header
//    string body = ConstructBody(ToStr(Email),
//                                ToStr(filename),
//                                filedata64.c_str());
//    int size = body.size();
//    string header = ConstructHeader(size);
//
//    //Send Data
//    int p = send(dataSock, header.c_str(), header.size(), 0);
//    int k = send(dataSock, body.c_str(), size, 0);
//
//    if (p == SOCKET_ERROR || k == SOCKET_ERROR) {
//        return FALSE;
//    }
//
//
//    //wait for response?
//
//    closesocket(dataSock);
//    WSACleanup();
//    return TRUE;
//}
