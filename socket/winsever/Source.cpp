#include<WinSock2.h>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
#include<thread>
#include<sstream>
#include<winhttp.h>
#include<fstream>

#define PORT "2000"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment	(lib, "winhttp.lib")
 
std::wstring API_KEY = L"";


struct User
{
	std::string username;
	int socket;
	std::thread process;
	bool check;
	User* next;
};

void clientConnect(User *user) {
	std::string mess = "ACCEPTED\r\n";
	std::cout << user->username << " logined\n";
	send(user->socket, mess.c_str(), mess.size(), 0);
}


User* activeUser = nullptr;
bool updateStatus = false;
void addActiveUser(std::string user, int socket) {
	activeUser = new User{ user,socket,std::thread(),true,activeUser };
	activeUser->process = std::thread(clientConnect, activeUser);
	activeUser->process.detach();
}

void deAvtiveUser(std::string user) {
	User* temp = new User;
	temp->next = activeUser;
	while (temp->next != nullptr && temp->next->username.compare(user) != 0)
	{
		temp = temp->next;
	}
	if (temp->next != nullptr) {

		User* k = temp->next;
		temp->next = temp->next->next;
		k->process.~thread();
		send(k->socket, "LOGOUT\r\n", 6, 0);
		delete k;
	}
	temp = activeUser;
	activeUser = temp->next;
	delete temp;
	return;
}

void GET(User& user,char*_str) {
	std::stringstream sstr(_str);
	std::string gold, username,date, response;
	std::string str(_str);
	sstr.seekg(str.find("/",str.find("GET")) + 1, sstr.beg);
	sstr >> gold;
	sstr.seekg(str.find(":", str.find("username")) + 1, sstr.beg);
	sstr >> username;
	sstr.seekg(str.find(":", str.find("day")) + 1, sstr.beg);
	sstr >> date;
	std::cout << gold << " " << username << " " << date;

	//check avtive accout
	//check command
	//give result

	send(user.socket, response.c_str(), response.size(), 0);
}

void LOGIN(int socket, char* _str) {
	std::stringstream sstr(_str);
	std::string username, password;
	std::string str(_str);
	sstr.seekg(str.find(":", str.find("username")) + 1, sstr.beg);
	sstr >> username;
	sstr.seekg(str.find(":", str.find("password")) + 1, sstr.beg);
	sstr >> password;
	while (updateStatus);



	if (true) {
		addActiveUser(username, socket);
	}
	else {
		send(socket, "DENIED\r\n", 6, 0);
		std::cout << "denied access from " << socket << '\n';
	}
}

void LOGOUT(User &user) {
	deAvtiveUser(user.username);
	closesocket(user.socket);
}


void REG(int socket, char* _str) {
	std::stringstream sstr(_str);
	std::string username, password, response;
	std::string str(_str);
	sstr.seekg(str.find(":", str.find("username")) + 1, sstr.beg);
	sstr >> username;
	sstr.seekg(str.find(":", str.find("password")) + 1, sstr.beg);
	sstr >> username;
}

void CHECK(int socket) {
	User* temp = activeUser;
	while (temp != nullptr && temp->socket != socket)
		temp = temp->next;
	if (!temp)
		temp->check = true;
}

void CleanClientList() {
	while (true)
	{
		Sleep(2000);
		updateStatus = true;
		User* temp = new User;
		temp->next = activeUser;
		activeUser = temp;
		while (temp->next != nullptr) {
			if (temp->next->check == false) {
				User* k = temp->next;
				temp->next = k->next;
				k->process.~thread();
				delete k;
			}
		}
		temp = activeUser;
		activeUser = temp->next;
		delete temp;
		updateStatus = false;
	}
}

void LoginAccount(int socket) {
	
	char a[1000];
	std::string str;
	int check = 0;
	do {
		check = recv(socket, a, 1000, 0);
		str += a;
	} while (check > 0);
	char* _str = new char[str.size() + 1];
	memset(_str, 0, str.size() + 1);
	str.copy(_str, str.size(), 0);
	if (strncmp(_str, "IN", 2) == 0) { LOGIN(socket, _str); }
	if (strncmp(_str, "REG", 3) == 0) { REG(socket, _str); }
	else {
		std::string k = "ERROR REQUEST\r\n";
		std::cout << "ERROR REQUEST from: " << socket;
		send(socket, k.c_str(), k.size(), 0);
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
	client = accept(listenSock, nullptr, nullptr);
	if (client == INVALID_SOCKET) {
		std::cout << "accept error: " << WSAGetLastError();
		closesocket(listenSock);
		WSACleanup();
		return 1;
	}
	do {
		LoginAccount(client);
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

void RefeshData(HINTERNET connectSV) {
	std::wstring mess = L"//api/v2/gold/doji?api_key=";
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
		RefeshData(connectSV);
		return;
	}
	std::cout << result;
}


int main() {
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL, hConnect = NULL, hRequest = NULL;
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"vapi.vnappmob.com",INTERNET_DEFAULT_HTTPS_PORT, 0);

	RefeshData(hConnect);

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}

void JsonFile() {
	
}