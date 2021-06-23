#pragma once

#include "Mesh.h"
#include "Model.h"
#include "Vector3.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_map>

struct V_Data //Making this static might not work for a header file 
{
	std::vector<Vec3f> v_array;
	std::vector<Vec3f> vn_array;
	std::vector<Vec2f> vt_array;
	std::unordered_map<std::string, int> index_map;
};

class OBJLoader {
public:
	static constexpr int MAX_POLY = 20;

	static Mesh* loadMesh(const char* filepath) //Returns a pointer to a dynamically allocated mesh. 
	{
		//Arrays to store vertex normals and texture from obj file. v, vn and vt
		V_Data vData;
		Mesh* new_mesh = new Mesh();

		//Open file
		std::ifstream ifs;
		ifs.open(filepath, std::ifstream::in);
		if (ifs.fail()) {
			assert("OBJ filepath invalid");
			exit;
		};
	
		while (!ifs.eof())
		{
			//Set up stringstream
			std::string nextline,cmd,temp;
			std::getline(ifs, nextline);
			std::stringstream iss(nextline);

			//Read command from obj file
			iss>>cmd;

			if (cmd == "v") // vertex data 
			{
				Vec3f v;
				for (int i = 0; i < 3; i++) 
				{
					iss>>v[i];
				}
				vData.v_array.push_back(v);
			}
			else if (cmd == "vt") // texture data
			{
				Vec2f vt;
				for (int i = 0; i < 2; i++)
				{
					iss >> vt[i];
				}
				vData.vt_array.push_back(vt);
			}
			else if (cmd == "vn") // vnormal data
			{
				Vec3f vn;
				for (int i = 0; i < 3; i++)
				{
					iss >> vn[i];
				}
				vData.vn_array.push_back(vn);
			}
			else if (cmd == "f") // face command
			{
				int count = 0;
				
				std::string face_data[MAX_POLY];
				//Count number of sides in face
				while (iss >> temp) 
				{
					face_data[count] = temp;
					count += 1;
				}

				//Assert that count is not
				if (!(count > 2)) {
					assert("Face with only two vertices");
					return nullptr;//ERROR
				}

				parseFaceData(face_data, count, vData, new_mesh);

			}
		}

		calculateVertexTangents(new_mesh);

		return new_mesh;
	};

	

private:
	OBJLoader();

	static void parseFaceData(std::string(&face_data)[MAX_POLY], int v_count, V_Data &vData, Mesh* new_mesh)
	{
		std::string delimiter = "/";
		size_t pos = 0;
		std::vector<int> face_indeces;

		for (int i = 0; i < v_count; i++) 
		{
			//Extract vertex indices from face command
			std::vector<std::string > vertex_indices;
			while ((pos = face_data[i].find(delimiter)) != std::string::npos)
			{
				vertex_indices.push_back(face_data[i].substr(0, pos));

				face_data[i].erase(0, pos + delimiter.length());
			}
			vertex_indices.push_back(face_data[i].substr(0, pos));

			
			//Build index key
			std::stringstream kss;
			kss << vertex_indices[0] << "/" << vertex_indices[1] << "/" << vertex_indices[2];
			std::string idx_key = kss.str();

			//Check if vertex already exists 
			if (vData.index_map.find(idx_key) == vData.index_map.end()) {
				//vertex doesnt exists: store key(v/vn/vt) value(vertex index 0,1,2...) 
				//and build and add vertex to mesh vertex buffer.
				int idx = vData.index_map.size();
				vData.index_map.insert({idx_key, idx});

				//Load new vertex
				Vertex vertex;

				//Vertex coordinates
				int v = std::stoi(vertex_indices[0]);
				vertex.position = vData.v_array[--v];//OBJ indices start at 1 -- to fix

				//Vertex texture
				int vt = std::stoi(vertex_indices[1]);
				vertex.texcoord = vData.vt_array[--vt];

				//Vertex normals
				int vn = std::stoi(vertex_indices[2]);
				vertex.normal = vData.vn_array[--vn];

				//Add vertex to mesh
				new_mesh->m_vertex_buffer.push_back(vertex);

				//Add vertex index to face indeces list
				face_indeces.push_back(idx);
			}
			else {
				//Add vertex index to face indeces list
				face_indeces.push_back(vData.index_map.at(idx_key));
			}
				
		}


		//Add triangle indeces to mesh index buffer. 
		//Account for quads+ and turn them to triangle fan
		for (int i = 1; i <= v_count - 2; i++)//If trinagle for loop runs once
		{
			std::vector<Vec3i> cur_face;

			//add first vertex 
			new_mesh->m_index_buffer.push_back(face_indeces[0]);

			//add second vertex 
			new_mesh->m_index_buffer.push_back(face_indeces[i]);

			//add third vertex 
			new_mesh->m_index_buffer.push_back(face_indeces[i+1]);

			//Build tangents for that face
		}
	}

