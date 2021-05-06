#pragma once
#include<vector>
#include "Vector3.h"


struct Vertex { //Note Vec3f is actually 4 floats due to hidden w component
	Vec3f position;
	Vec3f normal;
	Vec3f tangent;
	Vec2f texcoord;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	std::vector<Vertex>& getVBuffer();
	std::vector<int>& getIdxBuffer();
	
private:
	
	friend class OBJLoader;
	std::vector<Vertex> m_vertex_buffer;
	std::vector<int> m_index_buffer;

};

