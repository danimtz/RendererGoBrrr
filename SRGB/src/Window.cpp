
#include<iostream>
#include "Window.h"




//Default constructor
Window::Window() : m_sdl_window(nullptr),m_sdl_surface(nullptr), m_gcontext(nullptr) 
{

	WindowProps props = WindowProps();

	init(props);

	

};

//Custom props constructor
Window::Window(WindowProps &props) : m_sdl_window(nullptr), m_sdl_surface(nullptr), m_gcontext(nullptr)
{

	init(props);

};


Window::~Window()
{
	delete m_gcontext;
};


void Window::init(WindowProps &props)
{

	
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Could not init video: %s\n", SDL_GetError());
	}

	//Create SDL window HARD CODED VALUES ATM BUT COULD BE CHANGED
	m_sdl_window = SDL_CreateWindow(
		props.Name.c_str(),			// window title
		SDL_WINDOWPOS_UNDEFINED,	// x position
		SDL_WINDOWPOS_UNDEFINED,	// y position
		props.Width,				// width pixels
		props.Height,				// height pixels
		0							// flags
	);

	//Check window was created succesfully
	if (m_sdl_window == nullptr)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		
	}

	//Get surface window and check that it was gotten succesfully
	m_sdl_surface = SDL_GetWindowSurface(m_sdl_window);
	if (m_sdl_surface == nullptr)
	{
		printf("Could not get surface: %s\n", SDL_GetError());
		
	}

	//Create renderer instance
	m_gcontext = new Renderer(props.Width, props.Height); //deleted in destructor


}


void Window::quit()
{
	//Destroy window
	SDL_DestroyWindow(m_sdl_window);
	m_sdl_window = nullptr;
	m_sdl_surface = nullptr;

	// Clean up
	SDL_Quit();
}



void Window::onUpdate()
{
	
	Buffer<uint32_t> *render_target = m_gcontext->getRenderTarget();
	
	swapBuffers(render_target);

}

void Window::swapBuffers(Buffer<uint32_t> *px_buff)
{
	

	
	//Lock SDL surface
	SDL_LockSurface(m_sdl_surface);


	//Copy buffer
	memcpy(m_sdl_surface->pixels, px_buff->buffer, px_buff->m_buff_sz);
	

	//Unlock SDL surface
	SDL_UnlockSurface(m_sdl_surface);

	//Update window surface
	SDL_UpdateWindowSurface(m_sdl_window);

}



Renderer* Window::getGraphicContext()
{
	return m_gcontext;
}








//POKEBALL
/*
//Test draw circle
int count = 0;
for (float i = 0; i < M_PI; i += M_PI / 128)
{
	float x = cos(i);
	float y = sin(i);

	int mid_h = m_sdl_surface->h / 2;
	int mid_w = m_sdl_surface->w / 2;



	colour = SDL_MapRGB(m_sdl_surface->format, 255, 255, 255);


	Rasterizer::drawLine(mid_w + (int)(50 * x), mid_h + (int)(50 * y), mid_w + (int)(200 * x), mid_h + (int)(200 * y)
		, px_buff, colour);

	//count++;
}
for (float i = M_PI; i < 2 * M_PI; i += M_PI / 128)
{
	float x = cos(i);
	float y = sin(i);

	int mid_h = m_sdl_surface->h / 2;
	int mid_w = m_sdl_surface->w / 2;



	colour = SDL_MapRGB(m_sdl_surface->format, 255, 10, 25);


	Rasterizer::drawLine(mid_w + (int)(50 * x), mid_h + (int)(50 * y), mid_w + (int)(200 * x), mid_h + (int)(200 * y)
		, px_buff, colour);

	//count++;
}
*/