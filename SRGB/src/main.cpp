#include <iostream>

#include "Window.h"

#include "Rasterizer.h"

#include "Vector3.h"

#include "Model.h"



int main(int argc, char* args[])
{
	


	//Model *teapot = new Model("assets\\bunny.obj");


	WindowProps windProperties = WindowProps("WINDOOOOOOW",1280,720);

	Window *main_window = new Window(windProperties);



	


	//THIS MAIN LOOP SHOULD BE MOVED AWAY FROM MAIN INTO engine CLASS OR SOMETHING

	bool running = true;
	//main loop

	SDL_Event event;//DEBUG
	while (running)
	{

		//TODO:
		
		//INPUT MANAGERISH CLASS HERE AND FEED RUNNING VARAIBEL ADDRESS TO IT TO CHANGE IT WHEN CLOSE WINDOW


		//DEBUG
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:

			running = false;
			break;
		}
		//DEBUG

		main_window->onUpdate();

	}
	
	

	delete main_window;

	return 0;

}


	
