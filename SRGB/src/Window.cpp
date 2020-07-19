
#include<iostream>
#include "Window.h"




//Default constructor
Window::Window() : m_window(nullptr),m_surface(nullptr), m_renderer(nullptr) 
{

	WindowProps props = WindowProps();

	init(props);

	

};

//Custom props constructor
Window::Window(WindowProps &props) : m_window(nullptr), m_surface(nullptr), m_renderer(nullptr)
{

	init(props);

};


Window::~Window()
{
	delete m_renderer;
};


void Window::init(WindowProps &props)
{

	
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Could not init video: %s\n", SDL_GetError());
	}

	//Create SDL window HARD CODED VALUES ATM BUT COULD BE CHANGED
	m_window = SDL_CreateWindow(
		props.Name.c_str(),			// window title
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

	//Create renderer instance
	m_renderer = new Renderer(props.Width, props.Height); //deleted in destructor


}


void Window::quit()
{
	//Destroy window
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	m_surface = nullptr;

	// Clean up
	SDL_Quit();
}



void Window::onUpdate()
{
	
	
	



	
	Buffer<uint32_t> *render_target = m_renderer->getRenderTarget();
	



#if 1 //DEBUG STUFF THAT WHOULDNT ACTUALLY GO HERE

	//THE BUNNY IS BEING LOADED EACH FRAME BIG FUCKING NOPE. THIS NEEDS TO BE IN A RENDDERER CLASS OR SOMETHING WITH A RENDER QUEUE
	Model *teapot = new Model("assets\\head.obj");// CHECK THIS
	uint32_t colour = SDL_MapRGB(m_surface->format, 255, 255, 255);
	Rasterizer::drawWireFrame(teapot, render_target, colour);//CAUSES HEAP ALLOC ERROR. CHECK OBJ PARSER WORKS


	//Rasterizer::drawLine(1279,719,0,0, render_target, colour);//WORK ON EDGE CASES OF PIXEL BUFFER


#endif


	swapBuffers(render_target);

	

	delete teapot;


	
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












//POKEBALL
/*
//Test draw circle
int count = 0;
for (float i = 0; i < M_PI; i += M_PI / 128)
{
	float x = cos(i);
	float y = sin(i);

	int mid_h = m_surface->h / 2;
	int mid_w = m_surface->w / 2;



	colour = SDL_MapRGB(m_surface->format, 255, 255, 255);


	Rasterizer::drawLine(mid_w + (int)(50 * x), mid_h + (int)(50 * y), mid_w + (int)(200 * x), mid_h + (int)(200 * y)
		, px_buff, colour);

	//count++;
}
for (float i = M_PI; i < 2 * M_PI; i += M_PI / 128)
{
	float x = cos(i);
	float y = sin(i);

	int mid_h = m_surface->h / 2;
	int mid_w = m_surface->w / 2;



	colour = SDL_MapRGB(m_surface->format, 255, 10, 25);


	Rasterizer::drawLine(mid_w + (int)(50 * x), mid_h + (int)(50 * y), mid_w + (int)(200 * x), mid_h + (int)(200 * y)
		, px_buff, colour);

	//count++;
}
*/