#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>
#include <winsock2.h>

#include "../ioutil.h"

#define BUFFER_SIZE 1024

VOID start(VOID) {
    WSADATA wsaData;
    WORD wVersionRequested;
    WSABUF wsaBuffer;
    SOCKET listenSocket;
    SOCKADDR_IN service;
    SOCKADDR_IN sender;
    WSAEVENT aEvent[WSA_MAXIMUM_WAIT_EVENTS];
    WSAOVERLAPPED overlapped;
    CHAR recieveBuffer[BUFFER_SIZE];
    INT senderAddrSize;
    DWORD dwFlags;
    INT iResult;
    dwFlags = 0;

    overlapped.hEvent = &recieveBuffer[0];
    wVersionRequested = MAKEWORD(2, 2);

    if((iResult = WSAStartup(wVersionRequested, &wsaData))) {
        ErrorfW(L"WSAStartup failed with: %d\n", iResult);
    }

    listenSocket = WSASocketW(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(listenSocket == INVALID_SOCKET) {
        ErrorfW(L"WSASocket returned INVALID_SOCKET, error: %d\n", WSAGetLastError());
        goto fail;
    }

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    service.sin_port = htons(5150);

    iResult = bind(listenSocket, (const PSOCKADDR)&service, sizeof(service));
    if(iResult == SOCKET_ERROR) {
        ErrorfW(L"bind failed with error: %u\n", WSAGetLastError());
        closesocket(listenSocket);
        goto fail;
    }

    wsaBuffer.buf = recieveBuffer;
    wsaBuffer.len = BUFFER_SIZE;
    senderAddrSize = sizeof(sender);

    while(TRUE) {
        iResult = WSARecvFrom(
            listenSocket, 
            &wsaBuffer, 
            1,
            (LPDWORD) NULL, 
            &dwFlags, 
            (PSOCKADDR)&sender, 
            &senderAddrSize, 
            &overlapped,
            NULL
        );

        if(iResult != WSA_IO_PENDING) {
            ErrorfW(L"WSARecvFrom failed with error: %ld\n", error);
        }
    }

  fail:
    closesocket(listenSocket);
    WSACleanup();
    ExitProcess(EXIT_SUCCESS);
}