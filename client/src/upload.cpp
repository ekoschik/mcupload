//
#include "stdafx.h"
//#include "MCuploader.h"
//
//#include <iostream>
//#include <string>
//#include <fstream>
//#include "stdio.h"
//
//using namespace std;
//
//#define RECEIVER   "/screenshots"
//
//
//CHAR ipbuf[1000];
//const char* ReadIP() {
//    ZeroMemory(&ipbuf, 1000);
//    wcstombs((char*)&ipbuf, UD.servername.c_str(), UD.servername.size());
//    return (const char*)&ipbuf;
//}
//
//unsigned short ReadPort() {
//    unsigned short port = wcstoul(UD.port.c_str(), NULL, 0);
//    return port;
//}
//
//
//
//BOOL HandleServerResponse(string response, LPCWSTR filepath, LPCWSTR filename)
//{
//    return (response[9] == '2');
//}
//
//
//string getBody(string username, string filename, string filedata) {
//    string body;
//    body.append("{\"user\":\"");
//    body.append(username.c_str());
//    body.append("\",\"filename\":\"");
//    body.append(filename.c_str());
//    body.append("\",\"filedata\":\"");
//    body.append(filedata.c_str());
//    body.append("\"}");
//    return body;
//}
//
//string getFile(LPCWSTR filepath) 
//{
//    std::ifstream input(filepath, std::ios::binary);
//    string buffer((
//        std::istreambuf_iterator<char>(input)),
//        (std::istreambuf_iterator<char>()));
//    input.close();
//
//    string filedata64 = base64_encode(
//        (unsigned char*)buffer.c_str(),
//        buffer.size());
//
//    return filedata64;
//}
//
//string getHeader(int bodysize)
//{
//    char header[1024];
//    ZeroMemory(&header, 1024);
//    sprintf(header,
//        "POST %s HTTP/1.1\r\n"
//        "Host: %s\r\n"
//        "Content-Length: %d\r\n"
//        "Connection: Keep-Alive\r\n"
//        "Content-Type:application/json\r\n"
//        "Accept: application/json\r\n\r\n",
//        RECEIVER, ReadIP(), bodysize);
//    return string(header);
//}
//
//
//SOCKET GetSocket() 
//{
//    //Setup Connection
//    WSADATA wsaData;
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        return NULL;
//    }
//
//    //Create Socket
//    SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    
//    //Resolve host name
//    struct hostent *host;
//    host = gethostbyname(ReadIP());
//    if (host == NULL) {
//        return NULL;
//    }
//
//    //Connect to socket
//    SOCKADDR_IN SockAddr;
//    SockAddr.sin_port = htons(ReadPort());
//    SockAddr.sin_family = AF_INET;
//    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
//    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
//        return NULL;
//    }
//
//    return Socket;
//}
//
//BOOL UploadFile(LPCWSTR filepath, LPCWSTR filename, SOCKET Socket)
//{
//    if (Socket == NULL) {
//        return FALSE;
//    }
//
//    //Setup Data To Be Sent
//    string filedata = getFile(filepath);
//    string body = getBody(ToStr(UD.username.c_str()), 
//                          ToStr(filename), 
//                          filedata);
//	string header = getHeader(body.size());
//
//    //Send Request
//    int r1 = send(Socket, header.c_str(), header.size(), 0);
//    int r2 = send(Socket, body.c_str(), body.size(), 0);
//
//
//    //Read response
//    const int returnbufsize = 100;
//    char buffer[returnbufsize] = {};
//    int nDataLength;
//    string ret;
//    do {
//        ZeroMemory(&buffer, returnbufsize);
//        nDataLength = recv(Socket, buffer, returnbufsize, 0);
//        ret += buffer;
//
//        if (nDataLength == SOCKET_ERROR) {
//            Error(_T("recv returned SOCKET_ERROR"));
//        }
//
//    } while (nDataLength == returnbufsize);
//        //TODO why does recv fail if called one more time???
//
//
//    //Handle response and return
//    return HandleServerResponse(ret, filepath, filename);
//
//}
//
//VOID CloseConnection(SOCKET Socket)
//{
//    closesocket(Socket);
//    WSACleanup();
//}