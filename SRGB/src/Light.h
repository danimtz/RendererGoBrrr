#pragma once

#include "Vector3.h"

//GOOD CANDIDATE FOR POLYMORPHISM (ILIGHT, DIRECTIONAL LIGHT, SPOTLIGHT ETC ETC)
struct Light {
	
	Vec3f m_pos;
	Vec3f m_target;
	Vec3f m_colour;//White
	
	Light(Vec3f pos = Vec3f( 0.0f, 0.0f, 1.0f ), 
			Vec3f target = Vec3f( 0.0f, 0.0f, 0.0f ), 
			Vec3f colour = Vec3f( 255.0f, 255.0f, 255.0f )) : m_pos(pos), m_target(target), m_colour(colour){};

};