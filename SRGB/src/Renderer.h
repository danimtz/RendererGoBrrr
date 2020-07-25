#pragma once

#include "Buffer.h"
#include "Model.h"
#include "Rasterizer.h"
#include "Scene.h"


#include <queue>//in .cpp maybe?

class Renderer {
public:

	Renderer(int buff_width, int buff_height);
	~Renderer();

	//Get buffer
	Buffer<uint32_t>* getRenderTarget();
	Buffer<float>* getDepthBuffer();

	//Render wireframe given a model. ONLY NDC RIGHT NOW
	void renderWireFrame(Model *model, uint32_t colour);

	//Render a model. extremly simple at the moment. must be overhauled later
	void renderModel(Model *model);

	//Render a scene
	void renderScene(Scene *scene);

private:

	Buffer<uint32_t> *m_px_buff;
	Buffer<float> *m_z_buff;


};

