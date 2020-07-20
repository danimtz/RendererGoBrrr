#pragma once

#include <iostream>

#include "Vector3.h"
#include "Matrix4.h"
#include "Buffer.h"
#include "SDL.h"
#include "Model.h"


//Static method class. 
class Rasterizer {

public:
		

	static void drawLine(int x0, int y0, int x1, int y1, Buffer<uint32_t> *px_buff, uint32_t colour);
	static void drawWireFrame(Model *model, Buffer<uint32_t> *px_buff, uint32_t colour);



private:

	Rasterizer();

	//SDL surface pixel formats
	static const uint32_t PX_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const SDL_PixelFormat *px_format;

	//Colours
	static const uint32_t red;
	static const uint32_t green;
	static const uint32_t blue; 
	static const uint32_t white;


};

