#pragma once

#include "SDL.h"

//Window properties
struct WindowProps {
	std::string Name;
	uint32_t Width;
	uint32_t Height;

	WindowProps(std::string window_name = "SRGB",
				uint32_t width_px = 1280, 
				uint32_t height_px = 720)
				: Name(window_name),Width(width_px),Height(height_px) {};
};

// Creates and destroys nd writes buffer to SDL window surface
class Window {
	public:

		
		// Constructor/Destructor
		Window();
		Window(WindowProps &props);
		~Window();

		
		
		void onUpdate();
		//TODO memcpy buffer to surface at some point swapbuffer etc


	private:

		SDL_Window *m_window;
		SDL_Surface *m_surface;
		
		// Starts and end funcions of window
		void init(WindowProps &window_props);
		void quit();


};

