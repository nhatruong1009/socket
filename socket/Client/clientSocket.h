#ifndef _SUP
#define _SUP
#include<WinSock2.h>
#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include<thread>
#include<ctime>
#include<iomanip>
#include<Windows.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace rapidjson;
#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")

	void MarshalString(System::String^ s, std::string& os);
	bool serverConnect(int& Socket, std::string ip);
	int senddata(int socket, const char* str);
	std::string revicedata(int sock);
	std::string GetNow(std::string gold);
	std::string GetDay(std::string gold, int day, int month, int year);
	std::string Log(std::string user, std::string password);
	std::string Logout(std::string user);
	std::string Register(std::string user, std::string password);

#endif // !_SUP
