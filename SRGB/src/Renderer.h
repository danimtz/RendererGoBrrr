#pragma once

#include "Buffer.h"
#include "Model.h"
#include "Rasterizer.h"

class Renderer {
public:

	Renderer(int buff_width, int buff_height);
	~Renderer();

	//Get buffer
	Buffer<uint32_t>* getRenderTarget();

	//Render wireframe given a model. ONLY NDC RIGHT NOW
	void renderWireFrame(Model *model, uint32_t colour);

private:

	Buffer<uint32_t> *m_px_buff;


};

