#pragma once

#include<vector>
#include "Vector3.h"

class Model {


public:

	//Constructor that will call OBJ parser to fill
	Model();
	~Model();

	//Functions to get normals, vertices and uv given the face number and vertex etc etc

private:

	//Obtained from face command in OBJ file
	std::vector<Vec3f> m_vertex;
	std::vector<Vec3f> m_vnorms;
	std::vector<Vec2f> m_uv;
	//std::vector<Vec3f> m_fnorms; MIGHT NOT BE NEEDED FOR OBJ FILES SINCE WE GET VERTEX NORMALS

	//Faces defined by face command. Vec3i of  vertex/uv/vnormal points to the respective index in m_vertex/m_uv/m_norms
	std::vector<std::vector<Vec3i>> m_faces;

};

