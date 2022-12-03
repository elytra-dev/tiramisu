#ifndef WIN32_LEAN_AND_MEAN //Differentiates between Windows.h and Winsock as there are Windows.h functions in Winsock.
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <winsock2.h> //Winsock library containing most of winsock's functions and structures.
#include <ws2tcpip.h> //Addition to winsock library with newer functions and structures for Port/IP connections.
#include <stdio.h> //For printf() and related output functions

#pragma comment(lib, "Ws2_32.lib") //Linking to winsock 32 library
#define DEFAULT_PORT "27015"

#define DEFAULT_BUFLEN 512
int main(int argc, char** argv) { 
	WSAData wsaDATA;

	char recvbuf[DEFAULT_BUFLEN] = "";
	int iSendResult = 0;
	char sendbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	memset(recvbuf, 0, DEFAULT_BUFLEN);
	int iResult; 

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaDATA);
	if (iResult != 0) {
		printf("WSAStartup failed: %d", iResult);
		return 1;
	}

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Bind failed: %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("Accept failed: %d", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	
	std::string connected = "Client connected.";
	iResult = send(ClientSocket, connected.c_str(), DEFAULT_BUFLEN, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Client has joined succesfully." << std::endl;

	do {
		iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			printf("Client: %s\n", recvbuf);
			std::cout << "You: ";
			std::cin.getline(sendbuf, DEFAULT_BUFLEN);
			iResult = send(ClientSocket, sendbuf, DEFAULT_BUFLEN, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		}
		else if (iResult == 0) {
			printf("Connection closing...");

		}
		else {
			printf("recv failed %d", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
		
	} while (iResult > 0);

	if (recvbuf == "%exit") {
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}
