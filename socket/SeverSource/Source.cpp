#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#include<sstream>
#include<winhttp.h>
#include<fstream>
#include<ctime>
#include<queue>
#include<mutex>
#include<codecvt>
#include <conio.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/istreamwrapper.h"

#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment	(lib, "winhttp.lib")

std::wstring API_KEY = L"";
std::mutex mtx;
using namespace rapidjson;
static bool outUser(int socket);

std::wstring _Province = L"";
std::wstring _Goldname = L"";

int AccountREGLOG(std::string user, std::string password, bool reg = false) {
	mtx.lock();
	int result = 0;
	std::fstream ac("account.txt", std::ios_base::in);
	if (!ac) { std::fstream fo("account.txt", std::ios_base::out); fo.close(); ac.open("account.txt", std::ios_base::in); }
	std::string tk = "", mk;
	while (!ac.eof() && tk != user)
	{
		ac >> tk;
		ac >> mk;
	}
	ac.close();
	if (tk == user && mk == password) {
		result = 1;
	}
	else if (tk == user && mk != password) {
		result = 0;
	}
	if (tk != user) {
		if (reg) {
			ac.open("account.txt", std::ios_base::app);
			ac << user << " " << password << '\n';
		}
		result = -1;
	}
	mtx.unlock();
	return result;
}

bool LOG(const char* resquest) {
	std::stringstream loc(resquest);
	std::string username, password;
	std::getline(loc, username, ':');
	std::getline(loc, username, '/');
	std::getline(loc, password, ':');
	std::getline(loc, password, '/');
	if (AccountREGLOG(username, password) == 1)
		return true;
	return false;
}

bool REG(const char* resquest) {
	std::stringstream loc(resquest);
	std::string username, password;
	std::getline(loc, username, ':');
	std::getline(loc, username, '/');
	std::getline(loc, password, ':');
	std::getline(loc, password, '/');

	if (AccountREGLOG(username, password, true) == -1) {
		return true;
	}
	return false;
}

std::string GetData(const char* data) { //sjc , pnj & doji only
	mtx.lock();
	std::cout << data << '\n';
	std::stringstream sstr(data);
	std::string type,gold, day, month, year, result;
	std::getline(sstr, type, ':');
	std::getline(sstr, type, '/');
	std::getline(sstr, gold, ':');
	std::getline(sstr, gold, '/');
	std::getline(sstr, day, ':');
	std::getline(sstr, day, '/');
	if (day == "now") {
		time_t timenow = time(0);
		tm now;
		localtime_s(&now, &timenow);
		day = std::to_string(now.tm_mday);
		month = std::to_string(now.tm_mon+1);
		year = std::to_string(now.tm_year + 1900);
	}
	else {
		day = std::to_string(std::stoi(day));
		std::getline(sstr, month, '/');
		month = std::to_string(std::stoi(month));
		std::getline(sstr, year, '/');
		year = std::to_string(std::stoi(year));
	}

	std::string filename;
	if (type == "0") {
		filename = month + "-" + year + ".json";
		std::ifstream fi(filename);
		IStreamWrapper isw(fi);
		Document b;
		b.ParseStream(isw);
		fi.close();
		if (b.HasParseError()) result = "Fail";
		else {
			if (!b.HasMember(day.c_str())) result = "Fail";
			else {
				Value::ConstMemberIterator value = b[day.c_str()].MemberBegin();
				for (int i = 0; i < std::stoi(gold); i++) {
					value++;
				}
				if (value >= b[day.c_str()].MemberEnd()) result = "Fail";
				else {
					Value::ConstMemberIterator datasend = value->value.MemberEnd();
					datasend--;
					rapidjson::StringBuffer buffer;
					buffer.Clear();
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					datasend->value.Accept(writer);
					result = _strdup(buffer.GetString());
				}
			}
		}
		b.GetAllocator().~MemoryPoolAllocator();
	}
	else {
		filename = "Gold_" + month + "_" + year + ".json";
		std::ifstream fi(filename);
		IStreamWrapper isw(fi);
		Document b;
		b.ParseStream(isw);
		Document a;
		a.SetObject();
		fi.close();
		if (b.HasParseError()) result = "Fail";
		else {
			if (!b.HasMember(day.c_str())) result = "Fail";
			else {
				Value::ConstMemberIterator value = b[day.c_str()].MemberBegin();
				for (int i = 0; i < std::stoi(gold); i++) {
					value++;
				}
				if (value > b[day.c_str()].MemberEnd()) result = "Fail";
				else {
					Value::ConstMemberIterator data = value->value.MemberBegin();
					for (; data < value->value.MemberEnd(); data++) {
						Value::ConstMemberIterator buyandsell = data->value.MemberEnd();
						buyandsell--;
						Value temp;
						temp.CopyFrom(buyandsell->value, b.GetAllocator());
						a.AddMember(rapidjson::StringRef(data->name.GetString()), temp, a.GetAllocator());
					}
					rapidjson::StringBuffer buffer;
					buffer.Clear();
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					a.Accept(writer);
					result = _strdup(buffer.GetString());
				}
			}
		}
		b.GetAllocator().~MemoryPoolAllocator();
	}
	mtx.unlock();
	return result;
}

