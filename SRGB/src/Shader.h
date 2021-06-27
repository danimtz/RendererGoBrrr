#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include "Model.h"
#include "Light.h"
#include "Scene.h"
#include "Rasterizer.h"
#include <algorithm>

enum class ShaderType : int {
	PHONG,
	BLINNPHONG,
	PBR,
	ALBEDO,
	NORMALMAP,
	ROUGHNESS,
	METALLIC,
	AO
};

//Output of vertex shader. These would be out variables in GLSL etc.
//Some or redundant for flat/ gourad shading
struct VShaderOut {
	Vec3f position;
	Vec3f viewdir;
	Vec3f normal;
	Vec3f tangent;
	Vec3f bitangent;
	Vec2f texcoord;
};


class IShader {
public:
	
	virtual ~IShader() {};
	virtual VShaderOut vertex(const Vertex &input_vertex) = 0;
	virtual Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) = 0;
	virtual ShaderType getType() = 0;

protected:
	//Helper function that performs barycentric interpolation
	static Vec3f baryInterp(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec3f &bary)
	{
		float interp_x = v0.x + (v1.x - v0.x) * bary.y + (v2.x - v0.x) * bary.z;
		float interp_y = v0.y + (v1.y - v0.y) * bary.y + (v2.y - v0.y) * bary.z;
		float interp_z = v0.z + (v1.z - v0.z) * bary.y + (v2.z - v0.z) * bary.z;
		Vec3f interp_value(interp_x, interp_y, interp_z);
		interp_value.normalize();
		return interp_value;
	};
};





class NormalMapShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVPmat, Nmat; // Matrices
	Texture* normal_map;


	Vec3f getNormalMappedNormal(const Vec3f& t, const Vec3f& b, const Vec3f& n, const Vec2f& uv)
	{
		if (normal_map != nullptr) {
			Vec3f normal = normal_map->getTexel(uv.u, uv.v);
			normal = normal * 2.0f - 1.0f;
			normal.normalize();
			Mat4f TBN = Mat4f::createTBN(t, b, n);
			normal = TBN * normal;
			normal.normalize();
			return normal;
		}
		else
		{
			return n;
		};
	}

	VShaderOut vertex(const Vertex& input_vertex) override
	{

		VShaderOut vout;
		//If texture, load uv values
		if (normal_map != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

		//Calculate TBN vectors
		vout.tangent = Nmat * input_vertex.tangent;
		vout.bitangent = Nmat * (input_vertex.normal.cross(input_vertex.tangent));
		vout.normal = Nmat * input_vertex.normal;


		vout.tangent.normalize();
		vout.bitangent.normalize();
		vout.normal.normalize();


		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{

		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		//Interpolate normals and view direction. Vector split into individual components for optimization. (My own vector library is kinda slow unfortunately)
		Vec3f interp_normal = IShader::baryInterp(v0.normal, v1.normal, v2.normal, bary);
		Vec3f interp_tangent = IShader::baryInterp(v0.tangent, v1.tangent, v2.tangent, bary);
		Vec3f interp_bitangent = IShader::baryInterp(v0.bitangent, v1.bitangent, v2.bitangent, bary);


		if (normal_map != nullptr)//If model has texture
		{
			interp_normal = getNormalMappedNormal(interp_tangent, interp_bitangent, interp_normal, Vec2f(u, v));
		}
		
		Vec3f colour = interp_normal*0.5 + 0.5;

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::NORMALMAP;
	}

};


class AlbedoShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVPmat; // Matrices
	Texture* albedo;


	VShaderOut vertex(const Vertex& input_vertex) override
	{

		VShaderOut vout;
		//If texture, load uv values
		if (albedo != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{

		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		Vec3f colour = Vec3f(1.0f, 0.2f, 0.2f);
		if (albedo != nullptr){
			colour = albedo->getTexel(u,v);
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::ALBEDO;
	}

};



class MetallicShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVPmat; // Matrices
	Texture* metallic;


	VShaderOut vertex(const Vertex& input_vertex) override
	{

		VShaderOut vout;
		//If texture, load uv values
		if (metallic != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{

		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		Vec3f colour = Vec3f(1.0f, 0.2f, 0.2f);
		if (metallic != nullptr) {
			float col = metallic->getTexel(u, v).x;
			colour = Vec3f(col);
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::METALLIC;
	}


};


class RoughnessShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVPmat; // Matrices
	Texture* roughness;


	VShaderOut vertex(const Vertex& input_vertex) override
	{

		VShaderOut vout;
		//If texture, load uv values
		if (roughness != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{

		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		Vec3f colour = Vec3f(1.0f, 0.2f, 0.2f);
		if (roughness != nullptr) {
			float col = roughness->getTexel(u, v).x;
			colour = Vec3f(col);
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::ROUGHNESS;
	}


};


class AOShader : public IShader {
public:


	//UNIFORMS
	Mat4f  MVPmat; // Matrices
	Texture* AO;


	VShaderOut vertex(const Vertex& input_vertex) override
	{

		VShaderOut vout;
		//If texture, load uv values
		if (AO != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{

		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		Vec3f colour = Vec3f(1.0f, 0.2f, 0.2f);
		if (AO != nullptr) {
			float col = AO->getTexel(u, v).x;
			colour = Vec3f(col);
		}

		return colour;
	}

	virtual ShaderType getType() override {
		return ShaderType::AO;
	}


};



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

	
		vout.normal =  Nmat * input_vertex.normal;
		vout.normal.normalize();

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


		//Interpolate normals and view direction. Vector split into individual components for optimization. (My own vector library is kinda slow unfortunately)
		Vec3f interp_normal = IShader::baryInterp(v0.normal, v1.normal, v2.normal, bary);
		Vec3f interp_viewdir = IShader::baryInterp(v0.viewdir, v1.viewdir, v2.viewdir, bary);


		//Fragment illumination
		Vec3f Ia_ = Ia; //Prevent data race
		Vec3f Il_ = Il;
		if (texture != nullptr)//If model has texture
		{
			Ia_ = texture->getTexel(u, v);
			Il_ = Ia_;
		}

		//FOR EACH LIGHT:
		Vec3f colour;
		for(int i = 0; i < light_dir.size(); i++)
		{ 
			

			//Calculate diffuse component
			float diff_NL = std::fmax(0.0f, interp_normal.dot(light_dir[i]));


			//Calculate specular component
			Vec3f reflect_dir = Vec3f::reflect(-light_dir[i], interp_normal);

			float spec_RVn = 0.0f;
			if (diff_NL > 0.0f){//Test for negative reflection
				spec_RVn = std::pow<float>(std::fmax<float>(0.0f, -interp_viewdir.dot(reflect_dir)), spec_n); //spec_n can be obtained from mod`el file
			}

		
			//Illumination equation
			float spec_diff_component = diff_NL * kd;// + spec_RVn * ks;
			for (int j = 0; j < 3; j++)
			{
				colour[j] += light_colour[i][j] * (ka * Ia_[j] + Il_[j] * (spec_diff_component));//only specular and ambient for now
			}
		}
		
		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::fmin(1.0f, colour[i]);
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
	Texture *normal_map;
	std::vector<Vec3f> light_dir;
	std::vector<Vec3f> light_colour;

	//Phong illumination variables
	Vec3f Ia, Il; // Ambient and light intensity (defult colour)
	float ka, kd, ks; // Ambient, diffuse, specular coefficients
	float spec_n; // Specular shininess coefficient    32


	Vec3f getNormalMappedNormal(const Vec3f& t, const Vec3f& b, const Vec3f& n, const Vec2f& uv)
	{
		if (normal_map != nullptr) {
			Vec3f normal = normal_map->getTexel(uv.u, uv.v);
			normal = normal * 2.0f - 1.0f;
			normal.normalize();
			Mat4f TBN = Mat4f::createTBN(t, b, n);
			normal = TBN * normal;
			normal.normalize();
			return normal;
		}
		else
		{
			return n;
		};
	}

	VShaderOut vertex(const Vertex &input_vertex) override
	{
		
		VShaderOut vout;
		//If texture, load uv values
		if (texture != nullptr)
		{
			vout.texcoord = input_vertex.texcoord;
		}

		//Calculate TBN vectors
		vout.tangent = Nmat * input_vertex.tangent; 
		vout.bitangent = Nmat * (input_vertex.normal.cross(input_vertex.tangent));
		vout.normal = Nmat * input_vertex.normal;
		

		vout.tangent.normalize();
		vout.bitangent.normalize();
		vout.normal.normalize();


		//Calculate view direction
		vout.viewdir = (MVmat * input_vertex.position);

		//Calculate vertex position post transform
		vout.position = MVPmat * input_vertex.position;

		return vout;

	}

	Vec3f fragment(const Vec3f& bary, const VShaderOut& v0, const VShaderOut& v1, const VShaderOut& v2) override
	{

		//This part is usually taken care of in GLSL by setting in and out varaibles from vertex shadew to fragment shader and are already interpolated when used in fragment shader

		//Interpolate texture uv values
		float u = v0.texcoord.u + (v1.texcoord.u - v0.texcoord.u) * bary.y + (v2.texcoord.u - v0.texcoord.u) * bary.z;
		float v = v0.texcoord.v + (v1.texcoord.v - v0.texcoord.v) * bary.y + (v2.texcoord.v - v0.texcoord.v) * bary.z;

		//Interpolate normals and view direction. Vector split into individual components for optimization. (My own vector library is kinda slow unfortunately)
		Vec3f interp_normal = IShader::baryInterp(v0.normal, v1.normal, v2.normal, bary);
		Vec3f interp_tangent = IShader::baryInterp(v0.tangent, v1.tangent, v2.tangent, bary);
		Vec3f interp_bitangent = IShader::baryInterp(v0.bitangent, v1.bitangent, v2.bitangent, bary);
		Vec3f interp_viewdir = -IShader::baryInterp(v0.viewdir, v1.viewdir, v2.viewdir, bary);


		
		Vec3f Ia_ = Ia;//Prevent data race
		Vec3f Il_ = Il;
		if (texture != nullptr)//If model has texture
		{
			Ia_ = texture->getTexel(u, v);
			Il_ = Ia_;
			interp_normal = getNormalMappedNormal(interp_tangent, interp_bitangent, interp_normal, Vec2f(u,v));
			
		}


		//Fragment illumination

		//FOR EACH LIGHT:
		Vec3f colour;
		for (int i = 0; i < light_dir.size(); i++)
		{


			//Calculate diffuse component
			float diff_NL = std::fmax(0.0f, interp_normal.dot(light_dir[i]));


			//Calculate specular component
			Vec3f halfwayDir = interp_viewdir + light_dir[i]; //HalfwayDir is lightDir + viewDir but viewDir needs to be negated first to aim from the fragment to eye
			halfwayDir.normalize();

		

			float spec_RVn = 0.0f;
			if (diff_NL > 0.0f) {//Test for negative reflection
				spec_RVn = std::pow(std::fmax(0.0f, interp_normal.dot(halfwayDir)), spec_n); //spec_n can be obtained from model file
			}

			
			//Illumination equation
			for (int j = 0; j < 3; j++)
			{
				colour[j] +=  light_colour[i][j] * (ka * Ia_[j] + Il_[j] * (diff_NL * kd + spec_RVn * ks));//only specular and ambient for now
			}
		}

		//Cap intensity values
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::fmin(1.0f, colour[i]);
		}

		//Debug
		//if(bary.x <0.01 || bary.y <0.01 || bary.z<0.01){colour = Vec3f(1.0f, 0.0f, 0.0f);}

		//colour = interp_normal*0.5 + 0.5;

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
	float Ka; //Ambient coefficient
	Texture* albedo_map;
	Texture* normal_map;
	Texture* roughness_map;
	Texture* metallic_map;
	Texture* AO_map;

	std::vector<Vec3f> light_dir; //Can be placed here as a uniform because they are all directional lights, so direction is constant regardless of fragment position
	std::vector<Vec3f> light_colour;


	

	Vec3f getNormalMappedNormal(const Vec3f& t, const Vec3f& b, const Vec3f& n, const Vec2f& uv) 
	{
		if (normal_map != nullptr){
			Vec3f normal = normal_map->getTexel(uv.u, uv.v);
			normal = normal * 2.0f - 1.0f;
			normal.normalize();
			Mat4f TBN = Mat4f::createTBN(t, b, n);
			normal = TBN * normal;
			normal.normalize();
			return normal;
		}
		else
		{
			return n;
		};
	}

	float distributionGGX(const Vec3f &N, const Vec3f &H, float roughness)//Trowbridge-Reitz GGX normal distribution function 
	{
		float a = roughness * roughness;//alpha
		float a2 = a*a;
		float NdotH = std::fmax(N.dot(H), 0.0f);
		float NdotH2 = NdotH*NdotH;

		float num = a2;
		float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
		float inv_denom = 1.0f/(M_PI* denom * denom);

		return num * inv_denom;
	}


	Vec3f fresnelSchlick(const float cos_theta, const Vec3f &F0) //Fresnel Schlick approximation function
	{
		float neg_cos_theta = 1.0f - cos_theta;
		return F0 + (Vec3f(1.0f)-F0) * (neg_cos_theta * neg_cos_theta * neg_cos_theta * neg_cos_theta * neg_cos_theta);//std::pow is expensive
	}

	float geometrySmith(const Vec3f &N, const Vec3f &V, const Vec3f &L, const float &roughness)//Geometry function
	{
		float r_1 = roughness + 1.0f;
		float k = (r_1 * r_1) / 8.0f;

		float NdotV = std::fmax(N.dot(V), 0.0f);
		float NdotL = std::fmax(N.dot(L), 0.0f);
		float obstGGX = geometrySchlickGGX(NdotV, k); //Geometry obstruction
		float shdwGGX = geometrySchlickGGX(NdotL, k); //Geometry shadowing

		return obstGGX * shdwGGX;
	}

	float geometrySchlickGGX(const float &Ndotx, const float &k)//Geometry obstruction/shadowing calculator using SchlickGGX approx.
	{
		float num = Ndotx;
		float inv_denom = 1.0f/(Ndotx*(1.0f-k)+k);
		
		return num * inv_denom;
	}

	VShaderOut vertex(const Vertex& input_vertex) override
	{
		VShaderOut vout;

		vout.texcoord = input_vertex.texcoord;

		//Calculate TBN vectors
		vout.tangent = Nmat * input_vertex.tangent; 
		vout.bitangent = Nmat * (input_vertex.normal.cross(input_vertex.tangent));
		vout.normal = Nmat * input_vertex.normal;

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

		//Interpolate normals and view direction. Vector split into individual components for optimization. (My own vector library is kinda slow unfortunately)
		Vec3f interp_normal = IShader::baryInterp(v0.normal, v1.normal, v2.normal, bary);
		Vec3f interp_tangent = IShader::baryInterp(v0.tangent, v1.tangent, v2.tangent, bary);
		Vec3f interp_bitangent = IShader::baryInterp(v0.bitangent, v1.bitangent, v2.bitangent, bary);
		Vec3f V = -IShader::baryInterp(v0.viewdir, v1.viewdir, v2.viewdir, bary);
		//INTERPOLATE LIGHT DIRECTIONS HERE IF POINT LIGHTS ADDDED
		

		//Sample textures===========
		
		//albedo
		Vec3f albedo = Vec3f(0.2f);
		if (albedo_map != nullptr) 
		{
			//NEEDS GAMMA CORRECTION
			float gamma = 2.2f;
			albedo.x = std::pow(albedo_map->getTexel(u, v).x, gamma);
			albedo.y = std::pow(albedo_map->getTexel(u, v).y, gamma);
			albedo.z = std::pow(albedo_map->getTexel(u, v).z, gamma);
		}
		
		Vec3f N = getNormalMappedNormal(interp_tangent, interp_bitangent, interp_normal, Vec2f(u, v));

		//roughness
		float roughness = 1.0f;
		if (roughness_map != nullptr)
		{
			roughness = roughness_map->getTexel(u, v).x;
		}
		//metallic
		float metallic = 0.2f;
		if (metallic_map != nullptr)
		{
			metallic = metallic_map->getTexel(u, v).x;
		}
		//AO
		float AO = 1.0;
		if (AO_map != nullptr)
		{
			AO = AO_map->getTexel(u, v).x;
		}


		
		
		
		


		//Calculate F0 (Used in fresnel)
		Vec3f F0 = Vec3f(0.04);
		F0 = F0*(1 - metallic) + albedo*(metallic); //Lerp using metallic as t between albedo and F0


		float NdotV = std::fmax(N.dot(V), 0.0f); //Can be calculated once outside loop
		Vec3f Lo = Vec3f(0.0f); //Output radiance


		//This loop can be vectorized with omp simd but needs investigating how to do it
		for (int i = 0; i < light_dir.size(); i++)
		{
			//Per light radiance and costheta calculation(dot product)
			Vec3f H = V + light_dir[i];
			H.normalize();
			Vec3f radiance = light_colour[i]; //Could be light colour * attenuation but using directional lights atm
			float NdotL = std::fmax(N.dot(light_dir[i]), 0.0f);

			//////////Cook Torrance BRDF calculation//////////////
			
			float NDF = distributionGGX(N, H, roughness);//Normal distribution function
			Vec3f F = fresnelSchlick(std::fmax(H.dot(light_dir[i]), 0.0f), F0);//Fresnel
			float G = geometrySmith(N, V, light_dir[i], roughness);//Geometry

			//Calcualte diffuse coefficient
			Vec3f Kd = (Vec3f(1.0f) - F)*(1.0f-metallic); //Fully metallic materials dont have diffuse

			Vec3f numerator = F * NDF * G;
			Vec3f inv_denominator = 1.0f / std::fmax( 4.0f * (NdotL * NdotV), 0.001); //Account for cases where dot product is 0

			Vec3f specular = numerator * inv_denominator;
			Vec3f diffuse = albedo/M_PI;

			//Add up radiance
			Lo += (Kd * diffuse + specular) * radiance * NdotL;

		}
		
		//Ambient light
		Vec3f ambient = albedo * Ka * AO;

		//Final colour, HDR tone mapping???? and gamma correction
		Vec3f colour = (ambient + Lo *AO);
		
		//Cap intensity values and gamma correct
		float invgamma = 0.4545;
		for (int i = 0; i < 3; i++)
		{
			colour[i] = std::pow(colour[i], invgamma);
			colour[i] = std::fmin(1.0f, colour[i]);
		}
		
		//colour = colour / (colour + Vec3f(1.0f));
		//colour.x = std::pow(colour.x, Vec3f(1.0f / 2.2f).x);
		//colour.y = std::pow(colour.y, Vec3f(1.0f / 2.2f).y);
		//colour.z = std::pow(colour.z, Vec3f(1.0f / 2.2f).z);
		



		//Mat4f TBN = Mat4f::createTBN(interp_tangent, interp_bitangent, interp_normal);
		//TBN.transpose();
		

		return colour;
	}

	virtual ShaderType getType() override 
	{
		return ShaderType::PBR;
	}

};