#pragma once

#include "Buffer.h"
#include "Model.h"
#include "Rasterizer.h"
#include "Scene.h"
#include "Camera.h"
#include "Matrix4.h"
#include "Shader.h"

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
	void renderModel(const Model *model, const SceneLights* lights);

	//Render a scene
	void renderScene(Scene *scene);

	void setRenderCam(Camera *cam);

	//Loads shader by moving data from created shader to member shader
	void loadShader(std::unique_ptr<IShader>& shader) { this->m_shader = std::move(shader); } //CHECK UNDERSTANDING OF THIS LINE /AKA go trough debugger memory understand how it works

private:
	
	void setShaderUniforms(const Mat4f MV, const Mat4f MVP, const Mat4f V, const Mat4f N, const SceneLights* sceneLights, const Model* model);

	Buffer<uint32_t> *m_px_buff;
	Buffer<float> *m_z_buff;

	Camera *m_camera; //Scene owns this

	std::unique_ptr<IShader> m_shader;

};

