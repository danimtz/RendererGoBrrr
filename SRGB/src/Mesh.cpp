#include "Mesh.h"

Mesh::Mesh()
{


}

Mesh::~Mesh()
{


}


std::vector<Vertex>& Mesh::getVBuffer(){
	return m_vertex_buffer;
}



std::vector<int>& Mesh::getIdxBuffer() {
	return m_index_buffer;
}