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

	//Render wireframe given a model. 
	//void renderWireFrame(const Model *model, uint32_t colour); DEPRECATED

	//Render a model. extremly simple at the moment. must be overhauled later
	void renderModel(const Model *model, const SceneLights* lights);

	//Render a scene
	void renderScene(Scene *scene);

	void setRenderCam(Camera *cam);

	//Loads shader by moving data from created shader to member shader
	void loadShader(std::unique_ptr<IShader>& shader) { this->m_shader = std::move(shader); } //CHECK UNDERSTANDING OF THIS LINE /AKA go trough debugger memory understand how it works

private:
	struct ClippingMask {
		enum vLimits{
			PosX = 0b000001,
			NegX = 0b000010,
			PosY = 0b000100,
			NegY = 0b001000,
			PosZ = 0b010000,
			NegZ = 0b100000
		};
	};
	void setShaderUniforms(const Mat4f MV, const Mat4f MVP, const Mat4f V, const Mat4f N, const SceneLights* sceneLights, const Model* model);

private:

	Camera *m_camera; //Scene owns this

	std::unique_ptr<IShader> m_shader;

	//Buffers
	std::vector<int> m_idxbuffer_out;
	std::vector<VShaderOut> m_vbuffer_out;

	
	std::vector<bool> m_processedFlag; //Bool buffer to check if vertex has already been processed
	std::vector<int> m_clipping_mask; //Stores clipping mask for vertices

	Buffer<uint32_t>* m_px_buff;
	Buffer<float>* m_z_buff;



};

