#include "GuiConsole.hpp"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	startConsole();

	std::cout << "Windows 10" << std::endl;

	Sleep(15000);

	return 0;
}
