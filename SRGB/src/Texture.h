#pragma once

#include<iostream>
//#include<cmath>
#include "Vector3.h"

class Texture {
public:
	
	Texture();

	Texture(const char* filename);
	Texture(const Texture &other);
	Texture& operator=(const Texture& other);

	~Texture();

	Vec3f getTexel(float u, float v) const;

private:

	int m_width;
	int m_height;
	int m_nchannels;
	// owner
	float *m_data;

};

