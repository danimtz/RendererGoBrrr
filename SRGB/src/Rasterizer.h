#pragma once

#include <iostream>

#include "Vector3.h"
#include "Buffer.h"
#include "SDL.h"

//Static method class. 
class Rasterizer {

	public:
		//Constructor that uses SDL surface as input to know what type of pixel format it is using

		Rasterizer(SDL_Surface *surface) : m_px_format(surface->format),  
											red(SDL_MapRGB(m_px_format,255,0,0)), 
											green(SDL_MapRGB(m_px_format, 255, 0, 0)), 
											blue(SDL_MapRGB(m_px_format, 255, 0, 0)){}

		void drawLine(int x0, int y0, int x1, int y1, Buffer<uint32_t> *px_buff, uint32_t colour);
		void drawWireFrame(/*Model model,*/ Buffer<uint32_t> *px_buff, uint32_t colour);

	private:
		SDL_PixelFormat *m_px_format;
		uint32_t red;
		uint32_t green;
		uint32_t blue;


};

