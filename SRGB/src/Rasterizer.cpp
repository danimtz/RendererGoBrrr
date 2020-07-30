#include "Rasterizer.h"
#include<array>


//Constants
const SDL_PixelFormat* Rasterizer::px_format = SDL_AllocFormat(PX_FORMAT);

const uint32_t Rasterizer::red = SDL_MapRGB(px_format, 255, 0, 0);
const uint32_t Rasterizer::green = SDL_MapRGB(px_format, 0, 255, 0);
const uint32_t Rasterizer::blue = SDL_MapRGB(px_format, 0, 0, 255);
const uint32_t Rasterizer::white = SDL_MapRGB(px_format, 255, 255, 255);



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


//Currently sloppy and only works for specific obj files MAYBE THIS SHOULDNT TAKE THE MODEL AS ARGUMENTS AND ONLY THE THREE VERTICES/FACE
void Rasterizer::drawWireFrame(const Vec3f *verts, Buffer<uint32_t> *px_buff, uint32_t colour)
{
	drawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, px_buff, colour);//Conversion from const T to int possible loos of data wraning. MAYBE CAST IT?
	drawLine(verts[0].x, verts[0].y, verts[2].x, verts[2].y, px_buff, colour);
	drawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, px_buff, colour);

}




//Unoptimized simple version of triangle rasterization
void Rasterizer::simpleRasterizeTri(const Vec3f *verts, Buffer<uint32_t> *px_buff, Buffer<float> *z_buff, float intensity)
{
	//				v2
	//				/\
	//			   /  \
	//			  /	   \
	//			 /		\
	//			/	     \
	//		   /__________\
	//		 v0			   v1
	//

	

	

	//transform to viewport coords 
	Vec3f vptri[3];
	Mat4f viewPrt_transform = Mat4f::createViewportTransform(px_buff->m_width, px_buff->m_height);
	for (int i = 0; i < 3; i++)
	{
		vptri[i] = viewPrt_transform * verts[i];
	}


	//Save z values for depth buffer (POST TRANSFORM)
	Vec3f z_values(vptri[0].z, vptri[1].z, vptri[2].z);


	//Find triangle bounding box
	Vec2i min,max;
	setTriBBox(min,max,vptri, px_buff->m_width, px_buff->m_height);

	Vec3f p;//point p
	

	// colour for triangle
	uint32_t colour = SDL_MapRGB(px_format, intensity * 255, intensity * 255, intensity * 255);
	
	

	for (p.y = min.y; p.y <= max.y; p.y++)
	{
		for (p.x = min.x; p.x <= max.x; p.x++)
		{

			//Calculate unnormalized barycentric coords
			Vec3f weights;
			weights.x = edgeFunct(vptri[1], vptri[2], p);
			weights.y = edgeFunct(vptri[2], vptri[0], p);
			weights.z = edgeFunct(vptri[0], vptri[1], p);

			

			//If inside triangle draw pixel
			if (weights.x >= 0 && weights.y >= 0 && weights.z >= 0) //(w0|w1|w2>0)bit flag
			{
				
				//Normalize barycentric coordinates: w weights / sum of weights (which are the area of the parallelogram)
				//THIS IS SLOW?? could just calculate area of trinagle at the start since thats what sum of weights are
				
				float oneOver_w_sum = 1/(weights.x + weights.y + weights.z);

				Vec3f baryC_w = weights* oneOver_w_sum;

				float depth = baryC_w.dot(z_values);




				//Depth buffer check
				if ((*z_buff)(p.x, p.y) < depth) //Near plane 1, far plane 0
				{
					(*z_buff)(p.x, p.y) = depth;

					//Z buffer plot: (change viewport matrix to d/2 d/2 to d=255) 
					//colour = SDL_MapRGB(px_format, depth, depth, depth);


					(*px_buff)(p.x, p.y) = colour;//THIS SHOULD BE A FUNCTION IN RASTERIZER CALLED drawPixel()
				
				}



			}
		}
	}



}








void Rasterizer::setTriBBox(Vec2i &min, Vec2i &max, const Vec3f *verts, int vp_width, int vp_height)//Expects verts to be in viewport coordinates
{
	//Initialize min and max to one of the vertices
	min = verts[0];
	max = verts[0];

	//Check the other two vertices for min and max values
	for (int i = 1; i < 3; i++)
	{
		//Update min
		if (verts[i].x < min.x)
			min.x = verts[i].x;
		if (verts[i].y < min.y)
			min.y = verts[i].y;
		//Update max
		if (verts[i].x > max.x)
			max.x = verts[i].x;
		if (verts[i].y > max.y)
			max.y = verts[i].y;
	}


	//Clip viewport bounds
	
	min.x = std::max(min.x,0);
	min.y = std::max(min.y,0);
	max.x = std::min(max.x, vp_width - 1);
	max.y = std::min(max.y, vp_height - 1);
	
}


float Rasterizer::edgeFunct(const Vec3f &v0, const Vec3f &v1, const Vec3f p)
{
	//Returns edge function whch is given by the determinant of: 
	//		| (v1.x - v0.x)  (p.x - v0.x) |
	//		| (v1.y - v0.y)  (p.y - v0.y) |
	//output weight is non-normalized barycentric coordinates. a
	//a.k.a. 2* area of triangle between the three points v0,v1,p
	
	//Negative so that inside the triangle is posive i.e. given counterclockwise triangle vertices,
	//the positive half-space is on the left
	return -((v1.x - v0.x)*(p.y - v0.y) - (v1.y - v0.y)*(p.x - v0.x));
}