std::string splitCommand(std::string& buff) {
	int n = buff.find("\r");
	if (n == -1) return "";
	else {
		std::string result;
		result.resize(n);
		for (int i = 0; i < n; i++) {
			result[i] = buff[i];
		}
		std::string bufftemp;
		int m = buff.length() - n - 1;
		bufftemp.resize(m);
		for (int i = 0; i < m; i++) {
			bufftemp[i] = buff[n + i + 1];
		}
		buff = bufftemp;
		return result;
	}
}

bool checkTaken(std::string username_);
class User
{

private:
	std::string revbuff = "";
public:
	std::string username = "";
	std::string LastResquest = "";
	std::string LastRespone = "";
	int socket;
	char* lastRes = nullptr;
	User() {}
	User(int socket) {
		this->socket = socket;
	}
	User(int socket, std::string username) {
		this->socket = socket;
		this->username = username;
	}
	void SendData(const char* str, int n = 0) {
		if (n == 0) {
			std::string k = str;
			k += "\r";
			send(this->socket, k.c_str(), k.size(), 0);
		}
		else {
			send(this->socket, (char*)&n, 4, 0);
			send(this->socket, str, n, 0);
			send(this->socket, "\r", 1, 0);
		}
	}
	std::string revicedata(bool& check) {
		int recive = 0;
		char a[1024];
		std::string buff = splitCommand(revbuff);
		if (buff == "") {
			do {
				memset(a, 0, 1024);
				recive = recv(this->socket, a, 1024, 0);
				if (recive == -1) { check = false; break; }
				if (strstr(a, "\r") != nullptr) {
					char* b = strstr(a, "\r");
					char temp[1024] = "";
					int count = b - a;
					for (int i = 0; i < count; i++) {
						temp[i] = a[i];
					}
					buff += temp;
					char temp2[1024] = "";
					for (int i = count + 1; i < recive; i++) {
						temp2[i - count - 1] = a[i];
					}
					revbuff = temp2;
					break;
				}
				buff += a;
			} while (recive > 0);
		}
		return buff;
	}

	void Response(const char* resquest) {
		std::cout << this->socket << " resquest ";
		if (strncmp(resquest, "LOG", 3) == 0) {
			std::cout << "LOG\n";
			std::stringstream loc(resquest);
			std::string username_;
			std::getline(loc, username_, ':');
			std::getline(loc, username_, '/');
			if (checkTaken(username_)) {
				LastRespone = "Taken";
				this->SendData("Taken");
			}
			else if (LOG(&resquest[4]) == true) {
				LastRespone = "Accept";
				this->SendData("Accept");
				this->username = username_;
			}
			else { this->SendData("Deny"); this->LastRespone = "Deny"; }
			return;
		}
		if(this->LastResquest.compare(resquest) == 0) { std::cout << "as same as last request\n"; this->SendData(LastRespone.c_str()); return; }
		LastResquest = resquest;
		if (strncmp(resquest, "REG", 3) == 0) {
			std::cout << "REG\n";
			if (REG(&resquest[4])) {
				this->SendData("Accept");
				this->LastRespone = "Accept";
			}
			else {
				this->LastRespone = "Deny"; this->SendData("Deny");
			}
		}
		else if (this->username == "") { this->SendData("Deny"); this->LastRespone = "Deny"; }
		else if (strcmp(resquest, "NAME") == 0) {
			this->SendData((char*)&*_Goldname.c_str(), _Goldname.size() * 2);
			this->SendData((char*)&*_Province.c_str(), _Province.size() * 2);

		}
		else if (strncmp(resquest, "GET", 3) == 0) {
			std::cout << "GET\n";
			this->LastRespone = GetData(resquest);
			this->SendData(this->LastRespone.c_str());
			//this->SendData("Accept");
		}
		else { this->SendData("Deny"); this->LastRespone = "Deny"; }
	}

};

