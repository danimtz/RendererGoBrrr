
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
	SDL_Init(SDL_INIT_VIDEO);

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
	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		// Later, you'll be adding your code that handles keyboard / mouse input here
	}
}