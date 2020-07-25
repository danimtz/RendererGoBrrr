#include "Scene.h"


Scene::Scene()
{
	//TEMPORARY MODEL CREATION LATER WILL BE DONE TRHOUGH READING A FILE OR SOMETHING

	
	Mat4f rot = Mat4f::createRotAxis(Vec3f(90,0,0));
	Mat4f sc = Mat4f::createScale(Vec3f(1.5,1.5,1));
	Mat4f trans = rot*sc; //T R S (firts scale then rotate then translate)(although theres no translation here)
	
	Model *model = new Model("assets\\head.obj", trans);

	m_models_in_scene.push_back(model);
	

}

Scene::~Scene()
{
	// Deallocate models in vector
	for (int i = 0; i < m_models_in_scene.size(); ++i)
	{
		delete m_models_in_scene[i]; 
	}
}


std::queue<Model*>* Scene::createRenderQueue()//Put models to be rendered from scene in queue.
{
	//CURRENLY JUST PLACES ALL MODELS IN SCENE IN THE QUEUE. CHANGE THIS LATER


	for (int i = 0; i < m_models_in_scene.size(); i++)
	{
		
		m_render_queue.push(m_models_in_scene[i]);
	}

	return &m_render_queue;
}