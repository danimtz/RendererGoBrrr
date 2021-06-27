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
	
	setShaderUniforms(MVmat, MVPmat, Vmat, Nmat, lights,  model);

	
	
	///////////////////////////////////////////////////////////////
	/////Vertex shader and buffer setups///////////////////////////
	///////////////////////////////////////////////////////////////
	
	
	//Reserve memory for post vertex shader output buffers
	m_vbuffer_out.clear();
	m_idxbuffer_out.clear();

	std::vector<Vertex> vertex_buffer = model->getMesh()->getVBuffer();
	std::vector<int> index_buffer = model->getMesh()->getIdxBuffer();

	m_vbuffer_out.resize(vertex_buffer.size()); //DYNAMIC ALLOCATION WITHIN MAIN LOOP EVIL BAD NO
	m_idxbuffer_out.resize(index_buffer.size());
	
	//Copy mesh index buffer
	m_idxbuffer_out = index_buffer;


	//Run vertex shader on all vertices in vertex buffer first. (Could iterate index buffer instead, would only change performance if parallellism is added and then it would depend on implementation)
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < vertex_buffer.size(); i++) 
	{
		m_vbuffer_out[i] = m_shader->vertex(vertex_buffer[i]);
	}
	

	///////////////////////////////////////////////////////////////
	/////Clip primitives///////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	m_clipping_mask.clear();
	m_clipping_mask.resize(m_vbuffer_out.size());

#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < m_vbuffer_out.size(); i++) 
	{
		//Build clipping mask for each vertex
		int mask = 0;
		Vec3f vert = m_vbuffer_out[i].position;

		if (vert.x + vert.w < 0) { mask |= ClippingMask::NegX; };
		if (vert.w - vert.x < 0) { mask |= ClippingMask::PosX; };
		if (vert.y + vert.w < 0) { mask |= ClippingMask::NegY; };
		if (vert.w - vert.y < 0) { mask |= ClippingMask::PosY; };
		if (		 vert.z < 0) { mask |= ClippingMask::NegZ; };
		if (vert.w - vert.z < 0) { mask |= ClippingMask::PosZ; };
		
		m_clipping_mask[i] = mask;
		
	}
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < m_idxbuffer_out.size(); i += 3) 
	{
		
		//Clip triangles outside mask
		bool isClipped = false;
		int combined_mask = m_clipping_mask[m_idxbuffer_out[i]] | m_clipping_mask[m_idxbuffer_out[i+1]] | m_clipping_mask[m_idxbuffer_out[i+2]];
		if (combined_mask & ClippingMask::NegX) { isClipped = true; };
		if (combined_mask & ClippingMask::PosX) { isClipped = true; };
		if (combined_mask & ClippingMask::NegY) { isClipped = true; };
		if (combined_mask & ClippingMask::PosY) { isClipped = true; };
		if (combined_mask & ClippingMask::NegZ) { isClipped = true; };
		if (combined_mask & ClippingMask::PosZ) { isClipped = true; };

		if(isClipped) //TEMPORARILY TURNED OFF not sure if this comment is right
		{
			m_idxbuffer_out[i]   = -1;
			m_idxbuffer_out[i+1] = -1;
			m_idxbuffer_out[i+2] = -1;
		};

	}

	///////////////////////////////////////////////////////////////
	/////Transform vertices from NDC to Screen space///////////////
    ///////////////////////////////////////////////////////////////

	m_processedFlag.clear();
	m_processedFlag.resize(vertex_buffer.size());
	for (int i = 0; i < m_idxbuffer_out.size(); i++) 
	{
		int idx = m_idxbuffer_out[i];

		if (idx == -1) {continue;}//Data race, cant easily parallelize with omp
		if (m_processedFlag[idx]) {continue;}
		
		
		Vec3f& vout = m_vbuffer_out[idx].position;

		//Perpective divide
		vout.perspecDiv();

		//Viewport transform. Vertex from NDC to SSC
		vout.x = vout.x * ((m_px_buff->m_width - 1 ) / 2) + (m_px_buff->m_width / 2);
		vout.y = -vout.y * ((m_px_buff->m_height - 1) / 2) + (m_px_buff->m_height / 2);
		//no need for z since NDC cube already between 0 and 1

		m_processedFlag[idx] = true;
	}
	

	//Draw primitives (aka draw triangle with 3 indices as input)
	Rasterizer::drawTriangle(m_vbuffer_out, m_idxbuffer_out, m_shader.get(), m_px_buff, m_z_buff);




	//Iterate each face
	//Parallelize loop. shader is private to each thread and initialized as the original shader. 
	//Schedule dynamic since many threads will finish early due to early rejection due to front end backface culling and clipping
	
	//#pragma omp parallel for firstprivate(m_shader) schedule(dynamic)
	
}


