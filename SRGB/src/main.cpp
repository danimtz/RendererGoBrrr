#include <iostream>
#include "Window.h"




int main(int argc, char* args[])
{
	
	WindowProps windProperties = WindowProps("WINDOOOOOOW");

	Window *main_window = new Window(windProperties);

	
	bool running = true;
	
	while (running)
	{
		//INPUT MANAGERISH CLASS HERE AND FEED RUNNING VARAIBEL ADDRESS TO IT TO CHANGE IT WHEN CLOSE WINDOW

		main_window->onUpdate();
	}
	

	//main_window->quit();

	delete main_window;
	
	return 0;

}
/*
	SDL_Window *window;
	SDL_Surface *surface;

	SDL_Init(SDL_INIT_VIDEO);

	//Create SDL window
	window = SDL_CreateWindow(
		"SGBR",						// window title
		SDL_WINDOWPOS_UNDEFINED,	// x position
		SDL_WINDOWPOS_UNDEFINED,	// y position
		1280,						// width pixels
		720,						// height pixels
		0							// flags
		);

	if (window == nullptr)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}


	surface = SDL_GetWindowSurface(window);

	if (surface == nullptr)
	{
		printf("Could not get surface: %s\n", SDL_GetError());
		return 1;
	}

	int temp;
	std::cin >> temp;

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	*/


	
