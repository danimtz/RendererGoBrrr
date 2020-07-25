#include "Model.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<array>


Model::Model(const char* filename, const Mat4f transform) : m_vertex(), m_vnorms(), m_uv(), m_faces(), m_model_mat(transform)
{
	
	loadOBJfile(filename);

	
}


Model::Model(const char* filename, const char* texture_fname, const Mat4f transform) : m_vertex(), m_vnorms(), m_uv(), m_faces(), m_model_mat(transform)
{
	loadOBJfile(filename, texture_fname);
	
}


Model::~Model()
{
	delete m_texture;
}




int Model::getFaceCount() const
{
	return (int)m_faces.size();
}

int Model::getVertCount() const
{
	return (int)m_vertex.size();
}

//Get nth vertex
Vec3f Model::getVertex(int n) const
{
	return m_vertex[n];
}

//Get the nth vertex of a face
Vec3f Model::getVertex(int nface, int nth_vert) const
{
	return m_vertex[  m_faces[nface][nth_vert][0]  ];
}

//Returns a vector of the 3 vertex indexes of the given face
Vec3i Model::getFaceVertices(int nface) const
{

	Vec3i face_vertices;

	for (int i = 0; i<3; i++)
	{
		int vindex = m_faces[nface][i][0];
		face_vertices[i] = vindex;
	}

	return face_vertices;

}





//Loads OBJ file. currently doenst accept obj files with faces larger than quads
void Model::loadOBJfile(const char* filename, const char* texture_fname)
{
	std::ifstream ifs;
	ifs.open(filename, std::ifstream::in);

	if (ifs.fail()) return;//ERROR

	

	int debug_Line = 0;
	while (!ifs.eof())
	{
		std::string newline,cmd,temp;
		std::getline(ifs, newline);
		std::istringstream iss(newline);
		
		iss>>cmd;

		if (cmd == "v") // vertex data
		{
			Vec3f v;
			for (int i = 0; i < 3; i++)
			{
				iss>>v[i];
			}
			m_vertex.push_back(v);
		}
		else if (cmd == "vt") // texture data
		{
			Vec2f vt;
			for (int i = 0; i < 2; i++)
			{
				iss >> vt[i];
			}
			m_uv.push_back(vt);
		}
		else if (cmd == "vn") // vnormal data
		{
			Vec3f vn;
			for (int i = 0; i < 3; i++)
			{
				iss >> vn[i];
			}
			m_vnorms.push_back(vn);
		}
		else if (cmd == "f") // face command
		{
			
			//Parse and count vertices in face command
			int count = 0;
			std::string face_data[4];
			while(iss >> temp)
			{
				face_data[count] = temp;
				count +=1;
			};

			//Assert that face is a triangle or quad
			//assert(count > 2 && count <=4);
			if (!(count > 2 && count <= 4)){
				__debugbreak();
				return;//ERROR
			}
			parseFaceData(face_data, count);
		}

		debug_Line +=1;
		
	}
	

	//Load texture THIS COULD BE DONE IN MEMEBR INITIALIZER LIST
	
	m_texture = new Texture(texture_fname);


}


void Model::parseFaceData(std::string (&face_data)[4], int vCount)
{
	
	std::string delimiter = "/";
	
	std::vector<Vec3i> fvertex;
	size_t pos = 0;
	for (int i = 0; i < vCount; i++)
	{
		std::vector<std::string > tmp;
		//std::cout << "new vertex:" << std::endl;
		while ((pos = face_data[i].find(delimiter)) != std::string::npos)
		{
			tmp.push_back(face_data[i].substr(0, pos));
			
			face_data[i].erase(0, pos + delimiter.length());
		}
		tmp.push_back(face_data[i].substr(0, pos));
		
		Vec3i vindex;
		for (int k = 0; k < tmp.size(); ++k)//k used to be size_t
		{
			if (tmp[k] == "")
			{
				vindex[k] = -1;
			}else{
				 int n = std::stoi(tmp[k]);
				 vindex[k] = --n; //OBJ file indexes start at 1 so -- to fix
			}
		}
		fvertex.push_back(vindex);

	}
	
	//If vCount == 4 then convert quad to two trinagles. vCount can only be 3 or more
 	for (int i = 1; i <= vCount - 2; i++)
	{
		std::vector<Vec3i> cur_face;

		//add first vertex
		cur_face.push_back(fvertex[0]);

		//add second vertex
		cur_face.push_back(fvertex[i]);

		//add third vertex
		cur_face.push_back(fvertex[i+1]);

		//push face to memeber vector of faces
		m_faces.push_back(cur_face);
	}
	
	
	


}