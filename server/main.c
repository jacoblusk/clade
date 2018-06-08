#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>
#include <winsock2.h>

#include "../ioutil.h"

#define BUFFER_SIZE 1024

void CALLBACK CompletionROUTINE(
  DWORD dwError, 
  DWORD cbTransferred, 
  LPWSAOVERLAPPED lpOverlapped, 
  DWORD dwFlags
) {
    UNREFERENCED_PARAMETER(dwError);
    UNREFERENCED_PARAMETER(dwFlags);
    UNREFERENCED_PARAMETER(lpOverlapped);

    CHAR *buffer = (CHAR *)lpOverlapped->hEvent;
    for(DWORD i = 0; i < cbTransferred; i++) {
        PrintfW(L"%d\n", buffer[i]);
    }

    PrintfA2W("%5s", buffer);
}


VOID start(VOID) {
    WSADATA wsaData;
    WORD wVersionRequested;
    WSABUF wsaBuffer;
    SOCKET listenSocket;
    SOCKADDR_IN service;
    SOCKADDR_IN sender;
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
    DWORD cbTransfered;

    iResult = WSARecvFrom(
        listenSocket, 
        &wsaBuffer, 
        1,
        (LPDWORD) NULL, 
        &dwFlags, 
        (PSOCKADDR)&sender, 
        &senderAddrSize, 
        &overlapped,
        CompletionROUTINE
    );

    if(iResult != 0) {
        BOOL bResult;
        INT error = WSAGetLastError();
        if(error != WSA_IO_PENDING) {
            ErrorfW(L"WSARecvFrom failed with error: %ld\n", error);
            goto fail;
        }

        while(TRUE) {
            bResult = WSAGetOverlappedResult(listenSocket, &overlapped, &cbTransfered, FALSE, &dwFlags);
            if(bResult)
                break;
        }
    }

  fail:
    closesocket(listenSocket);
    WSACleanup();
    ExitProcess(EXIT_SUCCESS);
}