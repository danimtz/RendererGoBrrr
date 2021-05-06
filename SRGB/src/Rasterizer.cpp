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
		
		if (steep){//TEMPORARY FIX AS DIAGONAL 1 y 2 WORK BUT -1 y -2 DONT AND BREAK. FIX DIAGONAL -1 y -2
			if(!((y>=px_buff->m_width || y<0) || (x >= px_buff->m_height || x < 0))) 
				(*px_buff)(y, x) = colour;
		}
		else{
			if (!((x >= px_buff->m_width || x < 0) || (y >= px_buff->m_height || y < 0)))
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


//Draws debug arrow of given face  CHANGE THIS FUNCTION TO BE DRAW ARROW. THEN MAKE FUNCTION THAT CALLS THIS ONE CALLED DRAW NORMAL
void Rasterizer::drawNormal(const Vec3f &verts, const Vec3f *normal, Buffer<uint32_t>* px_buff, const float sf)
{
	
	
	Vec3f start, end;
	//Calculate origin()at vertex 0 for now
	start.x = (verts.x);// + verts[1].x + verts[2].x) / 3.0f;
	start.y = (verts.y);// + verts[1].y + verts[2].y) / 3.0f;
	start.z = (verts.z);// + verts[1].z + verts[2].z) / 3.0f;

	//Calculate endpoint of normal
	end.x = start.x + (sf * normal->x);
	end.y = start.y + (sf * normal->y);
	end.z = start.z + (sf * normal->z);

	drawArrow(start, end, px_buff);
}



void Rasterizer::drawArrow(const Vec3f &start, const Vec3f &end, Buffer<uint32_t>* px_buff) {

	Vec3f line[2];
	Vec3f arrow_line1[2];
	Vec3f arrow_line2[2];

	//Calculate origin()at vertex 0 for now
	line[0].x = (start.x);// + verts[1].x + verts[2].x) / 3.0f;
	line[0].y = (start.y);// + verts[1].y + verts[2].y) / 3.0f;
	line[0].z = (start.z);// + verts[1].z + verts[2].z) / 3.0f;

	//Calculate endpoint of normal
	line[1].x = (end.x);// + verts[1].x + verts[2].x) / 3.0f;
	line[1].y = (end.y);// + verts[1].y + verts[2].y) / 3.0f;
	line[1].z = (end.z);// + verts[1].z + verts[2].z) / 3.0f;

	//Set arrow lines end points
	arrow_line1[1].x = line[1].x;
	arrow_line2[1].x = line[1].x;
	arrow_line1[1].y = line[1].y;
	arrow_line2[1].y = line[1].y;
	arrow_line1[1].z = line[1].z;
	arrow_line2[1].z = line[1].z;

	
	//Lerp value
	float att = 0.85;
	Vec3f arrow_base((1 - att)*line[0].x + att * line[1].x, (1 - att) * line[0].y + att * line[1].y, (1 - att) * line[0].z + att * line[1].z);

	//Find a vector perpendicular to line at arrow_base
	Vec3f arrow_normal;
	if ((line[0].z - line[1].z) != 0) {
		arrow_normal.x = 1;
		arrow_normal.y = 1;
		arrow_normal.z = -((line[0].x - line[1].x) - (line[0].y - line[1].y)) / (line[0].z - line[1].z);
		arrow_normal.normalize();
	}
	else if ((line[0].x - line[1].x) != 0) {
		arrow_normal.z = 1;
		arrow_normal.y = 1;
		arrow_normal.x = -((line[0].y - line[1].y) - (line[0].z - line[1].z)) / (line[0].x - line[1].x);
		arrow_normal.normalize();
	}
	else {
		arrow_normal.z = 1;
		arrow_normal.x = 1;
		arrow_normal.y = -((line[0].x - line[1].x) - (line[0].z - line[1].z)) / (line[0].y - line[1].y);
		arrow_normal.normalize();
	}

	arrow_line1[0].x = arrow_base.x + 0.08 * arrow_normal.x;
	arrow_line1[0].y = arrow_base.y + 0.08 * arrow_normal.y;
	arrow_line1[0].z = arrow_base.z + 0.08 * arrow_normal.z;

	arrow_normal.negate();

	arrow_line2[0].x = arrow_base.x + 0.08 * arrow_normal.x;
	arrow_line2[0].y = arrow_base.y + 0.08 * arrow_normal.y;
	arrow_line2[0].z = arrow_base.z + 0.08 * arrow_normal.z;



	Vec3f v[2];
	Vec3f varrow1[2];
	Vec3f varrow2[2];
	Mat4f viewPrt_transform = Mat4f::createViewportTransform(px_buff->m_width, px_buff->m_height);
	for (int i = 0; i < 2; i++)
	{

		//Transorm to viewport
		v[i] = viewPrt_transform * line[i];
		varrow1[i] = viewPrt_transform * arrow_line1[i];
		varrow2[i] = viewPrt_transform * arrow_line2[i];
	}

	//Save z values for depth buffer (POST TRANSFORM) THIS IS NOT USED FOR THE NORMAL DRAWING CURRENTLY
	Vec2f z_values(v[0].z, v[1].z);


	//main line
	drawLine(v[0].x, v[0].y, v[1].x, v[1].y, px_buff, Rasterizer::red);

	//sub arrows
	drawLine(varrow1[0].x, varrow1[0].y, varrow1[1].x, varrow1[1].y, px_buff, Rasterizer::red);
	drawLine(varrow2[0].x, varrow2[0].y, varrow2[1].x, varrow2[1].y, px_buff, Rasterizer::red);
}


//Unoptimized simple version of triangle rasterization
void Rasterizer::simpleRasterizeTri( Vec3f *verts, IShader *shader, Buffer<uint32_t> *px_buff, Buffer<float> *z_buff)
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
	Vec3f invW{ 1 / verts[0].w, 1 / verts[1].w, 1 / verts[2].w };
	for (int i = 0; i < 3; i++)
	{
		verts[i].w = 1;
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
				
				Vec3f baryWeights = weights*invW;
				float oneOver_w_sum = 1/(baryWeights.x + baryWeights.y + baryWeights.z);

				Vec3f baryC_w = baryWeights * oneOver_w_sum;

				float depth = z_values.x + baryC_w.y*(z_values.y-z_values.x) + baryC_w.z*(z_values.z-z_values.x);//baryC_w.dot(z_values);



				//Depth buffer check
				if ((*z_buff)(p.x, p.y) < depth) //Near plane 1, far plane 0
				{
					(*z_buff)(p.x, p.y) = depth;

					Vec3f rgb;
					//Vec3f rgb = shader->fragment(baryC_w);

					uint32_t colour = SDL_MapRGB(px_format, rgb.x, rgb.y, rgb.z);



					//DEBUG
					//Z buffer plot: (change viewport matrix to d/2 d/2 to d=255) 
					//colour = SDL_MapRGB(px_format, depth, depth, depth);


					(*px_buff)(p.x, p.y) = colour;//THIS SHOULD BE A FUNCTION IN RASTERIZER CALLED drawPixel()
				
				}
			}
		}
	}
}


