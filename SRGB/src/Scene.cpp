#include "Scene.h"


Scene::Scene()
{
	//Create materials
	m_material_list.push_back(Material());

	m_material_list.push_back(Material(Vec3f(12, 134, 36), 32.0f, 0.5f)); //0.4 ks

	m_material_list.push_back(Material(Vec3f(255, 255, 255), 32.0f, 0, Vec3f(255,255,255),1,0));

	//TEMPORARY MODEL CREATION LATER WILL BE DONE TRHOUGH READING A FILE OR SOMETHING. 
	//CAM PROPERTIES SHOULD BE SPECIFIED IN FILE AS WELL AND PUT INTO A STRUCT LIKE CAM_PROPERTIES
	Mat4f rot,sc,trs,transf;
	
	

	
	rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
	sc = Mat4f::createScale(Vec3f(0.4, 0.4, 0.4));
	trs = Mat4f::createTranslation(Vec3f(1, 0, -1));
	transf = trs*(rot*sc);
	//m_models_in_scene.push_back(Model("assets\\head.obj", "assets\\african_head_diffuse.tga", &m_material_list[0], transf));


	
	rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
	sc = Mat4f::createScale(Vec3f(4, 4, 4));
	trs = Mat4f::createTranslation(Vec3f(-1, -0.5, 0));
	transf = trs * (rot * sc);
	//m_models_in_scene.push_back(Model("assets\\bunnyHD.obj", &m_material_list[0], transf));
	


	rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
	sc = Mat4f::createScale(Vec3f(0.3, 0.3, 0.3));
	trs = Mat4f::createTranslation(Vec3f(0, 0, 0));
	transf = trs * (rot * sc);
	m_models_in_scene.push_back(Model("assets\\survival_guitar\\source\\guitar.obj", "assets\\survival_guitar\\source\\1001_albedo.jpg", &m_material_list[0], transf));
	
	

	rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
	sc = Mat4f::createScale(Vec3f(1, 1, 1));
	trs = Mat4f::createTranslation(Vec3f(0, -0.5, 0));
	transf = trs * (rot * sc);
	
	//m_models_in_scene.push_back(Model("assets\\cube2.obj", "assets\\uv_mapper.jpg", &m_material_list[1], transf));
	//m_models_in_scene.push_back(Model("assets\\sphere.obj",  &m_material_list[1], transf));
	
	//m_models_in_scene.back().setDrawNormal(true);
	
	m_camera = Camera(Vec3f(0, 0, 2), 0.0f, -90.0f);


	//Lights

	//new Light(Vec3f(0, 0, -1), Vec3f(0,0,0), Vec3f(0.2, 0.8, 0.9)); //BLUEISH LIGHT


	//m_lights_in_scene.dirLights.push_back( DirLight(Vec3f(0.7, 0.8, 1), Vec3f(0.7, 0.4, 0.15), Vec3f(0, 0, 0))); //Warm colouredish


	
	//m_lights_in_scene.dirLights.push_back( DirLight(Vec3f(0, 0.3, -1), Vec3f(0.4, 0.5, 0.9), Vec3f(0, 0, 0)) ); //BACK

	Vec3f lightpos = Vec3f(1, 1.5, 4); //(1,3,-6) Vec3f(0, 2, -2)
	m_lights_in_scene.dirLights.push_back( DirLight(lightpos, Vec3f(1, 1, 1), Vec3f(0, 0, 0)) );



	//LIGHT OBJECT
	rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
	sc = Mat4f::createScale(Vec3f(0.1, 0.1, 0.1));
	trs = Mat4f::createTranslation(lightpos);
	transf = trs * (rot * sc);

	//m_models_in_scene.push_back(Model("assets\\cube.obj", &m_material_list[2], transf));//LIGHTCUBE


	std::cout<< "Finished scene setup."<<std::endl;

	
}




std::queue<Model*>* Scene::createRenderQueue()//Put models to be rendered from scene in queue.
{
	//CURRENLY JUST PLACES ALL MODELS IN SCENE IN THE QUEUE. CHANGE THIS LATER


	for (int i = 0; i < m_models_in_scene.size(); i++)
	{
		
		m_render_queue.push(&m_models_in_scene[i]);
	}

	return &m_render_queue;
}

SceneLights* Scene::getLights()
{
	return &m_lights_in_scene;
}

Camera* Scene::getCam() 
{
	return &m_camera;
}