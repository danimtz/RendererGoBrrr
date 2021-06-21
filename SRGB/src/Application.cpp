#include "Application.h"
#include "Profiler.h"

Application::Application():m_window(nullptr) , m_renderer(nullptr), m_scene(nullptr){


	WindowProps windProperties = WindowProps("WINDOOOOOOW", 1280, 720);

	m_window = new Window(windProperties);

	
	m_scene = new Scene();//SET NEW SCENE HERER. CHANGE LATER TO BE ABLE TO SWAP SCENES OR SOMETHING

	m_renderer = m_window->getGraphicContext();


	m_renderer->setRenderCam(m_scene->getCam());//THIS MIGHT GO AGAINST RAII idk
	
	m_input_handler = new InputHandler(m_scene->getCam(), m_renderer);

}

Application::~Application()
{
	delete m_window;
	delete m_scene;
	delete m_input_handler;
}



void Application::run()
{

	bool running = true;
	
	
	//main loop
	while (running)
	{

		//Timer fpstimer;
		m_input_handler->onUpdate(running);
		
		//scene->onUpdate(deltatime)
		Camera *cam = m_scene->getCam();

		
		//m_scene->onUpdate();//update scene //TODO

		m_renderer->renderScene(m_scene);

		m_window->onUpdate();//Swap buffers

		//printf("newframe\n");
	}

}