void Rasterizer::drawTriangle(const std::vector<VShaderOut>& vrtx_buff, const std::vector<int>& idx_buff, IShader* shader, Buffer<uint32_t>* px_buff, Buffer<float>* z_buff)
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

//#pragma omp parallel for schedule(dynamic) 
	for(int i = 0; i<idx_buff.size(); i+=3)
	{ 
		
		//If trtaingled clipped skip
		if(idx_buff[i] == -1){ continue;};
		
		//Extract vertex positions
		Vec3f v[3];
		for (int k = 0; k < 3; k++) {
			v[k] = vrtx_buff[idx_buff[i+k]].position;
		}
	

		//Winding order backface culling
		float invArea;
		float signedArea = edgeFunct(v[0], v[1], v[2]);
		if (signedArea <= 0) {
		continue;};
		invArea = 1 / signedArea;


		//Rasterizer variables
		float depth;
		Vec3f rgb, persp_bary, baryW;
		uint32_t colour;
		
		float z_w1, z_w2; //barycentric weights for Z buffer
		Vec3f z_values(v[0].z, v[1].z, v[2].z);

		float w0, w1, w2;
		Vec3f invW{ 1 /v[0].w, 1 / v[1].w, 1 / v[2].w };

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
						baryW = Vec3f(w0 * invW.x, w1 * invW.y, w2 * invW.z);

						//Calculate barycentric coordinates from perpectiv div weights
						float persp_area = 1/(baryW.x + baryW.y + baryW.z);
						persp_bary = baryW * persp_area;

						
						rgb = shader->fragment(persp_bary, vrtx_buff[idx_buff[i]], vrtx_buff[idx_buff[i+1]], vrtx_buff[idx_buff[i+2]]);
						

						colour = SDL_MapRGB(px_format, rgb.x, rgb.y, rgb.z);
						
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
	return -((v1.x - v0.x) * (p.y - v0.y) - (v1.y - v0.y) * (p.x - v0.x));//REMOVED THE NEGASTIVE FOR TESTS
}

void Rasterizer::drawPixel(Buffer<uint32_t> *buff, int x, int y, uint32_t colour)
{
	(*buff)(x, y) = colour;
}
