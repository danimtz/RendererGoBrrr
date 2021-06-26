#include "Scene.h"


Scene::Scene()
{
	
	m_scene_ID = 0;
	
	//m_material_list.push_back(Material(Vec3f(12, 134, 36), 256.0f, 1.0f)); //0.4 ks

	//TEMPORARY MODEL CREATION LATER WILL BE DONE TRHOUGH READING A FILE OR SOMETHING. 

	Mat4f rot,sc,trs,transf;
	m_material_list.push_back(Material());

	//objects
	rot = Mat4f::createRotAxis(Vec3f(0, 245, 0));
	sc = Mat4f::createScale(Vec3f(1, 1, 1));
	trs = Mat4f::createTranslation(Vec3f(0, -1, 0));
	transf = trs * (rot * sc);
	m_models_in_scene.push_back(Model("assets\\hydrant\\firehydrant_mesh.obj", "assets\\hydrant\\firehydrant", &m_material_list[0], transf));

	//lights
	Vec3f lightpos1, lightpos2;
	lightpos1 = Vec3f(0.5, 0.6, 0.8);
	lightpos2 = Vec3f(0, 1, 0);
	m_lights_in_scene.dirLights.push_back(DirLight(lightpos1, Vec3f(1.0f, 0.82f, 0.71f), Vec3f(0, 0, 0)));//sun
	m_lights_in_scene.dirLights.push_back(DirLight(lightpos2, Vec3f(0.5f, 0.8f, 0.6f), Vec3f(0, 0, 0)));//sky

	//light objects

	//camera
	m_camera = Camera(Vec3f(0, 1, 2), 25.0f, -90.0f);

	//LIGHT OBJECT
	//rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
	//sc = Mat4f::createScale(Vec3f(0.5, 0.5, 0.5));
	//trs = Mat4f::createTranslation(lightpos1*20);
	//transf = trs * (rot * sc);
	//m_material_list.push_back(Material(Vec3f(255, 255, 255), 32.0f, 0, Vec3f(255,255,255),1,0));
	//m_models_in_scene.push_back(Model("assets\\cube.obj", &m_material_list[2], transf));//LIGHTCUBE


	std::cout<< "Finished scene setup."<<std::endl;

}

void Scene::changeScene() //THIS SHOULD READ NEW SCENE FROM FILE. (and the function should be in scene utility/manager class)
{
	//Clear scene buffers
	m_render_queue.empty();
	m_models_in_scene.clear();
	m_lights_in_scene.dirLights.clear();
	m_lights_in_scene.pointLights.clear();
	m_material_list.clear();

	//load new scene
	Mat4f rot, sc, trs, transf;
	Vec3f lightpos1, lightpos2;
	switch (m_scene_ID)
	{
		case 1://Chest
			m_material_list.push_back(Material());
			
			rot = Mat4f::createRotAxis(Vec3f(0, 0, 0));
			sc = Mat4f::createScale(Vec3f(1, 1, 1));
			trs = Mat4f::createTranslation(Vec3f(0, 0, 0));
			transf = trs * (rot * sc);
			m_models_in_scene.push_back(Model("assets\\chest\\chest_mesh.obj", "assets\\chest\\chest", &m_material_list[0], transf));
			
			//Lights
			m_lights_in_scene.dirLights.push_back(DirLight(Vec3f(0.0, 0.0, -1.0), Vec3f(0.4, 0.7, 1.0), Vec3f(0, 0, 0)));
			m_lights_in_scene.dirLights.push_back(DirLight(Vec3f(1.0, 0.0, 0.0), Vec3f(1.0, 1.0, 1.0), Vec3f(0, 0, 0)));
			m_lights_in_scene.dirLights.push_back(DirLight(Vec3f(1.0, 1.0, 1.0), Vec3f(1.0, 1.0, 1.0), Vec3f(0, 0, 0)));

			//camera
			m_camera = Camera(Vec3f(0, 1.5, 1), 15.0f, -90.0f);
			break;

		case 2://cerberus gun
			
			m_material_list.push_back(Material());

			//objects
			rot = Mat4f::createRotAxis(Vec3f(0, 120, 0));
			sc = Mat4f::createScale(Vec3f(1, 1, 1));
			trs = Mat4f::createTranslation(Vec3f(0, 0, 0));
			transf = trs * (rot * sc);
			m_models_in_scene.push_back(Model("assets\\cerberus\\meshes\\cerberus_mesh.obj", "assets\\cerberus\\materials\\cerberus", &m_material_list[0], transf));

			//lights
			lightpos1 = Vec3f(0.5, 0.6, 0.8);
			lightpos2 = Vec3f(0, 1, 0);
			m_lights_in_scene.dirLights.push_back(DirLight(lightpos1, Vec3f(1.0f, 0.82f, 0.71f), Vec3f(0, 0, 0)));//sun
			m_lights_in_scene.dirLights.push_back(DirLight(lightpos2, Vec3f(0.5f, 0.8f, 0.6f), Vec3f(0, 0, 0)));//sky

			//light objects

			//camera
			m_camera = Camera(Vec3f(0, 1, 1), 0.0f, -90.0f);
			break;


		default://Hydrant

			m_material_list.push_back(Material());

			//objects
			rot = Mat4f::createRotAxis(Vec3f(0, 245, 0));
			sc = Mat4f::createScale(Vec3f(1, 1, 1));
			trs = Mat4f::createTranslation(Vec3f(0, -1, 0));
			transf = trs * (rot * sc);
			m_models_in_scene.push_back(Model("assets\\hydrant\\firehydrant_mesh.obj", "assets\\hydrant\\firehydrant", &m_material_list[0], transf));

			//lights
			lightpos1 = Vec3f(0.5, 0.6, 0.8);
			lightpos2 = Vec3f(0, 1, 0);
			m_lights_in_scene.dirLights.push_back(DirLight(lightpos1, Vec3f(1.0f, 0.82f, 0.71f), Vec3f(0, 0, 0)));//sun
			m_lights_in_scene.dirLights.push_back(DirLight(lightpos2, Vec3f(0.5f, 0.8f, 0.6f), Vec3f(0, 0, 0)));//sky

			//light objects

			//camera
			m_camera = Camera(Vec3f(0, 1, 2), 25.0f, -90.0f);

			break;
	}
	std::cout << "Scene change!" << std::endl;
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


void Scene::onUpdate(float delta_t) 
{
	//update directional lights
	for (Light light : m_lights_in_scene.dirLights) {
		light.onUpdate(delta_t);
	}
	//update point lights
	for (Light light : m_lights_in_scene.pointLights) {
		light.onUpdate(delta_t);
	}
}

SceneLights* Scene::getLights()
{
	return &m_lights_in_scene;
}

Camera* Scene::getCam() 
{
	return &m_camera;
}