#pragma once


#include <iostream>
#include <queue>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

struct SceneLights {
	std::vector<PointLight> pointLights;
	std::vector<DirLight>	dirLights;
};


class Scene {
public:

	Scene();

	std::queue<Model*>* createRenderQueue();//Camera culling etc etc

	SceneLights* getLights();
	
	Camera* getCam();
	
	void onUpdate(float delta_t);

	//Scene swapping utility: This should be in a separate class such as sceneUtility. 
	void changeScene();
	int m_scene_ID;
	const int max_scenes = 2;

private:
	
	std::vector<Model> m_models_in_scene;//owner
	
	std::queue<Model*> m_render_queue; //This should be in renderer i think
	
	SceneLights m_lights_in_scene;
	
	std::vector<Material> m_material_list;//could be a different data structure instead of vector

	Camera m_camera;

};

