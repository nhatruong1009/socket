#include "Ip.h"
#include "Gold.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Client::Application::EnableVisualStyles();
	Client::Application::SetCompatibleTextRenderingDefault(false);
	Client::Application::Run(gcnew Client::Ip());

	return 0;
}