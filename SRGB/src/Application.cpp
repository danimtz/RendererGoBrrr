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
	Uint64 ticks = 0;
	Uint64 last_frame_ticks = 0;
	float delta_t = 0.0f;
	while (running)
	{
		ticks = SDL_GetPerformanceCounter();
		delta_t = (float)(ticks - last_frame_ticks)/(float)SDL_GetPerformanceFrequency();
		last_frame_ticks = ticks;

		std::cout << "Current frametime: "<< delta_t << " s"<< std::endl;
		//Timer fpstimer;
		m_input_handler->onUpdate(running, delta_t);
		m_scene->onUpdate(delta_t);

		//scene->onUpdate(deltatime)
		Camera *cam = m_scene->getCam();

		
		

		m_renderer->renderScene(m_scene);

		m_window->onUpdate();//Swap buffers

		//printf("newframe\n");
	}

}