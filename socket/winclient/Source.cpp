#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#include<ctime>
#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")
void Get(SOCKET Socket, std::string user, std::string gold);
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

void Choose(SOCKET Socket, std::string user)
{
	std::cout << "Input date: ";
	// Input date
	std::cout << "\nChoose company: \n 1. SJC\n 2. DOJI\n 3. PNJ\n";
	int n;
	std::cin >> n;
	if (n == 1)
	{
		Get(Socket, user, "SJC");
	}
	else if (n == 2)
	{
		Get(Socket, user, "DOJI");
	}
	else if (n == 3)
	{
		Get(Socket, user, "PNJ");
	}
	else
		return;
}

void Get(SOCKET Socket, std::string user, std::string gold)
{
	time_t temp = time(0);
	tm date;
	localtime_s(&date, &temp);
	std::string line = "GET /" + gold + " username: " + user + " day: " + std::to_string(date.tm_mday) + "/" + std::to_string(date.tm_mon + 1) + "/" + std::to_string(date.tm_year + 1900);
	// GET /sjc username: 123 day: 22/7/2021
	std::cout << line;
	int result = send(Socket, line.c_str(), line.size() + 1, 0);
}

void Login(SOCKET Socket, std::string& user)
{
	std::string temp1, temp2, line;
	std::cout << "Username: ";
	std::cin >> temp1;
	std::cout << "Password: ";
	std::cin >> temp2;
	line = "IN /username: " + temp1 + " password: " + temp2;
	std::cout << line << "\n";
	user = temp1;
	int result = send(Socket, line.c_str(), line.size() + 1, 0);
}

void Logout(SOCKET Socket, std::string user)
{
	std::string line = "OUT /username: " + user;
	std::cout << line << "\n";
	int result = send(Socket, line.c_str(), line.size() + 1, 0);
	if (result >= 0)
		std::cout << "Logout successfully\n";
}

void Register(SOCKET Socket)
{
	std::string temp1, temp2, temp3, line;
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
	line = "REG /username: " + temp1 + "password: " + temp2;
	std::cout << line << "\n";
	int result = send(Socket, line.c_str(), line.size() + 1, 0);
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
	std::string user = "samuel";
	serverConnect(Socket, argv[1]);
	Choose(Socket, user);
	WSACleanup();
	closesocket(Socket);
	return 0;
}