class UserList
{
public:
	User user;
	UserList* next;
	std::thread* userthread = nullptr;
	UserList(int socket, UserList* next = nullptr) {
		this->user.socket = socket;
		this->next = next;
	}
};

static UserList* user = nullptr;

static UserList* inUser(int socket) {
	mtx.lock();
	user = new UserList(socket, user);
	mtx.unlock();
	return user;
}

static bool outUser(int socket) {
	mtx.lock();
	UserList* temp = user;
	if (temp->user.socket == socket) {
		user = user->next;
		closesocket(temp->user.socket);
		delete temp;
		mtx.unlock();
		return true;
	}

	while (temp->next != nullptr && temp->next->user.socket != socket)
		temp = temp->next;
	if (temp->next == nullptr)
	{
		mtx.unlock();
		return false;
	}
	UserList* del = temp->next;
	temp->next = temp->next->next;

	closesocket(del->user.socket);
	delete del;
	mtx.unlock();
	return true;
}

bool checkTaken(std::string username_) {
	UserList *check = user;
	while (check != nullptr)
	{
		if (check->user.username.compare(username_) == 0) {
			return true;
		}
		check = check->next;
	}
	return false;
}

static void sendToAll(char* str) {
	UserList* check = user;
	while (check)
	{
		check->user.SendData(str);
		check = check->next;
	}
}

