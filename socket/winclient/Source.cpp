#include<WinSock2.h>
#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include<thread>
#include<ctime>
#include<iomanip>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")
void serverConnect(SOCKET& Socket, char* argv)
{
	int result;

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

	addrinfo hints, * addr = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo(argv, PORT, &hints, &addr);
	if (result != 0)
	{
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return;
	}
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

	//some thing here Socket
}

int senddata(int socket, const char* str) {
	std::string k = str;
	k += "\r";
	return send(socket, k.c_str(), k.size(), 0);
}

std::string revicedata(int sock) {
	int recive = 0;
	char a[1024];
	std::string buff = "";
	do {
		memset(a, 0, 1024);
		recive = recv(sock, a, 1024, 0);
		if (strstr(a, "\r") != nullptr) {
			char* b = strstr(a, "\r");
			char temp[1024] = "";
			int count = b - a;
			for (int i = 0; i < count; i++) {
				temp[i] = a[i];
			}
			buff += temp;
			break;
		}
		buff += a;
	} while (recive > 0);
	return buff;
}

std::string GetNow(std::string gold)
{
	return "GET " + gold + +"/day:now";
}

std::string GetDay(std::string gold, int day, int month, int year) {
	return "GET " + gold + +"/day:" + std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
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
		senddata(Socket, GetNow("sjc").c_str());
	}
	else if (n == 2)
	{
		senddata(Socket, GetNow("doji").c_str());
	}
	else if (n == 3)
	{
		senddata(Socket, GetNow("pnj").c_str());
	}
	else
		return;
}

std::string Login(std::string user)
{
	std::string temp1, temp2, line;
	std::cout << "Username: ";
	std::cin >> temp1;
	std::cout << "Password: ";
	std::cin >> temp2;
	line = "LOG username:" + temp1 + "/password:" + temp2;
	std::cout << line << "\n";
	user = temp1;
	return line;
}

std::string Logout(std::string user)
{
	return "OUT username: " + user;	
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
	line = "REG username:" + temp1 + "/password:" + temp2;
	std::cout << line << "\n";
	senddata(Socket, line.c_str());
}

int main(int argc, char** argv) 
{
	if (argc != 2)

	return 0;
}