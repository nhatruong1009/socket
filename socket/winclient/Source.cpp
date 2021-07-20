#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#define PORT "2000"

void severConnect(int sever) {

	closesocket(sever);
}
int main(int argc, char** argv) {
	if (argc < 2) return 0;
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

	ck = getaddrinfo(nullptr, PORT, &hints, &result);
	if (ck != 0) {
		std::cout << "getaddrinfo fail: " << ck;
		WSACleanup();
		return 1;
	}
	int connectSock;
	for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		connectSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSock == INVALID_SOCKET) {
			std::cout << "Socket fail: " << WSAGetLastError();
			WSACleanup();
			return 1;
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
		return 1;
	}
	return 0;
}

