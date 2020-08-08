#pragma once

#include <iostream>

#include "Vector3.h"
#include "Matrix4.h"
#include "Buffer.h"
#include "SDL.h"
#include "Model.h"
#include "Shader.h"


//Static method class. 
class Rasterizer {

public:
		

	static void drawLine(int x0, int y0, int x1, int y1, Buffer<uint32_t> *px_buff, uint32_t colour);

	//verts are Vec3f verts[3] and must be in viewport coordinates
	static void drawWireFrame(const Vec3f *verts, Buffer<uint32_t> *px_buff, uint32_t colour);

	
	
	//Simple version of drawTriangle()
	static void simpleRasterizeTri(const Vec3f *verts, IShader &shader, Buffer<uint32_t> *px_buff,  Buffer<float> *z_buff);

	//Optimized rasterizer function with subpixel precision 8x8 tile rejection
	static void drawTriangle(Vec3f *verts, IShader &shader, Buffer<uint32_t> *px_buff, Buffer<float> *z_buff);


private:

	Rasterizer();
	
	//Traingle rasterizing help functions

	//TODO
	static void setTriBBox(Vec2i &min, Vec2i &max, const Vec3f *verts, int vp_width, int vp_height);
	
	static float edgeFunct(const Vec3f &v0, const Vec3f &v1, const Vec3f p);

	static void drawPixel(Buffer<uint32_t> *buff, int x, int y, uint32_t colour);

	//SDL surface pixel formats
	static const uint32_t PX_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const SDL_PixelFormat *px_format;

	//Colours
	static const uint32_t red;
	static const uint32_t green;
	static const uint32_t blue; 
	static const uint32_t white;


};

