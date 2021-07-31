#include "MyForm.h"

#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Client::Application::EnableVisualStyles();
	Client::Application::SetCompatibleTextRenderingDefault(false);
	Client::Application::Run(gcnew Client::MyForm());
 
	return 0;
}