
#include "stdafx.h"
#include "MCuploader.h"

#include <iostream>
#include <string>
#include <fstream>
#include "stdio.h"

VOID InitUpload() 
{ 
    
}

using namespace std;

#define PORT       3000
#define IP         "localhost"
#define RECEIVER   "/upload"

string getBody(string username, string filename, string filedata) {
    string body;
    body.append("{\"user\":\"");
    body.append(username.c_str());
    body.append("\",\"filename\":\"");
    body.append(filename.c_str());
    body.append("\",\"filedata\":\"");
    body.append(filedata.c_str());
    body.append("\"}");
    return body;
}

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


SOCKET GetSocket() 
{
    //Setup Connection
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return NULL;
    }

    //socket() and connect()
    SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname(IP);
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(PORT);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
        return NULL;
    }

    return Socket;
}

BOOL UploadFile(LPCWSTR filepath, LPCWSTR filename, SOCKET Socket)
{
    if (Socket == NULL) {
        return FALSE;
    }

    //Setup Data To Be Sent
    string filedata = getFile(filepath);
    string body = getBody(ToStr(UD.username.c_str()), 
                          ToStr(filename), 
                          filedata);
	string header = getHeader(body.size());

    //Send Request
    send(Socket, header.c_str(), header.size(), 0);
    send(Socket, body.c_str(), body.size(), 0);


    //Read response
    char buffer[10000] = { };
    int nDataLength;
    nDataLength = recv(Socket, buffer, 10000, 0);

    //Jank City (...read response code)
    char* retStatus = buffer + 9;
    BOOL success = (retStatus[0] == '2');

    //Cleanup and return error code
    closesocket(Socket);
    WSACleanup();
    return success;
}
