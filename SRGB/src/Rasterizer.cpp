#include "Rasterizer.h"




//Integer Bresenham line drawer
void Rasterizer::drawLine(int x0, int y0, int x1, int y1, Buffer<uint32_t> *px_buff, uint32_t colour){	



	bool steep = false;
	if ( ((x1 - x0) * (x1 - x0)) < ((y1 - y0) * (y1 - y0)) ){
		steep = true;
		std::swap(x0,y0);
		std::swap(x1,y1);
	}

	if (x0 > x1){
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int y = y0;
	int dx = x1 - x0;
	int dy = y1 - y0;
	
	int err = 2 * dy - dx;
	
	//If y0 > y1 ie line goes downward
	int yi = 1;
	if (dy < 0){ yi = -1; dy = -dy; }


	for (int x=x0; x <= x1; x++)
	{
		if (steep){
			(*px_buff)(y, x) = colour;
		}
		else{
			(*px_buff)(x, y) = colour;
		}

		//If derr is larger than 0 increment y and update derr
		if (err >= 0){
			y += yi;
			err -= 2*dx;
		}

		//Update derr
		err += 2*dy;
	}
}