void threadStartClient(User* client) {
	bool check = true;
	do {
		std::string request= client->revicedata(check).c_str();
		if ( strncmp(request.c_str(),"OUT",3) == 0) {
			outUser(client->socket);
			return;
		}
		client->Response(request.c_str());
		if (!check) break;
	} while (1);
	outUser(client->socket);
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
	ZeroMemory(&hints, sizeof(hints));
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
	ck = listen(listenSock, 1000);
	if (ck == SOCKET_ERROR) {
		std::cout << "listen fail: " << WSAGetLastError();
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	int client;
	do {
		std::cout << "waiting\n";
		client = accept(listenSock, nullptr, nullptr);

		inUser(client);
		user->userthread = new std::thread(threadStartClient, &user->user);
		user->userthread->detach();
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

void RefeshData_(HINTERNET connectSV, std::wstring gold, std::string& _result) {
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
		RefeshData_(connectSV, gold, _result);
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
	RefeshData_(hConnect, L"pnj", respone);
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
	if (!b[today.c_str()].HasMember("sjc")) {
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
		b[today.c_str()][gett->name.GetString()].AddMember(rapidjson::StringRef(now_.c_str()), exxample, a.GetAllocator());
	}

	if (_Goldname==L"") {
		std::fstream goldname("goldname.txt", std::ios_base::out);
		Value::ConstMemberIterator last = b.MemberEnd();
		last--;
		int n = last->value.MemberCount();
		_Goldname = std::to_wstring(n) + L"\n";
		for (Value::ConstMemberIterator name = last->value.MemberBegin(); name < last->value.MemberEnd(); name++) {
			goldname << name->name.GetString() << '\n';
		}
		goldname.close();
		std::wfstream fo("goldname.txt", std::ios_base::in);
		std::wstring temp;
		for (int i = 0; i < n; i++) {
			fo >> temp;
			_Goldname += temp + L'\n';
		}
		fo.close();
	}
	std::ofstream fileout(filename);
	OStreamWrapper fo(fileout);
	Writer<OStreamWrapper> write{ fo };
	b.Accept(write);
	fileout.close();
	b.GetAllocator().~MemoryPoolAllocator();
	a.GetAllocator().~MemoryPoolAllocator();
}

void RefeshData1() {
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
	std::string today = std::to_string(now.tm_mday);
	std::string filename = "Gold_" + std::to_string(now.tm_mon + 1) + "_" + std::to_string(now.tm_year + 1900) + ".json";
	std::string now_ = std::to_string(now.tm_hour) + "h" + std::to_string(now.tm_min);
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
		if (!strcmp(a["golds"][0]["value"][i]["company"].GetString(), "1Coin")) {
			continue;
		}
		std::string k = a["golds"][0]["value"][i]["brand"].GetString();
		if (!b[today.c_str()].HasMember(k.c_str())) {
			Value empty;
			empty.SetObject();
			b[today.c_str()].AddMember(rapidjson::StringRef(a["golds"][0]["value"][i]["brand"].GetString()), empty, b.GetAllocator());
		}
		std::string e = a["golds"][0]["value"][i]["type"].GetString();
		if (!b[today.c_str()][k.c_str()].HasMember(e.c_str())) {
			Value empty;
			empty.SetObject();
			b[today.c_str()][k.c_str()].AddMember(rapidjson::StringRef(a["golds"][0]["value"][i]["type"].GetString()), empty, b.GetAllocator());
		}
		Value empty;
		empty.SetObject();
		Value buy;
		buy.SetString(rapidjson::StringRef(a["golds"][0]["value"][i]["buy"].GetString()));
		Value sell;
		sell.SetString(rapidjson::StringRef(a["golds"][0]["value"][i]["sell"].GetString()));
		Value emty;
		emty.SetObject();
		b[today.c_str()][k.c_str()][e.c_str()].AddMember(rapidjson::StringRef(now_.c_str()), emty, b.GetAllocator());
		b[today.c_str()][k.c_str()][e.c_str()][now_.c_str()].AddMember(rapidjson::StringRef("buy"), buy, b.GetAllocator());
		b[today.c_str()][k.c_str()][e.c_str()][now_.c_str()].AddMember(rapidjson::StringRef("sell"), sell, b.GetAllocator());
	}
	if (_Province==L"") {
		std::fstream pro("province.txt", std::ios_base::out);
		Value::ConstMemberIterator last = b.MemberEnd();
		last--;
		int n = last->value.MemberCount();
		_Province = std::to_wstring(n) + L"\n";
		for (Value::ConstMemberIterator name = last->value.MemberBegin(); name < last->value.MemberEnd(); name++) {
			pro << name->name.GetString() << '\n';
		}
		pro.close();
		std::wfstream fo("province.txt", std::ios_base::in);
		std::wstring temp;
		for (int i = 0; i < n; i++) {
			std::getline(fo, temp);
			_Province += temp + L'\n';
		}
		fo.close();
	}
	std::ofstream fileout(filename);
	OStreamWrapper fo(fileout);
	Writer<OStreamWrapper> write{ fo };
	b.Accept(write);
	fileout.close();
	a.GetAllocator().~MemoryPoolAllocator();
	b.GetAllocator().~MemoryPoolAllocator();
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	if (hRequest) WinHttpCloseHandle(hRequest);
}

void readProvinceAndGold(std::wstring& a, std::wstring& b) {
	std::wifstream fi("GoldName.txt");
	if (fi) {
		fi.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstring temp;
		std::getline(fi, temp,L'\n');
		_Goldname += temp + L'\n';
		int n = std::stoi(temp);
		for (int i = 0; i < n; i++) {
			std::getline(fi, temp, L'\n');
			_Goldname += temp + L'\n';
		}
		fi.close();
	}
	std::wifstream fi2("Province.txt");
	if (fi2) {
		fi2.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstring temp;
		std::getline(fi2, temp, L'\n');
		_Province += temp + L'\n';
		int n = std::stoi(temp);
		for (int i = 0; i < n; i++) {
			std::getline(fi, temp, L'\n');
			_Province += temp + L'\n';
		}
		fi2.close();
	}
}

int main() {
	RefeshData1();
	RefeshData2();
	//ExtraData();
	//std::cout << extraData;
	sever();
	return 0;
}