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
	BLINNPHONG,
	PBR
};

//Output of vertex shader. These would be out variables in GLSL etc.
//Some or redundant for flat/ gourad shading
struct VShaderOut {
	Vec3f position;
	Vec3f normal;
	Vec2f texcoord;
	Vec3f viewdir;
	Mat4f TBN;
};


class IShader {
public:
	
	virtual ~IShader() {};
	virtual VShaderOut vertex(const Vertex &input_vertex) = 0;
	virtual Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) = 0;
	virtual ShaderType getType() = 0;
};



//Flat and Gourad 
/*
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

*/



class PhongShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVmat, MVPmat, Vmat, Nmat; // Matrices
	Texture *texture;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;

	//Phong illumination variables
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)
	float ka, kd, ks; // Ambient, diffuse, specular coefficients
	float spec_n; // Specular shininess coefficient    32




	VShaderOut vertex(const Vertex &input_vertex) override
	{
		
		VShaderOut vout;
		//If texture, load uv values
		if (texture != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

	

		//Calculate diffuse intensity
		vout.normal =  Nmat * input_vertex.normal;
		vout.normal.normalize();

		//Calculate specular intensity
		vout.viewdir = (MVmat * input_vertex.position);

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{
		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		//Prevent data race
		Vec3f Ia_ = Ia;
		Vec3f Il_ = Il;
		if (texture != nullptr)//If model has texture
		{
			Ia_ = texture->getTexel(u, v);
			Il_ = Ia_;
		}
		

		//Fragment illumination

		//Interpolate normals and view direction. Vector split into individual components for optimization. (My own vector library is kinda slow unfortunately)
		float interp_norm_x = v0.normal.x + (v1.normal.x - v0.normal.x) * bary.y + (v2.normal.x - v0.normal.x) * bary.z;
		float interp_norm_y = v0.normal.y + (v1.normal.y - v0.normal.y) * bary.y + (v2.normal.y - v0.normal.y) * bary.z;
		float interp_norm_z = v0.normal.z + (v1.normal.z - v0.normal.z) * bary.y + (v2.normal.z - v0.normal.z) * bary.z;
		Vec3f interp_normal(interp_norm_x, interp_norm_y, interp_norm_z);
		interp_normal.normalize();

		float interp_view_x = v0.viewdir.x + (v1.viewdir.x - v0.viewdir.x) * bary.y + (v2.viewdir.x - v0.viewdir.x) * bary.z;
		float interp_view_y = v0.viewdir.y + (v1.viewdir.y - v0.viewdir.y) * bary.y + (v2.viewdir.y - v0.viewdir.y) * bary.z;
		float interp_view_z = v0.viewdir.z + (v1.viewdir.z - v0.viewdir.z) * bary.y + (v2.viewdir.z - v0.viewdir.z) * bary.z;
		Vec3f interp_viewdir(interp_view_x, interp_view_y, interp_view_z);
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
				spec_RVn = std::pow<float>(std::max<float>(0.0f, -interp_viewdir.dot(reflect_dir)), spec_n); //spec_n can be obtained from mod`el file
			}

		
			//Illumination equation
			float spec_diff_component = diff_NL * kd + spec_RVn * ks;
			for (int j = 0; j < 3; j++)
			{
				colour[j] += light_colour[i][j] * (ka * Ia_[j] + Il_[j] * (spec_diff_component));//only specular and ambient for now
			}
		}
		
		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::min<float>(255.0f, colour[i]);
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::PHONG;
	}

};




class BlinnPhongShader : public IShader {
public:

	
	//UNIFORMS
	Mat4f  MVmat, MVPmat, Vmat, Nmat; // Matrices
	Texture *texture;
	Texture *normalmap;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;

