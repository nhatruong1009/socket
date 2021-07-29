#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#include<sstream>
#include<winhttp.h>
#include<fstream>
#include<ctime>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/istreamwrapper.h"

#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment	(lib, "winhttp.lib")
 
std::wstring API_KEY = L"";

using namespace rapidjson;

struct User
{
	std::string username="";
	int socket;;
	bool check = true;
	std::string Respone="";
	char* lastRes = nullptr;
	User() {}	
	User(int socket) {
		this->socket = socket;
	}
	User(int socket, std::string username) {
		this->socket = socket;
		this->username = username;
	}
	void SendData( char* str ) {
		send(this->socket, str, strlen(str), 0);
	}
	std::string revicedata() {
		int recive = 0;
		char a[1024];
		std::string buff = "";
		do {
			memset(a, 0, 1024);
			recive = recv(this->socket, a, 1024, 0);
			buff += a;
		} while (recive > 0);
		return buff;
	}
	void CheckLive() {
		this->check = false;
		char a[] = { "CHECK" };
		send(this->socket, a, strlen(a), 0);
	}
	void ResponeCheck() {
		this->check = true;
	}
};

struct UserList
{
	User user;
	UserList* next;
	UserList(int socket,UserList*next=nullptr) {
		this->user.socket = socket;
		this->next = next;
	}
};

static UserList* user = nullptr;

static UserList* inUser(int socket) {		
	user = new UserList(socket, user);
	return user;
}

static bool outUser(int socket) {
	UserList* temp = user;
	if (temp->user.socket == socket) {
		user = user->next;
		// giao thuc huy ket noi
		delete temp;
		return true;
	}

	while (temp->next != nullptr && temp->next->user.socket != socket)
		temp = temp->next;
	if (temp->next == nullptr)
		return false;
	UserList* del = temp->next;
	temp->next = temp->next->next;
	//giao thuc huy ket noi
	delete del;
	return true;
}

static void checkAlive() {
	UserList* check = user;
	while (check)
	{
		check->user.CheckLive();
		check = check->next;
	}
}

static void sendToAll(char*str) {
	UserList* check = user;
	while (check)
	{
		check->user.SendData(str);
		check = check->next;
	}
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
	do {
		std::cout << "waiting";
		client = accept(listenSock, nullptr, nullptr);
	} while (client != INVALID_SOCKET);

	WSACleanup();
	return 0;
}

std::wstring TakeKey(HINTERNET connectSV) {
	std::string a = "";
	std::wstring mess = L"/api/request_api_key?scope=gold";
	HINTERNET request = WinHttpOpenRequest(connectSV, L"GET", mess.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	bool iresults = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	bool results = WinHttpReceiveResponse(request, NULL);
	DWORD dwSize = 0, dwDownloaded = 0;
	char* pszOutBuffer;
	if (results)
	{
		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(request, &dwSize))
				std::cout << "Error " << GetLastError() << " query data\n";
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				ZeroMemory(pszOutBuffer, dwSize + 1);
				if (!WinHttpReadData(request, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
					std::cout << "Error " << GetLastError() << " WinHttpReadData\n";
				else
					a += pszOutBuffer;
				delete[] pszOutBuffer;
			}

		} while (dwSize > 0);
	}
	if (request) WinHttpCloseHandle(request);
	std::wstring b = L"";
	if (a != "") {
		for (int i = a.find("\":\"") + 3; i < a.size() - 3; i++) {
			b += wchar_t(a[i]);
		}
	}
	return b;
}

