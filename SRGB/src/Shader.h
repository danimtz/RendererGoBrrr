#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include "Model.h"
#include "Light.h"
#include <algorithm>

class IShader {
public:
	
	virtual ~IShader() {};
	virtual Vec3f vertex(const Model &model, int face_idx, int nth_vert) = 0;
	virtual Vec3f fragment(const Vec3f &bary) = 0;

};

class FlatShader : public IShader {
public:

	//Per Model
	Mat4f MVPmat, MVmat, Nmat; // Matrices  
	Texture *texture;
	std::vector<Vec3f> light_dir;


	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	//Written by vertex shader
	Vec3f varying_intensity;
	Vec2f uv_values[3];


	FlatShader(const Mat4f MV, const Mat4f MVP, const Mat4f N, const std::vector<Light*> &lights) : MVPmat(MVP), MVmat(MV), Nmat(N)
	{
		light_dir.reserve(lights.size());
		for (int i = 0; i < lights.size(); i++)
		{
			light_dir.push_back(lights[i]->m_pos - lights[i]->m_target);
			light_dir[i].normalize();
		}
	};


	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		vertex = model.getVertex(verts_idx[nth_vert]);
		
		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

		//Compute light intensity for face  

		//Calculate face normal 
		Vec3f face_normal = model.getFaceNormal(face_idx); //FACE NORMALS ARE UPSIDE DOWN FIX THEM !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		//Transform face normal
		Vec3f trans_normal = Nmat * face_normal;
		trans_normal.normalize();

		varying_intensity[nth_vert] = std::max(0.0f, trans_normal.dot(light_dir[0]));

		return MVPmat *vertex;


	}

	Vec3f fragment(const Vec3f &bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);

		Vec3f rgb;
		if (texture != nullptr)//If model has texture
		{
			rgb = texture->getTexel(u, v);
		}
		else
		{
			rgb = Vec3f(255, 255, 255);
		}

		for (int i = 0; i < 3; i++)
		{
			rgb[i] *= varying_intensity[i];
		}

		return rgb;
	}

};





//Gourad shader

class GouradShader : public IShader {
public:

	//Per Model
	Mat4f MVPmat, MVmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;


	//Phong illumination variables
	Vec3f Ia{ 50,50,50 }, Il{ 199, 134, 36 }; // Ambient and light intensity (defult colour)

	float ka = 0.3, kd = 0.8, ks = 0.4; // Ambient, diffuse, specular coefficients

	float spec_n = 40; // Specular shininess coefficient


	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	//Written by vertex shader
	Vec3f varying_diffuse;
	Vec3f varying_spec;
	Vec2f uv_values[3];



	GouradShader(const Mat4f MV, const Mat4f MVP, const Mat4f N, const std::vector<Light*> &lights) : MVPmat(MVP), MVmat(MV), Nmat(N)
	{
		light_dir.reserve(lights.size());
		for (int i = 0; i < lights.size(); i++)
		{
			light_dir.push_back(lights[i]->m_pos - lights[i]->m_target);
			light_dir[i].normalize();
		}
	};



	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

		//Compute light intensity 

		//Compute light intensity for face  //THIS SHOULD BE DONE ONLY ONCE, NOT INSIDE vertex() BUT SINCE ITS FLAT SHADER IM LEAVING IT UNOPTIMIZED. SAME FOR GOURAD
		

		//Get face normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);


		//Calculate diffuse intensity
		Vec3f trans_normal = Nmat * vertex_normal;
		trans_normal.normalize();
		
		varying_diffuse[nth_vert] = std::max(0.0f, trans_normal.dot(light_dir[0]));


		//Calculate specular intensity
		Vec3f view_dir = MVmat * vertex;
		view_dir.normalize();

		Vec3f reflect_dir = Vec3f::reflect(-light_dir[0], trans_normal);

		varying_spec[nth_vert] = std::pow(std::max(0.0f, -view_dir.dot(reflect_dir)), spec_n);



		return MVPmat * vertex;


	}

	Vec3f fragment(const Vec3f &bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);

		
		if (texture != nullptr)//If model has texture
		{
			Il = texture->getTexel(u, v);
		}

		//Phong reflection
		float diff_NL =   bary.dot(varying_diffuse);
		float spec_RVn =   bary.dot(varying_spec);


		Vec3f colour;
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::min<float>(255, ka*Ia[i] + Il[i]*(diff_NL*kd + spec_RVn*ks));
		}
		
		

		return colour;
	}

};





class PhongShader : public IShader {
public:

	//Per Model
	Mat4f  MVmat, MVPmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;


	//Phong illumination variables
	Vec3f Ia{ 50,50,50 }, Il{ 199, 134, 36 }; // Ambient and light intensity (defult colour)

	float ka = 0.3, kd = 0.8, ks = 0.4; // Ambient, diffuse, specular coefficients

	float spec_n = 32; // Specular shininess coefficient    32

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes
	
	

	//Written by vertex shader
	Vec3f normals[3], view_dir[3];
	Vec2f uv_values[3];


