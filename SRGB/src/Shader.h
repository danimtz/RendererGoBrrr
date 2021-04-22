#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include "Model.h"
#include "Light.h"
#include "Scene.h"
#include "Rasterizer.h"
#include <algorithm>

enum class ShaderType : int {
	FLAT,
	GOURAD,
	PHONG,
	DEBUG
};
class IShader {
public:
	
	virtual ~IShader() {};
	virtual Vec3f vertex(const Model &model, int face_idx, int nth_vert) = 0;
	virtual Vec3f fragment(const Vec3f& bary) = 0;
	virtual ShaderType getType() = 0;
};



//Flat and Gourad 
class FlatShader : public IShader {
public:

	//Per Model
	Mat4f MVPmat, MVmat, Vmat, Nmat; // Matrices  
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;

	Vec3f Ia, Il; // Ambient and light intensity (defult colour)
	float ka, kd; // Ambient, diffuse coefficients

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	//Written by vertex shader
	Vec3f varying_intensity;
	Vec2f uv_values[3];




	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		verts_idx = model.getFaceVertices(face_idx);
		vertex = model.getVertex(verts_idx[nth_vert]);
		
		//If texture, load uv values
		if (texture != nullptr)
		{

			uv_idx = model.getUVidx(face_idx);
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

		//Compute light intensity for face  

		//Calculate face normal 
		Vec3f face_normal = Nmat*model.getFaceNormal(face_idx); 
		face_normal.normalize();
	

		varying_intensity[nth_vert] = std::max(0.0f, face_normal.dot(light_dir[0]));
		
		
		return MVPmat *vertex;


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
		

		Vec3f colour;
		for (int j = 0; j < 3; j++)
		{
			colour[j] += std::min<float>(255, light_colour[0][j] * (ka * Ia[j] + Il[j] * (varying_intensity[j] * kd)));
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::FLAT;
	}


};








//Gourad shader

class GouradShader : public IShader {
public:

	//Per Model
	Mat4f MVPmat, MVmat, Vmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;

	//Phong illumination variables
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)

	float ka, kd, ks; // Ambient, diffuse, specular coefficients

	float spec_n; // Specular shininess coefficient


	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	//Written by vertex shader
	Vec3f varying_diffuse;
	Vec3f varying_spec;
	Vec2f uv_values[3];






	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		verts_idx = model.getFaceVertices(face_idx);
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_idx = model.getUVidx(face_idx);
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

		//Compute light intensity 

		//Compute light intensity for face  //THIS SHOULD BE DONE ONLY ONCE, NOT INSIDE vertex() BUT SINCE ITS FLAT SHADER IM LEAVING IT UNOPTIMIZED. SAME FOR GOURAD
		

		//Get face normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);


		//Calculate diffuse intensity
		Vec3f trans_normal = Nmat * vertex_normal;
		trans_normal.normalize();
		
		float diff = std::max(0.0f, trans_normal.dot(light_dir[0]));
		varying_diffuse[nth_vert] = diff;

		//Calculate specular intensity
		Vec3f view_dir = MVmat * vertex;
		view_dir.normalize();

		Vec3f reflect_dir = Vec3f::reflect(-light_dir[0], trans_normal);

		float spec = 0.0f;
		if (diff > 0.0f) {//Test for negative reflection
			spec = std::pow(std::max(0.0f, -view_dir.dot(reflect_dir)), spec_n);
		}
		varying_spec[nth_vert] = spec;

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
			Ia = texture->getTexel(u, v);

		}

		//Phong reflection
		float diff_NL =   bary.dot(varying_diffuse);
		float spec_RVn =  bary.dot(varying_spec);


		Vec3f colour;
		for (int j = 0; j < 3; j++)
		{
			
			colour[j] += std::min<float>(255, light_colour[0][j] * (ka * Ia[j] + Il[j] * (diff_NL * kd + spec_RVn * ks)));//only specular and ambient for now	
		}
		

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::GOURAD;
	}

};








