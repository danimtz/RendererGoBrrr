
#include<iostream>
#include "Window.h"


//Constructor and destructor
Window::Window() : m_window(nullptr), m_surface(nullptr) {};
Window::~Window(){};


bool Window::init()
{

	
	//Start SDL
	SDL_Init(SDL_INIT_VIDEO);

	//Create SDL window HARD CODED VALUES ATM BUT COULD BE CHANGED
	m_window = SDL_CreateWindow(
		"SGBR",						// window title
		SDL_WINDOWPOS_UNDEFINED,	// x position
		SDL_WINDOWPOS_UNDEFINED,	// y position
		1280,						// width pixels
		720,						// height pixels
		0							// flags
	);

	//Check window was created succesfully
	if (m_window == nullptr)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	//Get surface window and check that it was gotten succesfully
	m_surface = SDL_GetWindowSurface(m_window);
	if (m_surface == nullptr)
	{
		printf("Could not get surface: %s\n", SDL_GetError());
		return false;
	}


	return true;
}


void Window::quit()
{
	//Destroy window
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	// Clean up
	SDL_Quit();
}

