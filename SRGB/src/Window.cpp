
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

	//Create rasterizer THIS SHOULD BE DONE FROM RENDERER CLASS OR SOMETHING. AND NEW RASTERIZER OBJECT SHOULDNT BE CREATED AND DELETED EACH FRAME LIKE IT IS NOW
	//SAME FOR THE PIXEL BUFFER



	Buffer<uint32_t> *px_buff = new Buffer<uint32_t>(m_surface->w, m_surface->h);
	Rasterizer *rasterizer = new Rasterizer(m_surface);
	
	uint32_t colour ;

	
	
	//Test draw circle
	int count = 0;
	for (float i = 0; i<M_PI; i+=M_PI/128)
	{
		float x = cos(i);
		float y = sin(i);

		int mid_h = m_surface->h / 2;
		int mid_w = m_surface->w / 2;



		colour = SDL_MapRGB(m_surface->format, 255, 255, 255);


		rasterizer->drawLine(mid_w + (int)(50 * x), mid_h + (int)(50* y), mid_w + (int)(200 * x), mid_h + (int)(200 * y)
		, px_buff, colour);

		//count++;
	}
	for (float i = M_PI; i < 2*M_PI; i += M_PI / 128)
	{
		float x = cos(i);
		float y = sin(i);

		int mid_h = m_surface->h / 2;
		int mid_w = m_surface->w / 2;



		colour = SDL_MapRGB(m_surface->format, 255, 10, 25);


		rasterizer->drawLine(mid_w + (int)(50 * x), mid_h + (int)(50 * y), mid_w + (int)(200 * x), mid_h + (int)(200 * y)
			, px_buff, colour);

		//count++;
	}

	


	swapBuffers(px_buff);

	//px_buff->clear();
	delete px_buff;
	delete rasterizer;
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