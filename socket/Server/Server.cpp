#include "Server.h"

#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Server::Application::EnableVisualStyles();
	Server::Application::SetCompatibleTextRenderingDefault(false);
	Server::Application::Run(gcnew Server::Server());

	return 0;
}