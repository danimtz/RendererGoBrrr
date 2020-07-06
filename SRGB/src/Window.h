#pragma once

#include "SDL.h"


// Creates and destroys nd writes buffer to SDL window surface
class Window {
	public:

		// Constructor/Destructor
		Window();
		~Window();

		// Starts and end funcions of window
		bool init();
		void quit();
		

		//TODO memcpy buffer to surface at some point swapbuffer etc


	private:

		SDL_Window *m_window;
		SDL_Surface *m_surface;



};

