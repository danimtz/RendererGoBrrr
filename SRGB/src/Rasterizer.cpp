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
void Rasterizer::simpleRasterizeTri(const Vec3f *verts, IShader &shader, Buffer<uint32_t> *px_buff, Buffer<float> *z_buff)
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

				Vec3f baryC_w = weights * oneOver_w_sum;

				float depth = z_values.x + baryC_w.y*(z_values.y-z_values.x) + baryC_w.z*(z_values.z-z_values.x);//baryC_w.dot(z_values);



				//Depth buffer check
				if ((*z_buff)(p.x, p.y) < depth) //Near plane 1, far plane 0
				{
					(*z_buff)(p.x, p.y) = depth;

					
					Vec3f rgb = shader.fragment(baryC_w);

					uint32_t colour = SDL_MapRGB(px_format, rgb.r, rgb.g, rgb.b);



					//DEBUG
					//Z buffer plot: (change viewport matrix to d/2 d/2 to d=255) 
					//colour = SDL_MapRGB(px_format, depth, depth, depth);


					(*px_buff)(p.x, p.y) = colour;//THIS SHOULD BE A FUNCTION IN RASTERIZER CALLED drawPixel()
				
				}
			}
		}
	}
}


void Rasterizer::drawTriangle(Vec3f *verts, IShader &shader, Buffer<uint32_t> *px_buff, Buffer<float> *z_buff)
{
	//				v2
	//				/\
	//			   /  \
	//		    w1/	   \w0
	//			 /		\
	//			/	     \
	//		   /__________\
	//		 v0	    w2	   v1
	//
	// More info: https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/
	//			 https://web.archive.org/web/20120625103536/http://devmaster.net/forums/topic/1145-advanced-rasterization/
	//
	//transform to viewport coords 



	//Triangle variables
	float invArea;
	Vec3f v[3];
	Vec3f invW{ 1 / verts[0].w, 1 / verts[1].w, 1 / verts[2].w };
	

	//Fragment variables
	float depth;
	Vec3f rgb, persp_bary, baryW;
	uint32_t colour;
	float w0, w1, w2;
	float z_w1, z_w2; //barycentric weights for Z buffer


	Mat4f viewPrt_transform = Mat4f::createViewportTransform(px_buff->m_width, px_buff->m_height);
	for (int i = 0; i < 3; i++)
	{
		//Reset hidden w
		verts[i].w = 1;
		//Transorm to viewport
		v[i] = viewPrt_transform * verts[i];//MAYBE HERE
	}

	//Save z values for depth buffer (POST TRANSFORM)
	Vec3f z_values(v[0].z, v[1].z, v[2].z);


	float tempinvArea = edgeFunct(v[0], v[1], v[2]);
	if (tempinvArea <= 0) return; //Winding order backface culling
	invArea = 1 / tempinvArea;


	//Edge function deltas (x needs to be negative or subtracted in loop)
	float Dx01 = -(v[0].x - v[1].x); 
	float Dx12 = -(v[1].x - v[2].x); 
	float Dx20 = -(v[2].x - v[0].x); 

	float Dy01 = -(v[0].y - v[1].y);
	float Dy12 = -(v[1].y - v[2].y); 
	float Dy20 = -(v[2].y - v[0].y); 

	//Find triangle bounding box
	Vec2i min, max;
	setTriBBox(min, max, v, px_buff->m_width, px_buff->m_height);


	Vec3f p = Vec3f(min.x, min.y, 0);//point p

	//Edge function at start of row (edge function constant+first delta)
	float w0_row = edgeFunct(v[1], v[2], p);
	float w1_row = edgeFunct(v[2], v[0], p);
	float w2_row = edgeFunct(v[0], v[1], p);



	// Fill convention(top left rule) bias (If edge going down) FIX WHEN FIXED POINTS COORDINATES ARE IMPLEMENTED and also CREATES SOME BLACK DOTS
	//if (Dy12 < 0 || (Dy12 == 0 && Dy12 > 0)) w0_row++;

	//if (Dy20 < 0 || (Dy20 == 0 && Dy20 > 0)) w1_row++;

	//if (Dy01 < 0 || (Dy01 == 0 && Dy01 > 0)) w2_row++;



	for (p.y = min.y; p.y <= max.y; p.y++)
	{
		//Update weights
		w0 = w0_row;
		w1 = w1_row;
		w2 = w2_row;
			
		for (p.x = min.x; p.x <= max.x; p.x++)
		{

			//If inside triangle render pixel
			if ((w0 >= 0) && (w1 >= 0) && (w2 >= 0))
			{
				
				//Calculate barycentric weights for Z-Buff and interpolate depth  THIS WEIGHTS MAY BE SLIGHTLY OVER 1 DUE TO PRECISION ERRORS CURRENTLY may be fied with fixed point precision
				z_w1 = w1 * invArea;
				z_w2 = w2 * invArea;
				depth = z_values.x + z_w1 * (z_values.y - z_values.x) + z_w2 * (z_values.z - z_values.x);

				//Depth buffer check
				if ((*z_buff)(p.x, p.y) < depth) //Near plane 1, far plane 0
				{
					//Update z-buffer
					(*z_buff)(p.x, p.y) = depth;//drawZbuff function


					//Perspective divide barycentric weights
					//baryW = Vec3f(w0, w1, w2);
					baryW = Vec3f(w0 * invW.x, w1 * invW.y, w2 * invW.z);

					//Calculate barycentric coordinates from perpectiv div weights
					float persp_area = 1/(baryW.x + baryW.y + baryW.z);
					persp_bary = baryW * persp_area;

					rgb = shader.fragment(persp_bary);


					colour = SDL_MapRGB(px_format, rgb.r, rgb.g, rgb.b);

					drawPixel(px_buff, p.x, p.y, colour);
				}
			}

			//Next col step
			w0 += Dy12;
			w1 += Dy20;
			w2 += Dy01;
		}

		//Next row step
		w0_row -= Dx12;
		w1_row -= Dx20;
		w2_row -= Dx01;
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
	//		| (v1.y - v0.y)  (p.y - v0.y) | = (v1.x - v0.x)*(p.y - v0.y) - (v1.y - v0.y)*(p.x - v0.x) alternative (v0.y - v1.y)*p.x + (v1.x - v0.x)*p.y + (v0.x*v1.y - v0.y*v1.x)
	//output weight is non-normalized barycentric coordinates. a
	//a.k.a. 2* area of triangle between the three points v0,v1,p
	//More info: "A Parallel Algorithm for Polygon Rasterization",Juan Pineda 1988
	
	//Negative so that inside the triangle is positve i.e. given counterclockwise triangle vertices,
	//the positive half-space is on the left
	return -((v1.x - v0.x) * (p.y - v0.y) - (v1.y - v0.y) * (p.x - v0.x));
}


void Rasterizer::drawPixel(Buffer<uint32_t> *buff, int x, int y, uint32_t colour)
{
	(*buff)(x, y) = colour;
}
