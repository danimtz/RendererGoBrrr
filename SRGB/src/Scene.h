#pragma once


#include <iostream>
#include <queue>
#include "Model.h"
#include "Camera.h"
#include "Light.h"

//TODO: Add scene to renderer class?
class Scene {
public:

	Scene();
	~Scene();

	std::queue<Model*>* createRenderQueue();//Camera culling etc etc
	//LIGHTS
	std::vector<Light*>& getLights();

	Camera* getCam() const;

private:


	std::vector<Model*> m_models_in_scene;
	std::queue<Model*> m_render_queue;
	std::vector<Light*> m_lights_in_scene;

	Camera *m_camera;

};

