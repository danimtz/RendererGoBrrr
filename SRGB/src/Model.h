#pragma once

#include<vector>
#include "Vector3.h"

class Model {


public:

	//Constructor that will call OBJ loader function to fill model data
	Model(const char* filename);
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
	
	//Obtained from face command in OBJ file
	std::vector<Vec3f> m_vertex;
	std::vector<Vec3f> m_vnorms;
	std::vector<Vec2f> m_uv;
	// face normals member variable??

	//Faces defined by face command. 
	//Vec3i of vertex/uv/vnormal points to the respective index in m_vertex/m_uv/m_norms
	std::vector<std::vector<Vec3i>> m_faces;


	//OBJ file parsing funcitons //THIS FUNCTION COULD MAYBE BE DECOUPLED FROM THE MODEL CLASS
	void loadOBJfile(const char* filename);
	void parseFaceData(std::string(&face_data)[4], int vCount);

};

