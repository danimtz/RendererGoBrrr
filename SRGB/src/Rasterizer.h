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

	//verts are Vec3f verts[3] and must be in viewport coordinates
	static void drawWireFrame(const Vec3f *verts, Buffer<uint32_t> *px_buff, uint32_t colour);

	
	static void drawTriangle(const Vec3f *verts, Buffer<uint32_t> *px_buff, uint32_t colour);//colour should later be like shader or something and maybe needs zbuffer

private:

	Rasterizer();
	
	//Traingle rasterizing help functions

	//TODO
	static void setTriBBox(Vec2f *min, Vec2f *max, const Vec3f *verts); 
	//TODO
	static Vec3f getBarycCoord(const Vec3f *verts);


	//SDL surface pixel formats
	static const uint32_t PX_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const SDL_PixelFormat *px_format;

	//Colours
	static const uint32_t red;
	static const uint32_t green;
	static const uint32_t blue; 
	static const uint32_t white;


};

