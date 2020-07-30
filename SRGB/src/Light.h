#pragma once

#include "Vector3.h"

//GOOD CANDIDATE FOR POLYMORPHISM (ILIGHT, DIRECTIONAL LIGHT, SPOTLIGHT ETC ETC)
struct Light {
	
	Vec3f m_pos{0.0f, 0.0f, 1.0f};
	Vec3f m_target{0, 0, 0};
	Vec3f m_colour{255,255,255};//White

};