#pragma once
#include<iostream>
#include"Window.h"
#include"Renderer.h"
#include"Scene.h"

class Application {

public:
	Application();
	~Application();

	void run();

private:

	Window* m_window;
	Renderer* m_renderer;
	Scene* m_scene;


};

