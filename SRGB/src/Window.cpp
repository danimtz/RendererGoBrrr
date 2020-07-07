
#include<iostream>
#include "Window.h"




//Default constructor
Window::Window() :m_window(nullptr),m_surface(nullptr) {

	WindowProps props = WindowProps();

	init(props);

	

};

//Custom props constructor
Window::Window(WindowProps &props) :m_window(nullptr), m_surface(nullptr)
{

	init(props);

};


Window::~Window(){};


void Window::init(WindowProps &props)
{

	
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Could not init video: %s\n", SDL_GetError());
	}

	//Create SDL window HARD CODED VALUES ATM BUT COULD BE CHANGED
	m_window = SDL_CreateWindow(
		props.Name.c_str(),						// window title
		SDL_WINDOWPOS_UNDEFINED,	// x position
		SDL_WINDOWPOS_UNDEFINED,	// y position
		props.Width,				// width pixels
		props.Height,				// height pixels
		0							// flags
	);

	//Check window was created succesfully
	if (m_window == nullptr)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		
	}

	//Get surface window and check that it was gotten succesfully
	m_surface = SDL_GetWindowSurface(m_window);
	if (m_surface == nullptr)
	{
		printf("Could not get surface: %s\n", SDL_GetError());
		
	}

}


void Window::quit()
{
	//Destroy window
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	// Clean up
	SDL_Quit();
}



void Window::onUpdate()
{
	
	
	



	//TEMP STUFF. THIS SHOULD BE CALLED FROM RENDERER OR WHEREVER LIKE: 
	//renderer.getRenderTarget ie buffer
	//swapBuffers(renderer.getRenderTarget)


	Buffer<uint32_t> *px_buff = new Buffer<uint32_t>(640, 480);  


	//Pixel colour and format
	SDL_PixelFormat *px_format = m_surface->format;//SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
	uint32_t colour = SDL_MapRGB(px_format, 150, 14, 255);



	px_buff->clear();

	for (int j = 50; j < 200; j++)
	{
		for (int i = 50; i < 200; i++)
		{
			(*px_buff)(i, j) = colour;
		}
	}

	/*
	uint8_t r, g, b;
	SDL_GetRGB(colour, px_format, &r, &g, &b);
	std::cout << (int)r << " " << (int)g << " " << (int)b << std::endl;
	*/

	swapBuffers(px_buff);

	delete px_buff;
}

void Window::swapBuffers(Buffer<uint32_t> *px_buff)
{
	

	
	//Lock SDL surface
	SDL_LockSurface(m_surface);


	//Copy buffer
	memcpy(m_surface->pixels, px_buff->buffer, px_buff->m_buff_sz);
	

	//Unlock SDL surface
	SDL_UnlockSurface(m_surface);

	//Update window surface
	SDL_UpdateWindowSurface(m_window);

}










/* THIS WILL GO IN SDLEVENT MANAGER CLASs/INPUTCLASS/ OR WHATEVER
SDL_Event Event;

while (SDL_PollEvent(&Event))
{
	// Later, you'll be adding your code that handles keyboard / mouse input here
}*/