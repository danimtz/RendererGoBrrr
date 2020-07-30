#pragma once

#include "Buffer.h"
#include "Model.h"
#include "Rasterizer.h"
#include "Scene.h"
#include "Camera.h"
#include "Matrix4.h"

#include <queue>//in .cpp maybe?

class Renderer {
public:

	Renderer(int buff_width, int buff_height);
	~Renderer();

	//Get buffer
	Buffer<uint32_t>* getRenderTarget() const;
	Buffer<float>* getDepthBuffer() const;

	//Render wireframe given a model. ONLY NDC RIGHT NOW
	void renderWireFrame(const Model *model, uint32_t colour);

	//Render a model. extremly simple at the moment. must be overhauled later
	void renderModel(const Model *model, const std::vector<Light*>& lights);

	//Render a scene
	void renderScene(Scene *scene);

	void setRenderCam(Camera *cam);

private:

	Buffer<uint32_t> *m_px_buff;
	Buffer<float> *m_z_buff;

	Camera *m_camera; //Scene owns this

};

