 #pragma once

#include<vector>
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4.h"
#include "Material.h"
#include "Mesh.h"
#include "OBJLoader.h"

struct PBRTexture {
	
	PBRTexture(std::string texture_fname) {//Currently uses fixed file extension. all jpg and normal is png
		
		//albedo
		std::string albedo = texture_fname;
		m_albedo = Texture(albedo.append("_albedo.jpg").c_str());
		
		
		//AO
		std::string AO = texture_fname;
		m_AO = Texture(AO.append("_AO.jpg").c_str());

		//metallic
		std::string metallic = texture_fname;
		m_metallic = Texture(metallic.append("_metallic.jpg").c_str());

		//normal
		std::string normal = texture_fname;
		m_normal = Texture(normal.append("_normal.jpg").c_str());

		//roughness
		std::string roughness = texture_fname;
		m_roughness = Texture(roughness.append("_roughness.jpg").c_str());
	};

	Texture m_albedo;
	Texture m_AO;
	Texture m_metallic;
	Texture m_normal;
	Texture m_roughness;
};

class Model {

public:
	Model(const char* filename, const Material* material, const Mat4f transform = Mat4f());
	Model(const char* filename, std::string texture_fname, const Material* material, const Mat4f transform = Mat4f());
	
	Model(const Model& other);
	Model& operator = (const Model& other) = delete;
	~Model();

	Mat4f getModelMat() const;
	PBRTexture* getTexture() const;
	const Material* getMaterial() const;
	Mesh* getMesh() const;

private:

	Mesh* m_mesh;
	const Material* m_material;
	Mat4f m_model_mat;

	//PBR textures
	PBRTexture* m_pbrtexture;
};