void RefeshData_(HINTERNET connectSV, std::wstring gold,std::string &_result) {
	std::wstring mess = L"//api/v2/gold/" + gold + L"?api_key=";
	mess += API_KEY;
	HINTERNET hRequest = WinHttpOpenRequest(connectSV, L"GET", mess.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

	bool bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	std::string result = "";
	DWORD dwSize = 0, dwDownloaded = 0;
	char* pszOutBuffer;
	if (bResults)
	{	
		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				std::cout << "Error " << GetLastError() << " query data\n";
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				ZeroMemory(pszOutBuffer, dwSize + 1);
				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
					std::cout << "Error " << GetLastError() << " WinHttpReadData\n";
				else
					result += pszOutBuffer;
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}
	if (!bResults)
		std::cout << "Error " << GetLastError() << " has occurred\n";
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (result.find("Auth Error:") != -1) {
		API_KEY = TakeKey(connectSV);
		RefeshData_(connectSV,gold,_result);
		return;
	}
	_result = result;
}

void RefeshData(HINTERNET hConnect) {
	time_t timenow = time(0);
	tm now;
	localtime_s(&now, &timenow);
	std::string now_ = std::to_string(now.tm_hour) + "h" + std::to_string(now.tm_min);
	std::string today = std::to_string(now.tm_mday);
	std::string filename = std::to_string(now.tm_mon + 1) + "-" + std::to_string(now.tm_year + 1900) + ".json";
	std::string respone;
	RefeshData_(hConnect, L"pnj" ,respone);
	std::string temp = "{\"pnj\":" + respone;
	RefeshData_(hConnect, L"sjc", respone);
	temp += ",\"sjc\":" + respone;
	RefeshData_(hConnect, L"doji", respone);
	temp += ",\"doji\":" + respone + "}";

	std::ifstream fi(filename);
	IStreamWrapper isw(fi);
	Document b;
	b.ParseStream(isw);
	fi.close();
	if (b.HasParseError()) {
		b.SetObject();
	}
	if (!b.HasMember(today.c_str())) {
		Value empty;
		empty.SetObject();
		b.AddMember(StringRef(today.c_str()), empty, b.GetAllocator());
	}
	if (! b[today.c_str()].HasMember("sjc") ) {
		Value empty;
		empty.SetObject();
		b[today.c_str()].AddMember(rapidjson::StringRef("sjc"), empty, b.GetAllocator());
	}
	if (!b[today.c_str()].HasMember("pnj")) {
		Value empty;
		empty.SetObject();
		b[today.c_str()].AddMember(rapidjson::StringRef("pnj"), empty, b.GetAllocator());
	}
	if (!b[today.c_str()].HasMember("doji")) {
		Value empty;
		empty.SetObject();
		b[today.c_str()].AddMember(rapidjson::StringRef("doji"), empty, b.GetAllocator());
	}
	Document a;
	a.Parse(temp.c_str());
	Value::ConstMemberIterator gett = a.MemberBegin();
	for (; gett < a.MemberEnd(); gett++) {
		Value::ConstMemberIterator gold = a[gett->name.GetString()]["results"][0].MemberBegin();
		Value::Object exxample = a[gett->name.GetString()]["results"][0].GetObj();
		exxample.RemoveMember("datetime");
		b[today.c_str()][gett->name.GetString()].AddMember( rapidjson::StringRef(now_.c_str()), exxample, a.GetAllocator());
	}
	std::ofstream fileout(filename);
	OStreamWrapper fo(fileout);
	Writer<OStreamWrapper> write{ fo };
	b.Accept(write);
	fileout.close();
	a.RemoveAllMembers();
	b.RemoveAllMembers();
}

void startup() {
	HINTERNET  hSession = NULL, hConnect = NULL;
	hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"vapi.vnappmob.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
	RefeshData(hConnect);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}


void RefeshData2() {
	HINTERNET  hSession = NULL, hConnect = NULL;
	hSession = WinHttpOpen(L"WinHTTP Example/2", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"tygia.com", INTERNET_DEFAULT_HTTPS_PORT, 0);


	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"//json.php?ran=0&rate=0&gold=1&date=now", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

	bool bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	std::string result = "";
	DWORD dwSize = 0, dwDownloaded = 0;
	char* pszOutBuffer;
	if (bResults)
	{
		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				std::cout << "Error " << GetLastError() << " query data\n";
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				ZeroMemory(pszOutBuffer, dwSize + 1);
				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
					std::cout << "Error " << GetLastError() << " WinHttpReadData\n";
				else
					result += pszOutBuffer;
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}
	if (!bResults)
		std::cout << "Error " << GetLastError() << " has occurred\n";
	if (hRequest) WinHttpCloseHandle(hRequest);
	time_t timenow = time(0);
	tm now;
	localtime_s(&now, &timenow);
	std::string now_ = std::to_string(now.tm_hour) + "h" + std::to_string(now.tm_min);
	std::string today = std::to_string(now.tm_mday);
	std::string filename = "Gold_" + std::to_string(now.tm_mon + 1) + "_" + std::to_string(now.tm_year + 1900) + ".json";
	Document a;
	a.Parse(&result.c_str()[3]);
	std::ifstream old(filename);
	IStreamWrapper isw(old);
	Document b;
	b.ParseStream(isw);
	if (b.HasParseError()) 
		b.SetObject();
	if (!b.HasMember(today.c_str())) {
		Value empty;
		empty.SetObject();
		b.AddMember(StringRef(today.c_str()), empty, b.GetAllocator());
	}
	Value::Array temp = a["golds"][0]["value"].GetArray();
	for (int i = 0; i < temp.Capacity(); i++) {
		std::string k = a["golds"][0]["value"][i]["company"].GetString();
		if (!strcmp(k.c_str(), "1Coin")) {
			continue;
		}
		if (!b[today.c_str()].HasMember(k.c_str())) {
			Value empty;
			empty.SetObject();
			b[today.c_str()].AddMember(rapidjson::StringRef(a["golds"][0]["value"][i]["company"].GetString()), empty, b.GetAllocator());
		}
		std::string e = a["golds"][0]["value"][i]["brand"].GetString();
		if (!b[today.c_str()][k.c_str()].HasMember(e.c_str())) {
			Value empty;
			empty.SetObject();
			b[today.c_str()][k.c_str()].AddMember(rapidjson::StringRef(a["golds"][0]["value"][i]["brand"].GetString()), empty, b.GetAllocator());
		}
		Value empty;
		empty.SetObject();
		Value buy;
		buy.SetString(rapidjson::StringRef(a["golds"][0]["value"][i]["buy"].GetString()));		
		Value sell;
		sell.SetString(rapidjson::StringRef(a["golds"][0]["value"][i]["sell"].GetString()));
		b[today.c_str()][k.c_str()][e.c_str()].AddMember(StringRef(now_.c_str()), empty, b.GetAllocator());
		b[today.c_str()][k.c_str()][e.c_str()][now_.c_str()].AddMember(rapidjson::StringRef("buy"), buy, b.GetAllocator());
		b[today.c_str()][k.c_str()][e.c_str()][now_.c_str()].AddMember(rapidjson::StringRef("sell"), sell, b.GetAllocator());
	}

	std::ofstream fileout(filename);
	OStreamWrapper fo(fileout);
	Writer<OStreamWrapper> write{ fo };
	b.Accept(write);
	fileout.close();
	a.RemoveAllMembers();
	b.RemoveAllMembers();

	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	if (hRequest) WinHttpCloseHandle(hRequest);
}

int main() {
}