//BlinnPhong OLD
#if 0 



class BlinnPhongShader : public IShader {
public:

	//Per Model
	Mat4f  MVmat, MVPmat, Vmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;


	//Phong illumination variables from material
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)

	float ka, kd, ks; // Ambient, diffuse, specular coefficients

	float spec_n; // Specular shininess coefficient

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes



	//Written by vertex shader
	Vec3f normals[3], view_dir[3];
	Vec2f uv_values[3];


	BlinnPhongShader(const Mat4f MV, const Mat4f MVP, const Mat4f V, const Mat4f N, const SceneLights* sceneLights, const Material* material)
		: MVPmat(MVP), MVmat(MV), Vmat(V), Nmat(N), texture(nullptr), Ia(material->m_Ia), Il(material->m_Il), ka(material->m_ka), kd(material->m_kd), ks(material->m_ks), spec_n(material->m_spec_n)
	{
		auto lights = sceneLights->dirLights;
		light_dir.reserve(lights.size());
		light_colour.reserve(lights.size());
		for (int i = 0; i < lights.size(); i++)
		{
			light_dir.push_back(Vmat * lights[i].m_direction);
			light_dir[i].normalize();


			light_colour.push_back(lights[i].m_colour);
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


		//Calculate normals at vertex
		normals[nth_vert] = Nmat * vertex_normal;
		normals[nth_vert].normalize();

		//Calculate view direction to vertex
		view_dir[nth_vert] = MVmat * vertex;
		view_dir[nth_vert].normalize();

		//Transform light to view space


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
		Vec3f baryc(bary.x, bary.y, bary.z);
		Vec3f interp_normal = normals[0] * baryc.x + normals[1] * baryc.y + normals[2] * baryc.z;//normals[0] + (normals[1] - normals[0]) * bary.y + (normals[2] - normals[0]) * bary.z;
		Vec3f interp_viewdir = view_dir[0] * baryc.x + view_dir[1] * baryc.y + view_dir[2] * baryc.z;//view_dir[0] + (view_dir[1] - view_dir[0]) * bary.y + (view_dir[2] - view_dir[0]) * bary.z;


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

#endif


class PhongShader : public IShader {
public:

	//Per Model
	Mat4f  MVmat, MVPmat, Vmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;


	//Phong illumination variables
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)

	float ka, kd, ks; // Ambient, diffuse, specular coefficients

	float spec_n; // Specular shininess coefficient    32

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes
	
	

	//Written by vertex shader
	Vec3f normals[3], view_dir[3];
	Vec2f uv_values[3];




	Vec3f vertex(const Model &model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		verts_idx = model.getFaceVertices(face_idx);
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_idx = model.getUVidx(face_idx);
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

	

		//Get vertex normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);

		//Calculate diffuse intensity
		normals[nth_vert] =  Nmat * vertex_normal;    
		normals[nth_vert].normalize();

		//Calculate specular intensity
		view_dir[nth_vert] = (MVmat * vertex);  
		

		

		return MVPmat * vertex;

	}

	Vec3f fragment(const Vec3f& bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);
		 

		if (texture != nullptr)//If model has texture
		{
			Ia = texture->getTexel(u, v);
			Il = texture->getTexel(u, v);
		}
		

		

		//Fragment illumination

		//Interpolate normals and view direction   THE BARYCENTRIC COORDINATES ARE CURRENTLY IN SCREEN SPACE?
		Vec3f interp_normal  = normals[0]  + (normals[1]  -  normals[0]) * bary.y + (normals[2]  -  normals[0]) * bary.z;
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

			float spec_RVn = 0.0f;
			if (diff_NL > 0.0f){//Test for negative reflection
				spec_RVn = std::pow(std::max(0.0f, -interp_viewdir.dot(reflect_dir)), spec_n); //spec_n can be obtained from mod`el file
			}

			//DEBUG
			if (spec_RVn > 0.99f) {
				//printf("a");
			}

			//Illumination equation
			
			for (int j = 0; j < 3; j++)
			{
				colour[j] += std::min<float>(255, light_colour[i][j] * (ka * Ia[j] + Il[j] * (diff_NL * kd + spec_RVn * ks)));//only specular and ambient for now
			}
		}
		
		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::min<float>(255, colour[i]);
		}


		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::PHONG;
	}

};






