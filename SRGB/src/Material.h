#pragma once
#include "Vector3.h"


//NOTE to future me: Material is currently being used as an input to the shader owned by the renderer. 
// In reality the material should own a shader to which the different material values are uploaded to as uniforms 
// then basically when you render an object with a material you are telling it which shader to use. 
//(Some uniformsa should still be set by renderer like light dir and transform matrices)
struct Material {

	Vec3f m_Il; //Light/Diffuse/Specular intensity aka colour
	
	Vec3f m_Ia; //Ambient intensity

	float m_ka, m_kd, m_ks; // Ambient, diffuse, specular coefficients

	float m_spec_n; //Shinyness

	Material(const Vec3f colour = Vec3f(199, 134, 36), const float shinyness = 64.0f, const float ks = 0.4, const Vec3f ambient = Vec3f(50,50,50), const float ka = 0.1,
		const float kd = 0.8 ) : m_Il(colour), m_Ia{ambient}, m_ka(ka), m_kd(kd), m_ks(ks), m_spec_n(shinyness){}

};