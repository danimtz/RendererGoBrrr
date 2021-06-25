#pragma once
#include<iostream>
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "InputHandler.h"


class Application {

public:
	Application();
	~Application();

	void run();

private:

	Window* m_window;
	Renderer* m_renderer;
	Scene* m_scene;//should be scene utility/manager
	InputHandler* m_input_handler;

};

