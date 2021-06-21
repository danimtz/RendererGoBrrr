 #pragma once

#include<vector>
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4.h"
#include "Material.h"
#include "Mesh.h"
#include "OBJLoader.h"


class PBRTexture {
public:
	PBRTexture();
	PBRTexture(std::string texture_fname);
	PBRTexture(const PBRTexture& other);

	PBRTexture& operator = (const PBRTexture& other) = delete;

	~PBRTexture();


	Texture* m_albedo;
	Texture* m_AO;
	Texture* m_metallic;
	Texture* m_normal;
	Texture* m_roughness;
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

