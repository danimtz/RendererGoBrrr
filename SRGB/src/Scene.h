#pragma once


#include <iostream>
#include <queue>
#include "Model.h"
#include "Camera.h"

//TODO: Add scene to renderer class?
class Scene {
public:

	Scene();
	~Scene();

	std::queue<Model*>* createRenderQueue();//Camera culling etc etc
	
	Camera* getCam() const;

private:


	std::vector<Model*> m_models_in_scene;
	std::queue<Model*> m_render_queue;

	Camera *m_camera;

};

