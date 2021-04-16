#pragma once
#include "Vector3.h"

struct Material {

	Vec3f m_Il; //Light/Diffuse/Specular intensity aka colour
	
	Vec3f m_Ia; //Ambient intensity

	float m_ka, m_kd, m_ks; // Ambient, diffuse, specular coefficients

	float m_spec_n; //Shinyness

	Material(const Vec3f colour = Vec3f(199, 134, 36), const float shinyness = 32, const float ks = 0.4, const Vec3f ambient = Vec3f(50,50,50), const float ka = 0.1,
		const float kd = 0.8 ) : m_Il(colour), m_Ia{ambient}, m_ka(ka), m_kd(kd), m_ks(ks), m_spec_n(shinyness){}

};