#include "Renderer.h"
#include "omp.h"

//Constructor
Renderer::Renderer(int buff_width, int buff_height) : m_px_buff(nullptr), m_z_buff(nullptr)
{
	m_px_buff = new Buffer<uint32_t>(buff_width, buff_height);
	m_z_buff = new Buffer<float>(buff_width, buff_height);

	std::unique_ptr<IShader> shader = std::unique_ptr<PhongShader>(new PhongShader());
	loadShader(shader);
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

	
	//Load shader uniforms
	Mat4f MVmat = (m_camera->getViewMat()) * (model->getModelMat()); 
	Mat4f MVPmat = m_camera->getProjectionMat() * MVmat;
	Mat4f Vmat = m_camera->getViewMat();
	Mat4f Nmat = MVmat.normalMatrix();  
	
	/*
	Mat4f::printMatToConsole(model->getModelMat());
	Mat4f::printMatToConsole(Vmat);
	Mat4f::printMatToConsole(MVmat);
	Mat4f::printMatToConsole(Nmat);
	printf("newframe\n");
	*/
	setShaderUniforms(MVmat, MVPmat, Vmat, Nmat, lights,  model);

	//FlatShader shader(MVmat, MVPmat, Vmat, Nmat, lights);
	
	//PhongShader shader(MVmat, MVPmat, Vmat, Nmat, lights, model->getMaterial());
	
	//GouradShader shader(MVmat, MVPmat, Vmat, Nmat, lights, model->getMaterial());


	//For backface culling
	Mat4f invModel = model->getModelMat().inverse();


	//Iterate each face
	//Parallelize loop. shader is private to each thread and initialized as the original shader. 
	//Schedule dynamic since many threads will finish early due to early rejection due to front end backface culling and clipping
	//CURRENTLY NOT WORKING: would need to make shader a variable created in this scope not a member of Renderer. But that makes it difficult to swap shaders on the fly since not a pointer
	
	
	//#pragma omp parallel for firstprivate(m_shader) schedule(dynamic)
	for (int i = 0; i < model->getFaceCount(); i++) //i = faceid
	{ 
		//DEBUG ONLY DRAW TRIANGLE 1 FOR NOW
		//if (i == 2 || i == 3)
		{

			/*if(i==2){
				printf("tri1");
			}
			else {
				printf("tri2");
			}*/
			
			//vertex shader per face members
			//m_shader.verts_idx = model->getFaceVertices(i); //THIS SHOULD/COULD BE AN INPUT TO THE VERTEX SHADER
			//m_shader.uv_idx = model->getUVidx(i);

			//m_shader.texture = model->getTexture();
		


			//Front-end perspective correct backface culling
			Vec3f faceNormal = model->getFaceNormal(i);
			Vec3f viewVec = model->getVertex(model->getFaceVertices(i).x) - (invModel * m_camera->m_pos); //A BIT DODGY TO USE SHADER MEMBER FOR THIS. MAYBE REFACTOR VERTEX SHADER ARGUMENTS  m_shader.verts_idx[0]
			viewVec.normalize();
			float normLimit = cos(((90.0f - m_camera->m_fov/2.0f) + 90.0f) * (M_PI / 180.0f));
			float bfc_intentsity1 = faceNormal.dot(-viewVec);
			if (bfc_intentsity1 <= normLimit) continue;
			

			//Vertex shader
			Vec3f face_verts[3];


			for (int j = 0; j < 3; j++)
			{
				face_verts[j] = m_shader->vertex(*model, i, j);

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
			if(isClipped) continue;//Next triangle CLIPPING DISABLED FOR NOW


			//Perform perspetive divide on vertices
			for (int j = 0; j < 3; j++)
			{
				//Transform vertices
				face_verts[j].perspecDiv();
			}

		
			Rasterizer::drawTriangle(face_verts, m_shader.get(), m_px_buff, m_z_buff);
		

		
			
			//DEBUG FUNCTION DRAW NORMAL
#if 1
			//Vec3i vid = model->getFaceVertices(i);
			//Vec3f vertices = model->getVertex(vid.x);
			//Vec3f shiftedNormal = Nmat * faceNormal;;//MVmat * vertices;//

			//if (model->getDrawNormal()) {
				//Rasterizer::drawNormal(face_verts, &shiftedNormal, m_px_buff, m_z_buff);//MUST NOT BE IN POST MVP
			//}
#endif
		}; //when face id = 1 breakpoint
	}
}


void Renderer::setShaderUniforms(const Mat4f MV, const Mat4f MVP, const Mat4f V, const Mat4f N, const SceneLights* sceneLights, const Model* model) {

	assert(m_shader != nullptr);

	auto lights = sceneLights->dirLights;
	const Material* temp_mat = model->getMaterial();
	switch (m_shader->getType()) {
		
	/*case ShaderType::DEBUG:
	{
		DebugShader* temp = dynamic_cast<DebugShader*>(m_shader.get());
		temp->MVmat = MV;
		temp->MVPmat = MVP;
		temp->Vmat = V;
		temp->Nmat = N;
		temp->Mmat = M;
		temp->Ia = temp_mat->m_Ia;
		temp->Il = temp_mat->m_Il;
		temp->ka = temp_mat->m_ka;
		temp->ks = temp_mat->m_ks;
		temp->kd = temp_mat->m_kd;
		temp->spec_n = temp_mat->m_spec_n;
		temp->texture = model->getTexture();
		temp->camera_pos = m_camera->m_pos;

		//Calculate and set light direction
		if (temp->light_dir.size() < lights.size()) {
			temp->light_dir.resize(lights.size());
			temp->light_colour.resize(lights.size());
		}
		for (int i = 0; i < lights.size(); i++)
		{

			temp->light_dir[i] = (lights[i].m_direction); //
			temp->light_dir[i].normalize();
			temp->light_colour[i] = (lights[i].m_colour);
		}
	}
	break;*/
		case ShaderType::PHONG:
		{
			PhongShader *temp = dynamic_cast<PhongShader*>(m_shader.get());
			temp->MVmat = MV;
			temp->MVPmat = MVP;
			temp->Vmat = V;
			temp->Nmat = N;
			temp->Ia = temp_mat->m_Ia;
			temp->Il = temp_mat->m_Il;
			temp->ka = temp_mat->m_ka;
			temp->ks = temp_mat->m_ks;
			temp->kd = temp_mat->m_kd;
			temp->spec_n = temp_mat->m_spec_n;
			temp->texture = model->getTexture();

			//Calculate and set light direction
			if (temp->light_dir.size() < lights.size()) {
				temp->light_dir.resize(lights.size());
				temp->light_colour.resize(lights.size());
			}
			for (int i = 0; i < lights.size(); i++)
			{
				
				temp->light_dir[i] = (temp->Vmat.convertDirToViewSpace(lights[i].m_direction)); //
				temp->light_dir[i].normalize();
				temp->light_colour[i]=(lights[i].m_colour);
			}
		}
		break;

		case ShaderType::GOURAD:
		{
			GouradShader* temp = dynamic_cast<GouradShader*>(m_shader.get());
			temp->MVmat = MV;
			temp->MVPmat = MVP;
			temp->Vmat = V;
			temp->Nmat = N;
			temp->Ia = temp_mat->m_Ia;
			temp->Il = temp_mat->m_Il;
			temp->ka = temp_mat->m_ka;
			temp->ks = temp_mat->m_ks;
			temp->kd = temp_mat->m_kd;
			temp->spec_n = temp_mat->m_spec_n;
			temp->texture = model->getTexture();

			//Calculate and set light direction
			if (temp->light_dir.size() < lights.size()) {
				temp->light_dir.resize(lights.size());
				temp->light_colour.resize(lights.size());
			}
			for (int i = 0; i < lights.size(); i++)
			{

				temp->light_dir[i] = (temp->Vmat.convertDirToViewSpace(lights[i].m_direction)); //
				temp->light_dir[i].normalize();
				temp->light_colour[i] = (lights[i].m_colour);
			}
		}
		break;

		case ShaderType::FLAT:
		{
			FlatShader* temp = dynamic_cast<FlatShader*>(m_shader.get());
			temp->MVmat = MV;
			temp->MVPmat = MVP;
			temp->Vmat = V;
			temp->Nmat = N;
			temp->Ia = temp_mat->m_Ia;
			temp->Il = temp_mat->m_Il;
			temp->ka = temp_mat->m_ka;
			//temp->ks = temp_mat->m_ks;
			temp->kd = temp_mat->m_kd;
			//temp->spec_n = temp_mat->m_spec_n;
			temp->texture = model->getTexture();

			//Calculate and set light direction
			if (temp->light_dir.size() < lights.size()) {
				temp->light_dir.resize(lights.size());
				temp->light_colour.resize(lights.size());
			}
			for (int i = 0; i < lights.size(); i++)
			{

				temp->light_dir[i] = (temp->Vmat.convertDirToViewSpace(lights[i].m_direction)); //
				temp->light_dir[i].normalize();
				temp->light_colour[i] = (lights[i].m_colour);
			}
		}
		break;

		default:
			assert("Setting uniforms failed.");
		break;
	}




}