	static void calculateVertexTangents(Mesh* mesh) 
	{
		std::vector<Vec3f> vert_tangents;
		std::vector<Vec3f> vert_bitangents;
		std::vector<int> tangent_count; //used for averaging
		vert_tangents.resize(mesh->m_vertex_buffer.size());
		vert_bitangents.resize(mesh->m_vertex_buffer.size());
		tangent_count.resize(mesh->m_vertex_buffer.size());

		for (int i = 0; i < mesh->m_index_buffer.size(); i+=3) 
		{
			//Calculate tangents for each triangle
			Vec3f vert0 = mesh->m_vertex_buffer[mesh->m_index_buffer[i+0]].position;
			Vec3f vert1 = mesh->m_vertex_buffer[mesh->m_index_buffer[i+1]].position;
			Vec3f vert2 = mesh->m_vertex_buffer[mesh->m_index_buffer[i+2]].position;

			Vec2f texcoord0 = mesh->m_vertex_buffer[mesh->m_index_buffer[i+0]].texcoord;
			Vec2f texcoord1 = mesh->m_vertex_buffer[mesh->m_index_buffer[i+1]].texcoord;
			Vec2f texcoord2 = mesh->m_vertex_buffer[mesh->m_index_buffer[i+2]].texcoord;

			Vec3f edge1 = vert1 - vert0;
			Vec3f edge2 = vert2 - vert0;

			Vec2f deltaUV1 = texcoord1 - texcoord0;
			Vec2f deltaUV2 = texcoord2 - texcoord0;

			//TODO tenagent space tangenst etc
			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x); //1 over determinant

			Vec3f tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;
			Vec3f bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * r;

			//Add tangent and bitangents to array at the indexed slot
			vert_tangents[mesh->m_index_buffer[i + 0]] += tangent;
			vert_tangents[mesh->m_index_buffer[i + 1]] += tangent;
			vert_tangents[mesh->m_index_buffer[i + 2]] += tangent;

			vert_bitangents[mesh->m_index_buffer[i + 0]] += bitangent;
			vert_bitangents[mesh->m_index_buffer[i + 1]] += bitangent;
			vert_bitangents[mesh->m_index_buffer[i + 2]] += bitangent;

			tangent_count[mesh->m_index_buffer[i + 0]] += 1;
			tangent_count[mesh->m_index_buffer[i + 1]] += 1;
			tangent_count[mesh->m_index_buffer[i + 2]] += 1;
		}


		for (int i = 0; i < mesh->m_vertex_buffer.size(); i++) //TANGENT AND BITANGENT AVERAGING
		{
			vert_tangents[i] = vert_tangents[i] * (1.0f / tangent_count[i]);
			vert_bitangents[i] = vert_bitangents[i] * (1.0f / tangent_count[i]);
		}

		for (int i = 0; i < mesh->m_vertex_buffer.size(); i++) 
		{
			//Gram-Schmidt orthogonalisation
			Vec3f n = mesh->m_vertex_buffer[i].normal;
			Vec3f t0 = vert_tangents[i];
			Vec3f b = vert_bitangents[i];

			Vec3f t = t0 - (n * n.dot(t0));
			t.normalize();

			//Correct handedness
			Vec3f c = n.cross(t);
			if ( b.dot(c) < 0.0f) {
				t = t * -1.0f;
			}

			//Add tangent to vertex
			mesh->m_vertex_buffer[i].tangent = t;
		}
	}
};