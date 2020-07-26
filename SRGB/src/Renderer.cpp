#include "Renderer.h"


//Constructor
Renderer::Renderer(int buff_width, int buff_height) : m_px_buff(nullptr), m_z_buff(nullptr)
{
	m_px_buff = new Buffer<uint32_t>(buff_width, buff_height);
	m_z_buff = new Buffer<float>(buff_width, buff_height);

}

//Destructor
Renderer::~Renderer()
{
	delete m_px_buff;
	delete m_z_buff;
}


Buffer<uint32_t>* Renderer::getRenderTarget() const
{
	return m_px_buff;
}
Buffer<float>* Renderer::getDepthBuffer() const
{
	return m_z_buff;
}


void Renderer::renderWireFrame(const Model *model, uint32_t colour)
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



void Renderer::renderScene(Scene* scene)//WILL NEED TO GET LIGHTS FROM SCENE ETC ETC. FOR NOW JUST OBJECT
{
	//Get render queue of models in scene. (Already in world object coordinates(aka post model matrix))
	std::queue<Model*> *render_queue = scene->createRenderQueue();

	//Render object in queue
	while (!render_queue->empty())
	{
		renderModel(render_queue->front());
		render_queue->pop();
	}

}


void Renderer::setRenderCam(Camera *cam)
{
	m_camera = cam;
}


void Renderer::renderModel(const Model *model)
{
	//Object coordinates to clip coordinates must be done here. viewport transform done in rasterizer(although that is done by the vertex shader techinically) FOR NOW USES ASUMES MODEL HAS NDC COORDS 

	//DIRECTIONAL LIGHT:  THIS WILL LATER BE STORED SOMETHWERER ELSE SUCH AS IN SCENE WITH MODELS ETC. PROBABLY FED AS ARGUEMENT AS PART OF SHADER OR SOMETHING TO RASTERIZER FUNCTION
	Vec3f light_dir(0, 0, -1);
	light_dir.normalize();

	for (int i = 0; i < model->getFaceCount(); i++)
	{
		
		

		//This could be a function like loadIndexData(). WILL BE NEEDED WHEN nEEDING TO LOAD NORMAL DATA, TEXEL DATA ETC FROM MODEL
		Vec3i face_verts_idx = model->getFaceVertices(i);
		Vec3f face_verts[3];
		for (int j = 0; j < 3; j++)
		{
			face_verts[j] = model->getVertex(face_verts_idx[j]);

			
		}

		//VERTEX SHADER STUFF HERE
		Mat4f modelViewProj_mat;
		modelViewProj_mat = (m_camera->getProjectionMat()) * (m_camera->getViewMat()) * (model->getModelMat());
		for (int j = 0; j < 3; j++)
		{
			//Transform vertices
			face_verts[j] = modelViewProj_mat * face_verts[j];
		}

		//Clip triangles (doenst reconstruct partly out ones)
		bool isClipped = false;
		for (int j = 0; j < 3; j++)
		{
			// CHECK THIS IDK IF Z IS FROM 0 1 or -1 0 etc etc
			//If x and y are within -w and w (-1 and 1 pre perspective divide) and z is within 0 and w (0 and 1 pre perspec div)
			bool in_bounds = (( -face_verts[j].w <= face_verts[j].x <= face_verts[j].w)&&(-face_verts[j].w <= face_verts[j].y <= face_verts[j].w)&&( 0 <= face_verts[j].z <= face_verts[j].w));

			if(!in_bounds){
				isClipped = true;
			}
			
		}
		if(isClipped) continue;//Next triangle

		//Perform perspetive divide on vertices
		for (int j = 0; j < 3; j++)
		{
			//Transform vertices
			face_verts[j].perspecDiv();
		}

		Rasterizer::simpleRasterizeTri(face_verts, m_px_buff, m_z_buff,light_dir);

	}

}