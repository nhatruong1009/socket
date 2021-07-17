#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")

void refeshData() {
	WORD version = MAKEWORD(2, 2);
	WSADATA wsa;
	int temp = WSAStartup(version, &wsa);
	addrinfo hints;
	addrinfo* result = nullptr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int ck = getaddrinfo("206.189.157.201", "443", &hints, &result);
	std::cout << ck;

	int connectSock;

	for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		connectSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSock == INVALID_SOCKET) {
			std::cout << "Socket fail: " << WSAGetLastError();
			WSACleanup();
			return;
		}

		ck = connect(connectSock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (ck == SOCKET_ERROR) {
			closesocket(connectSock);
			connectSock = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (connectSock == INVALID_SOCKET) {
		std::cout << "Unable to connect to sever\n";
		WSACleanup();
		return;
	}

	std::string k = "GET /api/request_api_key?scope=exchange_rate HTTP/2\r\nHost: vapi.vnappmob.com\r\n";
	char a[1024];
	memset(a, 0, 1024);
	send(connectSock, k.c_str(), k.length(), 0);
	recv(connectSock, a, 1024, 0);
	std::cout << '\n' << a;

}

void clientConnect(int client) {


	closesocket(client);
}

int sever() {
	WSADATA wsa;
	addrinfo* result = nullptr;
	addrinfo hints;
	int ck;
	WORD version = MAKEWORD(2, 2);
	ck = WSAStartup(version, &wsa);
	if (result != 0) {
		std::cout << "WSAtarup failed " << ck;
		return 1;
	}
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	ck = getaddrinfo(nullptr, PORT, &hints, &result);
	if (ck != 0) {
		std::cout << "Getaddrinfo fail: " << ck;
		return 1;
	}

	int listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSock == INVALID_SOCKET) {
		std::cout << "Listen fail: " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	ck = bind(listenSock, result->ai_addr, (int)result->ai_addrlen);
	if (ck == SOCKET_ERROR) {
		std::cout << "bind fail: " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	ck = listen(listenSock, SOMAXCONN);
	if (ck == SOCKET_ERROR) {
		std::cout << "listen fail: " << WSAGetLastError();
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	int client;
	client = accept(listenSock, nullptr, nullptr);
	if (client == INVALID_SOCKET) {
		std::cout << "accept error: " << WSAGetLastError();
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	do {
		std::thread conec(clientConnect, client);
		conec.detach();
		client = accept(listenSock, nullptr, nullptr);
	} while (client != INVALID_SOCKET);


	WSACleanup();
	return 0;
}

int main() {
	refeshData();
}