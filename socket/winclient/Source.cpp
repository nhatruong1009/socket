#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")

void serverConnect(SOCKET& Socket, char* argv)
{
	int result;
	//-------------------------
	// Khởi tạo WSA (cần thiết để khởi tạo socket)
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
		std::cout << "WSA initialize failed" << std::endl;
		return;
	}
	else
	{
		std::cout << "WSA initialize successfully" << std::endl;
	}

	//-------------------------
	addrinfo hints, * addr = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	result = getaddrinfo(argv, PORT, &hints, &addr);
	if (result != 0)
	{
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return;
	}

	//-------------------------
	// Initialize Socket
	// Attempt to connect to an address until one succeeds
	for (addrinfo* ptr = addr; ptr != nullptr; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (Socket == INVALID_SOCKET)
		{
			std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return;
		}

		// Connect to server.
		result = connect(Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			closesocket(Socket);
			Socket = INVALID_SOCKET;
		}
		else
			break;
	}
	freeaddrinfo(addr);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Unable to connect to server!\n";
		WSACleanup();
		return;
	}

	//WSACleanup();	// 10093
	//closesocket(Socket);	// 10038
}

void Get(SOCKET Socket, char* user)
{

}

void Login(SOCKET Socket)
{
	std::string temp1, temp2;
	std::cout << "Username: ";
	std::cin >> temp1;
	std::cout << "Password: ";
	std::cin >> temp2;
	int n1 = temp1.size() + 1, n2 = temp2.size() + 1;
	char* user = new char[n1];
	char* pass = new char[n2];
	strcpy_s(user, n1, temp1.c_str());
	strcpy_s(pass, n2, temp2.c_str());
	std::cout << user << "\n" << pass <<"\n";
	char KEY[2];
	int result;
	result = send(Socket, "IN", 3, 0);
	send(Socket, user, n1, 0);
	send(Socket, user, n2, 0);
	result = recv(Socket, KEY, 2, 0);
	if (result >= 0 && KEY == "1")
		std::cout << "Login successfully\n";
	else std::cout << "Login failed\n";

}

void Logout(SOCKET Socket, char* user)
{
	int result = send(Socket, user, strlen(user), 0);
	do
	{
		result = send(Socket, "OUT", 4, 0);
	} while (result < 0);
	std::cout << "Logout successfully\n";
}

void Register(SOCKET Socket)
{
	std::string temp1, temp2, temp3;
	do
	{
		std::cout << "Username: ";
		std::cin >> temp1;
		std::cout << "Password: ";
		std::cin >> temp2;
		std::cout << "Confirm paswword: ";
		std::cin >> temp3;
		if (temp2 != temp3)
		{
			system("cls");
			std::cout << "Confirm password does not match your password\n";
		}
	} while (temp2 != temp3);
	int n1 = temp1.size() + 1, n2 = temp2.size() + 1;
	char* user = new char[n1];
	char* pass = new char[n2];
	strcpy_s(user, n1, temp1.c_str());
	strcpy_s(pass, n2, temp2.c_str());
	std::cout << user << "\n" << pass << "\n";
	char KEY[2];
	int result;
	result = send(Socket, "REG", 4, 0);
	if (result < 0)
	{
		std :: cout << "send failed with error:" << WSAGetLastError() << std::endl;
	}
	send(Socket, user, n1, 0);
	send(Socket, user, n2, 0);
	result = recv(Socket, KEY, 2, 0);
	if (result >= 0 && KEY == "1")
		std::cout << "Register successfully\n";
	else std::cout << "Register failed\n";
}

int main(int argc, char** argv) 
{
	//if (argc != 2)
	//{
	//	std::cout << "Missing server ip\n";
	//	return 1;
	//}
	SOCKET Socket = 1;
	//-------------------------
	/*getStarted(Socket, argv[1]);*/
	serverConnect(Socket, argv[1]);
	Register(Socket);
	WSACleanup();
	closesocket(Socket);
	return 0;
}