	PhongShader(const Mat4f MV, const Mat4f MVP, const Mat4f N, const std::vector<Light*> &lights) : MVPmat(MVP), MVmat(MV), Nmat(N), texture(nullptr)
	{
		light_dir.reserve(lights.size());
		light_colour.reserve(lights.size());
		for (int i = 0; i < lights.size(); i++)
		{
			light_dir.push_back(lights[i]->m_pos - lights[i]->m_target);
			light_dir[i].normalize();

			light_colour.push_back(lights[i]->m_colour);
		}
	};



	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

	

		//Get vertex normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);

		//Calculate diffuse intensity
		normals[nth_vert] = Nmat * vertex_normal;
		normals[nth_vert].normalize();



		//Calculate specular intensity
		view_dir[nth_vert] = MVmat * vertex;
		view_dir[nth_vert].normalize();

		

		return MVPmat * vertex;

	}

	Vec3f fragment(const Vec3f &bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);

		if (texture != nullptr)//If model has texture
		{
			Il = texture->getTexel(u, v);
		}
		
		//Fragment illumination

		//Interpolate normals and view direction   
		Vec3f interp_normal = normals[0] + (normals[1]-normals[0])*bary.y  + (normals[2] - normals[0])*bary.z;
		Vec3f interp_viewdir = view_dir[0] + (view_dir[1] - view_dir[0]) * bary.y + (view_dir[2] - view_dir[0]) * bary.z;

		interp_normal.normalize();
		interp_viewdir.normalize();


		//FOR EACH LIGHT:
		Vec3f colour;
		for(int i = 0; i < light_dir.size(); i++)
		{ 
			

			//Calculate diffuse component
			float diff_NL = std::max(0.0f, interp_normal.dot(light_dir[i]));

			//Calculate specular component
			Vec3f reflect_dir = Vec3f::reflect(-light_dir[i], interp_normal);
			float spec_RVn = std::pow(std::max(0.0f, -interp_viewdir.dot(reflect_dir)), spec_n); //spec_n can be obtained from mod`el file


			//Illumination equation
			
			for (int j = 0; j < 3; j++)
			{
				colour[j] += std::min<float>(255, light_colour[i][j] * (ka * Ia[j] + Il[j] * (diff_NL * kd + spec_RVn * ks)));
			}
		}
		
		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::min<float>(255, colour[i]);
		}


		return colour;
	}

};





class BlinnPhongShader : public IShader {
public:

	//Per Model
	Mat4f  MVmat, MVPmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;


	//Phong illumination variables
	Vec3f Ia{ 50,50,50 }, Il{ 199, 134, 36 }; // Ambient and light intensity (defult colour)

	float ka = 0.3, kd = 0.8, ks = 0.4; // Ambient, diffuse, specular coefficients

	float spec_n = 32; // Specular shininess coefficient

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes



	//Written by vertex shader
	Vec3f normals[3], view_dir[3];
	Vec2f uv_values[3];


	BlinnPhongShader(const Mat4f MV, const Mat4f MVP, const Mat4f N, const std::vector<Light*> &lights) : MVPmat(MVP), MVmat(MV), Nmat(N), texture(nullptr)
	{
		light_dir.reserve(lights.size());
		light_colour.reserve(lights.size());
		for (int i = 0; i < lights.size(); i++)
		{
			light_dir.push_back(lights[i]->m_pos - lights[i]->m_target);
			light_dir[i].normalize();

			light_colour.push_back(lights[i]->m_colour);
		}
	};



	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}



		//Get vertex normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);

		//Calculate diffuse intensity
		normals[nth_vert] = Nmat * vertex_normal;
		normals[nth_vert].normalize();



		//Calculate specular intensity
		view_dir[nth_vert] = MVmat * vertex;
		view_dir[nth_vert].normalize();



		return MVPmat * vertex;

	}

	Vec3f fragment(const Vec3f &bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);

		if (texture != nullptr)//If model has texture
		{
			Il = texture->getTexel(u, v);
		}

		//Fragment illumination

		//Interpolate normals and view direction   
		Vec3f interp_normal = normals[0] + (normals[1] - normals[0]) * bary.y + (normals[2] - normals[0]) * bary.z;
		Vec3f interp_viewdir = view_dir[0] + (view_dir[1] - view_dir[0]) * bary.y + (view_dir[2] - view_dir[0]) * bary.z;

		interp_normal.normalize();
		interp_viewdir.normalize();


		//FOR EACH LIGHT:
		Vec3f colour;
		for (int i = 0; i < light_dir.size(); i++)
		{
			
			//Calculate diffuse component
			float diff_NL = std::max(0.0f, interp_normal.dot(light_dir[i]));


			//Calculate specular component
			Vec3f halfwayDir = -interp_viewdir + light_dir[i]; //HalfwayDir is lightDir + viewDir but viewDir needs to be negated first to aim from the fragment to eye
			halfwayDir.normalize();

			float spec_RVn = std::pow(std::max(0.0f, interp_normal.dot(halfwayDir)), spec_n); //spec_n can be obtained from model file

			
			//Illumination equation

			for (int j = 0; j < 3; j++)
			{
				colour[j] += std::min<float>(255, light_colour[i][j] * (ka * Ia[j] + Il[j] * (diff_NL * kd + spec_RVn * ks)));
			}
		}

		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::min<float>(255, colour[i]);
		}


		return colour;
	}

};