#pragma once

#include "Vector3.h"

//GOOD CANDIDATE FOR POLYMORPHISM (ILIGHT, DIRECTIONAL LIGHT, SPOTLIGHT ETC ETC)
class Light {
public:
	Vec3f m_pos;
	Vec3f m_colour;//White
	
	Light(Vec3f pos = Vec3f( 0.0f, 0.0f, 1.0f ), 
			Vec3f colour = Vec3f( 1.0f, 1.0f, 1.0f )) : m_pos(pos), m_colour(colour){};

};


class DirLight : public Light {
public:

	Vec3f m_target;
	Vec3f m_direction;

	DirLight(Vec3f pos = Vec3f(0.0f, 0.0f, 1.0f), 
		Vec3f colour = Vec3f(1.0f, 1.0f, 1.0f), 
		Vec3f target = Vec3f(0.0f, 0.0f, 0.0f)) : Light(pos, colour), m_target(target), m_direction(m_pos - m_target) {};

};



class PointLight : public Light {
public:

	float m_constant;
	float m_linear;
	float m_quadratic;

	PointLight(Vec3f pos = Vec3f(0.0f, 0.0f, 1.0f),
		Vec3f colour = Vec3f(1.0f, 1.0f, 1.0f), 
		 float linear = 0.09f, float quadratic = 0.032f, float constant = 1.0f) : Light(pos, colour), m_constant(constant), m_linear(linear), m_quadratic(quadratic) {};

};