/*


class DebugShader : public IShader {
public:

	//Per Model
	Mat4f  Mmat, MVmat, MVPmat, Vmat, Nmat; // Matrices
	Texture* texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;

	Vec3f camera_pos; //camera position ion world space

	//Phong illumination variables
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)

	float ka, kd, ks; // Ambient, diffuse, specular coefficients

	float spec_n; // Specular shininess coefficient    32

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	


	//Written by vertex shader
	Vec3f normals[3], view_dir[3], vertex_colour[3];
	Vec3f fragment_w;
	Vec3f fragment_pos[3];
	Vec2f uv_values[3];

	


	Vec3f vertex(const Model& model, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		verts_idx = model.getFaceVertices(face_idx);
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_idx = model.getUVidx(face_idx);
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}



		//Get vertex normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);

		//Calculate diffuse intensity
		normals[nth_vert] = Nmat * vertex_normal;
		normals[nth_vert].normalize();


		//normals[nth_vert] = normals[nth_vert];
		//normals[nth_vert].normalize();


		//Calculate specular intensity
		//view_dir[nth_vert] = (MVmat * vertex);   //VIEW DIRECTION IN VIEW SPACE
		//view_dir[nth_vert].normalize();


	

		Vec3f transformed_vertex = MVPmat * vertex;
		fragment_w[nth_vert] = transformed_vertex.w;

		fragment_pos[nth_vert] = Mmat * vertex; //out

		return transformed_vertex;

	}

	Vec3f fragment(const Vec3f& bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);


		if (texture != nullptr)//If model has texture
		{
			Ia = texture->getTexel(u, v);
			Il = texture->getTexel(u, v);
		}

		//Fragment illumination

		//Interpolate normals and view direction   THE BARYCENTRIC COORDINATES ARE CURRENTLY IN SCREEN SPACE?
		Vec3f interp_normal = normals[0] + (normals[1] - normals[0]) * bary.y + (normals[2] - normals[0]) * bary.z;
		Vec3f interp_fragpos = fragment_pos[0] + (fragment_pos[1] - fragment_pos[0]) * bary.y + (fragment_pos[2] - fragment_pos[0]) * bary.z; //in

		interp_normal.normalize();
		interp_fragpos.normalize();

	

		////////////////////////////////////////////////////////////////////////////////////
		
		//FOR EACH LIGHT:
		Vec3f colour;
		for (int i = 0; i < light_dir.size(); i++)
		{


			//Calculate diffuse component
			float diff_NL = std::max(0.0f, interp_normal.dot(light_dir[i]));


			//Calculate specular component
			Vec3f reflect_dir = Vec3f::reflect(-light_dir[i], interp_normal);
			reflect_dir.normalize();

			float spec_RVn = 0.0f;
			if (diff_NL > 0.0f) {//Test for negative reflection
				Vec3f viewDir = camera_pos - interp_fragpos;
				viewDir.normalize();
				spec_RVn = std::pow(std::max(0.0f, viewDir.dot(reflect_dir)), spec_n); //spec_n can be obtained from mod`el file
			}

		

			//Illumination equation

			for (int j = 0; j < 3; j++)
			{
				colour[j] += std::min<float>(255, light_colour[i][j] * (ka * Ia[j] + Il[j] * (diff_NL * kd + spec_RVn * ks)));//only specular and ambient for now
			}
		}

		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::min<float>(255, colour[i]);
		}


		return colour;


	}

	virtual ShaderType getType() override {
		return ShaderType::DEBUG;
	}

};
*/