 #pragma once

#include<vector>
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4.h"
#include "Material.h"
#include "Mesh.h"
#include "OBJLoader.h"

//TODO: split into model and mesh(add Vertex Buffers and Index Buffers) Then change Render loop  to render from renderer and not from the scene class.

class Model {

public:
	Model(const char* filename, const Material* material, const Mat4f transform = Mat4f());
	Model(const char* filename, const char* texture_fname, const Material* material, const Mat4f transform = Mat4f());
	
	Model(const Model& other);
	Model& operator = (const Model& other) = delete;
	~Model();

	Mat4f getModelMat() const;
	Texture* getTexture() const;
	const Material* getMaterial() const;
	Mesh* getMesh() const;

private:

	Mesh* m_mesh;
	Texture* m_texture;
	const Material* m_material;
	Mat4f m_model_mat;
	V_Data v_data; //IF THIS GIVES ERROR THEN IT WORKED
};
/*
class Model {



public:

	//Constructor that will call OBJ loader function to fill model data
	Model(const char* filename, const Material *material, const Mat4f transform = Mat4f());
	Model(const char* filename, const char* texture_fname, const  Material *material, const Mat4f transform = Mat4f());
	
	//COPY CONSTRUCTOR AND ASSIGNMENT detruct
	Model(const Model &other);
	Model& operator = (const Model &other) = delete;
	~Model();

	 




	//getters for normals, vertices and uv given the face number and vertex etc etc
	
	int getFaceCount() const;
	int getVertCount() const;

	Vec3f getVertex(int n) const;
	Vec3f getVertex(int nface, int nth_vert) const;

	Vec2f getUV(int n) const;

	Vec3i getFaceVertices(int nface) const;

	Vec3i getUVidx(int nface) const;//COULD PROBABLY COMBINE GETUV AND GETUVIDX. SAME FOR VERTEX (change in renderer/rasterizer/shader too)

	Mat4f getModelMat() const;
	//TODO:
	Vec3f getFaceNormal(int nface) const;

	Vec3f getVertexNormal(int nface, int nth_vert) const;

	Texture* getTexture() const;
	const Material* getMaterial() const;

	

private:
	
	//TEMP move this to mesh class later.

	//Obtained from face command in OBJ file
	std::vector<Vec3f> m_vertex;
	std::vector<Vec3f> m_vnorms;
	std::vector<Vec2f> m_uv;
	Mat4f m_model_mat;
	std::vector<Vec3f> m_fnorms;

	static constexpr int MAXPOLY = 20;

	//Faces defined by face command. 
	//Vec3i of vertex/uv/vnormal points to the respective index in m_vertex/m_uv/m_norms
	std::vector<std::vector<Vec3i>> m_faces;

	//Model texture
	Texture *m_texture;

	//Model material
	const Material *m_material;

	

	//OBJ file parsing funcitons //THESE FUNCTIONS COULD MAYBE BE DECOUPLED FROM THE MODEL CLASS
	void loadOBJfile(const char* filename, const char* texture_fname = nullptr);
	void parseFaceData(std::string(&face_data)[MAXPOLY], int vCount);
	void buildFaceNormals();
	void buildVertexNormals();

};

*/