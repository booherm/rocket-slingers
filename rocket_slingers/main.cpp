#include <iostream>
#include <windows.h>
#include "GuiConsole.hpp"
#include "RocketSlingersController.hpp"

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	startConsole();
	try {
		std::cout << "Starting Rocket Slingers" << std::endl;

		RocketSlingersController rsc;
		rsc.start();

		std::cout << "Exiting Rocket Slingers" << std::endl;
	}
	catch (std::string e) {
		std::cout << "Exception: " << e << std::endl;
	}

	system("pause");
	return 0;
}