	//Phong illumination variables
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)
	float ka, kd, ks; // Ambient, diffuse, specular coefficients
	float spec_n; // Specular shininess coefficient    32




	VShaderOut vertex(const Vertex &input_vertex) override
	{
		
		VShaderOut vout;
		//If texture, load uv values
		if (texture != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

	

		//Calculate normal
		vout.normal =  Nmat * input_vertex.normal;
		vout.normal.normalize();

		//Calculate TBN matrix
		Vec3f T = MVmat * input_vertex.tangent;
		Vec3f B = MVmat * input_vertex.normal.cross(input_vertex.tangent);
		Vec3f N = MVmat * input_vertex.normal;
		vout.TBN = Mat4f::createTBNmat(T,B,N);
		

		//Calculate view direction
		vout.viewdir = (MVmat * input_vertex.position);

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{
		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		


		//Fragment illumination

		//Interpolate normals and view direction. Vector split into individual components for optimization. (My own vector library is kinda slow unfortunately)
		float interp_norm_x = v0.normal.x + (v1.normal.x - v0.normal.x) * bary.y + (v2.normal.x - v0.normal.x) * bary.z;
		float interp_norm_y = v0.normal.y + (v1.normal.y - v0.normal.y) * bary.y + (v2.normal.y - v0.normal.y) * bary.z;
		float interp_norm_z = v0.normal.z + (v1.normal.z - v0.normal.z) * bary.y + (v2.normal.z - v0.normal.z) * bary.z;
		Vec3f interp_normal(interp_norm_x, interp_norm_y, interp_norm_z);
		interp_normal.normalize();

		float interp_view_x = v0.viewdir.x + (v1.viewdir.x - v0.viewdir.x) * bary.y + (v2.viewdir.x - v0.viewdir.x) * bary.z;
		float interp_view_y = v0.viewdir.y + (v1.viewdir.y - v0.viewdir.y) * bary.y + (v2.viewdir.y - v0.viewdir.y) * bary.z;
		float interp_view_z = v0.viewdir.z + (v1.viewdir.z - v0.viewdir.z) * bary.y + (v2.viewdir.z - v0.viewdir.z) * bary.z;
		Vec3f interp_viewdir(interp_view_x, interp_view_y, interp_view_z);
		interp_viewdir.normalize();
		


		//Prevent data race
		Vec3f Ia_ = Ia;
		Vec3f Il_ = Il;
		if (texture != nullptr)//If model has texture
		{
			Ia_ = texture->getTexel(u, v);
			Il_ = Ia_;
			
			Vec3f normal = normalmap->getTexel(u, v); //NEEDS TBN MAT ETC
			interp_normal = normal*2.0f - 1.0f;
			interp_normal.normalize();


		}




		//FOR EACH LIGHT:
		Vec3f colour;
		for (int i = 0; i < light_dir.size(); i++)
		{


			//Calculate diffuse component
			float diff_NL = std::max(0.0f, interp_normal.dot(light_dir[i]));


			//Calculate specular component
			Vec3f halfwayDir = -interp_viewdir + light_dir[i]; //HalfwayDir is lightDir + viewDir but viewDir needs to be negated first to aim from the fragment to eye
			halfwayDir.normalize();

		

			float spec_RVn = 0.0f;
			if (diff_NL > 0.0f) {//Test for negative reflection
				spec_RVn = std::pow(std::max(0.0f, interp_normal.dot(halfwayDir)), spec_n); //spec_n can be obtained from model file
			}

			

			//Illumination equation

			for (int j = 0; j < 3; j++)
			{
				colour[j] += std::min<float>(255, light_colour[i][j] * (ka * Ia_[j] + Il_[j] * (diff_NL * kd + spec_RVn * ks)));//only specular and ambient for now
			}
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::BLINNPHONG;
	}

};


class PBRShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVmat, MVPmat, Vmat, Nmat; // Matrices
	Texture& albedo;
	Texture& metallic;
	Texture& AO;
	Texture& normal;
	Texture& roughness;

	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;



	VShaderOut vertex(const Vertex& input_vertex) override
	{
		VShaderOut vout;

		vout.texcoord = input_vertex.texcoord;

		vout.normal = Nmat * input_vertex.normal;//Might not be needed since normal obtained form normalmap
		vout.normal.normalize();

		//Calculate view direction
		vout.viewdir = (MVmat * input_vertex.position);

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;
	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{
		Vec3f colour;
		return colour;
	}

	virtual ShaderType getType() override 
	{
		return ShaderType::PBR;
	}

};