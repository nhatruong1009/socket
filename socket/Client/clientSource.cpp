#include "clientSocket.h"

void MarshalString(System::String^ s, std::string& os) {
	using namespace System::Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}


bool serverConnect(int& Socket, std::string ip)
{
	int result;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
		return false;
	}
	addrinfo hints, * addr = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo(ip.c_str(), PORT, &hints, &addr);
	if (result != 0)
	{
		WSACleanup();
		return false;
	}
	for (addrinfo* ptr = addr; ptr != nullptr; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (Socket == INVALID_SOCKET)
		{
			WSACleanup();
			return false;
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
		WSACleanup();
		return false;
	}
	return true;

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


std::string Log(std::string user, std::string password)
{
	return "LOG username:" + user + "/password:" + password;
}

std::string Logout(std::string user)
{
	return "OUT username:" + user;
}

std::string Register(std::string user, std::string password)
{
	return "REG username:" + user + "/password:" + password;
}