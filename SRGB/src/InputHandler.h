#pragma once
#include "SDL.h"
#include "Scene.h"
#include "Renderer.h"
#include "Shader.h"

class InputHandler {
public:

	InputHandler(Scene *scene, Renderer* m_renderer);
	~InputHandler();

	void onUpdate(bool &running, float delta_t);

private:

	Scene *m_scene;
	Renderer* m_renderer;
	void handleSDLevent(bool &running, float delta_t, SDL_Event &event);

};

