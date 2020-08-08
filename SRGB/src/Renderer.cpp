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
	//Clear buffers
	m_z_buff->clear();
	m_px_buff->clear();
	
	//Get render queue of models in scene. (Already in world object coordinates(aka post model matrix))
	std::queue<Model*> *render_queue = scene->createRenderQueue();

	//Render object in queue
	while (!render_queue->empty())
	{
		renderModel(render_queue->front(), scene->getLights());
		render_queue->pop();
	}

}


void Renderer::setRenderCam(Camera *cam)
{
	m_camera = cam;
}




void Renderer::renderModel(const Model *model, const std::vector<Light*>& lights)
{
	//Object coordinates to clip coordinates must be done here. viewport transform done in rasterizer(although that is done by the vertex shader techinically) FOR NOW USES ASUMES MODEL HAS NDC COORDS 

	//DIRECTIONAL LIGHT:  THIS WILL LATER BE STORED SOMETHWERER ELSE SUCH AS IN SCENE WITH MODELS ETC. PROBABLY FED AS ARGUEMENT AS PART OF SHADER OR SOMETHING TO RASTERIZER FUNCTION done
	Vec3f light_dir = lights[0]->m_target - lights[0]->m_pos;
	light_dir.normalize();

	GouradShader shader;
	
	//Load shader members
	shader.MV = (m_camera->getViewMat()) * (model->getModelMat()); 
	shader.MVP = (m_camera->getProjectionMat()) * shader.MV;
	//shader.N = model->getModelMat();
	//shader.N = shader.N.inverse().transpose();


	//Iterate each face
	for (int i = 0; i < model->getFaceCount(); i++)
	{
		
		

		//vertex shader per face members
		shader.verts_idx = model->getFaceVertices(i);
		shader.uv_idx = model->getUVidx(i);
		shader.texture = model->getTexture();
	



		Vec3f face_verts[3];
		for (int j = 0; j < 3; j++)
		{
			face_verts[j] = shader.vertex(*model, light_dir, i, j);

		}


		//Clip triangles (doenst reconstruct partly out ones)
		bool isClipped = false;
		for (int j = 0; j < 3; j++)
		{
			// CHECK THIS IDK IF Z IS FROM 0 1 or -1 0 etc etc
			//If x and y are within -w and w (-1 and 1 pre perspective divide) and z is within 0 and w (0 and 1 pre perspec div)
			bool in_bounds = (( -face_verts[j].w <= face_verts[j].x <= face_verts[j].w) && 
								(-face_verts[j].w <= face_verts[j].y <= face_verts[j].w) && 
												( 0 <= face_verts[j].z <= face_verts[j].w));

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


		//Rasterizer::simpleRasterizeTri(face_verts, shader, m_px_buff, m_z_buff);
		Rasterizer::drawTriangle(face_verts, shader, m_px_buff, m_z_buff);

	}

}