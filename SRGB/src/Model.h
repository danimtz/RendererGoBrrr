#pragma once

#include<vector>
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4.h"

class Model {


public:

	//Constructor that will call OBJ loader function to fill model data
	Model(const char* filename, const Mat4f transform = Mat4f());
	Model(const char* filename, const char* texture_fname, const Mat4f transform = Mat4f());
	~Model();

	//getters for normals, vertices and uv given the face number and vertex etc etc
	
	int getFaceCount() const;
	int getVertCount() const;

	Vec3f getVertex(int n) const;
	Vec3f getVertex(int nface, int nth_vert) const;


	Vec3i  getFaceVertices(int nface) const;
	
	//TODO:
	//Vec3f getNormal() const;
	

private:
	
	//TEMP move this to mesh class later.

	//Obtained from face command in OBJ file
	std::vector<Vec3f> m_vertex;
	std::vector<Vec3f> m_vnorms;
	std::vector<Vec2f> m_uv;
	Mat4f m_model_mat;
	// face normals member variable??

	//Faces defined by face command. 
	//Vec3i of vertex/uv/vnormal points to the respective index in m_vertex/m_uv/m_norms
	std::vector<std::vector<Vec3i>> m_faces;

	//Model texture
	Texture *m_texture;


	//OBJ file parsing funcitons //THIS FUNCTION COULD MAYBE BE DECOUPLED FROM THE MODEL CLASS
	void loadOBJfile(const char* filename, const char* texture_fname = nullptr);
	void parseFaceData(std::string(&face_data)[4], int vCount);

};

