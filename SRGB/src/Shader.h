#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include "Model.h"

#include <algorithm>

class IShader {
public:

	virtual ~IShader() {};
	virtual Vec3f vertex(const Model &model, const Vec3f &light_dir, int face_idx, int nth_vert) = 0;
	virtual Vec3f fragment(const Vec3f &bary) = 0;

};


class FlatShader : public IShader {
public:

	//Per Model
	Mat4f MVP, itM; // Matrices
	Texture *texture;

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	//Written by vertex shader
	Vec3f varying_intensity;
	Vec2f uv_values[3];

	Vec3f vertex(const Model &model, const Vec3f &light_dir, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		vertex = model.getVertex(verts_idx[nth_vert]);
		
		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

		//Compute light intensity for face

		//Calculate face normal 
		Vec3f face_normal = model.getFaceNormal(face_idx);

		//Transform face normal
		Vec3f trans_normal = itM * face_normal;
		trans_normal.normalize();

		varying_intensity[nth_vert] = std::max(0.0f, trans_normal.dot(light_dir));

		return MVP*vertex;


	}

	Vec3f fragment(const Vec3f &bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);

		Vec3f rgb;
		if (texture != nullptr)//If model has texture
		{
			rgb = texture->getTexel(u, v);
		}
		else
		{
			rgb = Vec3f(255, 255, 255);
		}

		for (int i = 0; i < 3; i++)
		{
			rgb[i] *= varying_intensity[i];
		}

		return rgb;
	}

};





//Gourad shader

class GouradShader : public IShader {
public:

	//Per Model
	Mat4f MVP, itM; // Matrices
	Texture *texture;

	//Per triangle
	Vec3i verts_idx, uv_idx; // Triangle indexes

	//Written by vertex shader
	Vec3f varying_intensity;
	Vec2f uv_values[3];

	Vec3f vertex(const Model &model, const Vec3f &light_dir, int face_idx, int nth_vert) override
	{
		//Load vertex
		Vec3f vertex;
		vertex = model.getVertex(verts_idx[nth_vert]);

		//If texture, load uv values
		if (texture != nullptr)
		{
			uv_values[nth_vert] = model.getUV(uv_idx[nth_vert]);
		}

		//Compute light intensity for face

		//Calculate face normal 
		Vec3f vertex_normal = model.getVertexNormal(face_idx, nth_vert);

		//Transform face normal
		Vec3f trans_normal = itM * vertex_normal;
		trans_normal.normalize();

		varying_intensity[nth_vert] = std::max(0.0f, trans_normal.dot(light_dir));

		return MVP * vertex;


	}

	Vec3f fragment(const Vec3f &bary) override
	{
		//Calculate texture uv values
		Vec3f u_vals{ uv_values[0].u, uv_values[1].u, uv_values[2].u };
		Vec3f v_vals{ uv_values[0].v, uv_values[1].v, uv_values[2].v };

		float u = bary.dot(u_vals);
		float v = bary.dot(v_vals);

		Vec3f rgb;
		if (texture != nullptr)//If model has texture
		{
			rgb = texture->getTexel(u, v);
		}
		else
		{
			rgb = Vec3f(255, 255, 255);
		}

		
		//Interpolate intensityies between vertices
		rgb = rgb*bary.dot(varying_intensity);
		

		return rgb;
	}

};