#include <windows.h>
#include <iostream>

#include "winapiwrapper.h"
#include "openglwrapper.h"
#include "gameController.h"

using namespace GController;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
               LPSTR lpCmdLine, int nCmdShow)
{
	gameController *snake = new gameController(hInstance);
	//gameControllerSetObj(snake);	//for callback methods

	std::cout<<"Welcome to Snake game!"<<std::endl;
	std::cout<<"\n#####Controls#########"<<std::endl;
	std::cout<<"Arrow keys, Home and End buttons to move"<<std::endl;
	std::cout<<"C to change view"<<std::endl;
	std::cout<<"Esc to end game\n"<<std::endl;

	try
	{
		if(snake->init())
			snake->go();
		else
		{
			std::cout<<"\nError during game initialization."<<std::endl;
			return 1;
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout<<"\nQuiting...";

	return 0;

}
