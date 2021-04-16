#include "Renderer.h"
#include "omp.h"

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
	
	//Get render queue of models in scene. 
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




void Renderer::renderModel(const Model *model, const SceneLights* lights)
{

	
	//Load shader matrices
	Mat4f MVmat = (m_camera->getViewMat()) * (model->getModelMat()); 
	Mat4f MVPmat = m_camera->getProjectionMat() * MVmat;
	Mat4f Vmat = m_camera->getViewMat();
	Mat4f Nmat = Vmat.normalMatrix();
	
	
	//FlatShader shader(MVmat, MVPmat, Vmat, Nmat, lights);
	
	PhongShader shader(MVmat, MVPmat, Vmat, Nmat, lights, model->getMaterial());
	
	//GouradShader shader(MVmat, MVPmat, Vmat, Nmat, lights, model->getMaterial());


	//For backface culling
	Mat4f invModel = model->getModelMat().inverse();


	//Iterate each face
	//Parallelize loop. shader is private to each thread and initialized as the original shader. 
	//Schedule dynamic since many threads will finish early due to early rejection due to front end backface culling and clipping
	
	
	#pragma omp parallel for firstprivate(shader) schedule(dynamic)
	for (int i = 0; i < model->getFaceCount(); i++) //i = faceid
	{ 
		//DEBUG ONLY DRAW TRIANGLE 1 FOR NOW
		if (i == 2 || i == 3)
		{

			/*if(i==2){
				printf("tri1");
			}
			else {
				printf("tri2");
			}*/
			
			//vertex shader per face members
			shader.verts_idx = model->getFaceVertices(i);
			shader.uv_idx = model->getUVidx(i);
			shader.texture = model->getTexture();
		


			//Front-end perspective correct backface culling
			Vec3f faceNormal = model->getFaceNormal(i);
			Vec3f viewVec = model->getVertex(shader.verts_idx[0]) - (invModel * m_camera->m_pos); //A BIT DODGY TO USE SHADER MEMBER FOR THIS. MAYBE REFACTOR VERTEX SHADER ARGUMENTS
			viewVec.normalize();
			float normLimit = cos(((90.0f - m_camera->m_fov/2.0f) + 90.0f) * (M_PI / 180.0f));
			float bfc_intentsity1 = faceNormal.dot(-viewVec);
			if (bfc_intentsity1 <= normLimit) continue;
		

			//Vertex shader
			Vec3f face_verts[3];


			for (int j = 0; j < 3; j++)
			{
				face_verts[j] = shader.vertex(*model, i, j);

			}


			//Clip triangles (doenst reconstruct partly out ones)
			bool isClipped = false;
			for (int j = 0; j < 3; j++)
			{
				// CHECK THIS IDK IF Z IS FROM 0 1 or -1 0 etc etc
				//If x and y are within -w and w (-1 and 1 pre perspective divide) and z is within 0 and w (0 and 1 pre perspec div)
				bool in_bounds = (( -face_verts[j].w <= face_verts[j].x && face_verts[j].x <= face_verts[j].w) &&
									(-face_verts[j].w <= face_verts[j].y && face_verts[j].y <= face_verts[j].w) &&
													( 0 <= face_verts[j].z && face_verts[j].z <= face_verts[j].w));

				if(!in_bounds){
					isClipped = true;
				}
			
			}
			//if(isClipped) continue;//Next triangle CLIPPING DISABLED FOR NOW


			//Perform perspetive divide on vertices
			for (int j = 0; j < 3; j++)
			{
				//Transform vertices
				face_verts[j].perspecDiv();
			}

			
			Rasterizer::drawTriangle(face_verts, shader, m_px_buff, m_z_buff);
		

		

			//DEBUG FUNCTION DRAW NORMAL
#if 1
			//Vec3i vid = model->getFaceVertices(i);
			//Vec3f vertices = model->getVertex(vid.x);
			Vec3f shiftedNormal = Nmat * faceNormal;;//MVmat * vertices;//

			//if (model->getDrawNormal()) {
				Rasterizer::drawNormal(face_verts, &shiftedNormal, m_px_buff, m_z_buff);
			//}
#endif
		}; //when face id = 1 breakpoint
	}
}