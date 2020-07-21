#include "Renderer.h"


//Constructor
Renderer::Renderer(int buff_width, int buff_height) : m_px_buff(nullptr)
{
	m_px_buff = new Buffer<uint32_t>(buff_width, buff_height);
}

//Destructor
Renderer::~Renderer()
{
	delete m_px_buff;
}


Buffer<uint32_t>* Renderer::getRenderTarget() 
{
	return m_px_buff;
}


void Renderer::renderWireFrame(Model *model, uint32_t colour)
{
	//Currently assumes that OBJ file is in NDC of -1 to 1
	Mat4f viewPrt_transform = Mat4f::createViewportTransform(m_px_buff->m_width, m_px_buff->m_height);

	//Iterate faces
	for (int i = 0; i < model->getFaceCount(); i++)
	{

		Vec3i face_verts_idx = model->getFaceVertices(i);

		Vec3f face_verts[3];

		for (int j = 0; j < 3; j++)
		{
			Vec3f v = model->getVertex(face_verts_idx[j]);

			face_verts[j] = viewPrt_transform * v;//MAYBE THE VIEWPORT TRANSFORM SHOULD BE IN RASTERIZER FOR PERSPECTIVE CAMERA VIEW CALCULATIONS

		}

		Rasterizer::drawWireFrame(face_verts, m_px_buff, colour);
	}
}






void Renderer::renderModel(Model *model)//THIS WILL BE HEAVILY OVERHAULED LATER TO USE RENDER QUEUE ETC ETC ETC
{
	//Object coordinates to clip coordinates must be done here. viewport transform done in rasterizer FOR NOW USES ASUMES MODEL HAS NDC COORDS 


	for (int i = 0; i < model->getFaceCount(); i++)
	{

		//This could be a function like loadIndexData(). WILL BE NEEDED WHEN nEEDING TO LOAD NORMAL DATA, TEXEL DATA ETC FROM MODEL
		Vec3i face_verts_idx = model->getFaceVertices(i);
		Vec3f face_verts[3];
		for (int j = 0; j < 3; j++)
		{
			face_verts[j] = model->getVertex(face_verts_idx[j]);
		}


		Rasterizer::simpleRasterizeTri(face_verts, m_px_buff);

	}

}