void Renderer::setShaderUniforms(const Mat4f MV, const Mat4f MVP, const Mat4f V, const Mat4f N, const SceneLights* sceneLights, const Model* model) {

	assert(m_shader != nullptr);

	auto lights = sceneLights->dirLights;
	const Material* temp_mat = model->getMaterial();
	switch (m_shader->getType()) {
		
		case ShaderType::PBR:
		{
			PBRShader* temp = dynamic_cast<PBRShader*>(m_shader.get());
			temp->MVmat = MV;
			temp->MVPmat = MVP;
			temp->Vmat = V;
			temp->Nmat = N;

			temp->Ka = 0.03f;
			temp->albedo_map = model->getTexture()->m_albedo;
			temp->normal_map = model->getTexture()->m_normal;
			temp->roughness_map = model->getTexture()->m_roughness;
			temp->metallic_map = model->getTexture()->m_metallic;
			temp->AO_map = model->getTexture()->m_AO;

			//Calculate and set light direction
			if (temp->light_dir.size() != lights.size()) {
				temp->light_dir.resize(lights.size());
				temp->light_colour.resize(lights.size());
			}
			for (int i = 0; i < lights.size(); i++)
			{
				temp->light_dir[i] = (temp->Vmat.convertDirToViewSpace(lights[i].m_direction)); //
				temp->light_dir[i].normalize();
				temp->light_colour[i] = (lights[i].m_colour);//HDR value idk
			}
		}
		break;

		case ShaderType::BLINNPHONG:
		{
			BlinnPhongShader* temp = dynamic_cast<BlinnPhongShader*>(m_shader.get());
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
			temp->texture = model->getTexture()->m_albedo;
			temp->normal_map = model->getTexture()->m_normal;

			//Calculate and set light direction
			if (temp->light_dir.size() != lights.size()) {
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
			temp->texture = model->getTexture()->m_albedo;

			//Calculate and set light direction
			if (temp->light_dir.size() != lights.size()) {
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

		case ShaderType::NORMALMAP:
		{
			NormalMapShader* temp = dynamic_cast<NormalMapShader*>(m_shader.get());
			temp->MVPmat = MVP;
			temp->Nmat = N;
			temp->normal_map = model->getTexture()->m_normal;

		}
		break;

		case ShaderType::ALBEDO:
		{
			AlbedoShader* temp = dynamic_cast<AlbedoShader*>(m_shader.get());
			temp->MVPmat = MVP;	
			temp->albedo = model->getTexture()->m_albedo;

		}
		break;

		case ShaderType::ROUGHNESS:
		{
			RoughnessShader* temp = dynamic_cast<RoughnessShader*>(m_shader.get());
			temp->MVPmat = MVP;
			temp->roughness = model->getTexture()->m_roughness;

		}
		break;

		case ShaderType::METALLIC:
		{
			MetallicShader* temp = dynamic_cast<MetallicShader*>(m_shader.get());
			temp->MVPmat = MVP;
			temp->metallic = model->getTexture()->m_metallic;

		}
		break;

		case ShaderType::AO:
		{
			AOShader* temp = dynamic_cast<AOShader*>(m_shader.get());
			temp->MVPmat = MVP;
			temp->AO = model->getTexture()->m_AO;

		}
		break;
		
		
		default:
			printf("Setting uniforms failed.");
		break;
	}




}