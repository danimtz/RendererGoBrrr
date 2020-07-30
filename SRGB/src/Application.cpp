#include "Application.h"


Application::Application():m_window(nullptr) , m_renderer(nullptr), m_scene(nullptr){


	WindowProps windProperties = WindowProps("WINDOOOOOOW", 1280, 720);

	m_window = new Window(windProperties);

	
	m_scene = new Scene();//SET NEW SCENE HERER. CHANGE LATER TO BE ABLE TO SWAP SCENES OR SOMETHING

	m_renderer = m_window->getGraphicContext();


	m_renderer->setRenderCam(m_scene->getCam());//THIS MIGHT GO AGAINST RAII idk
	

}

Application::~Application()
{
	delete m_window;
	delete m_scene;
}



void Application::run()
{

	bool running = true;
	//main loop

	SDL_Event event;//DEBUG
	while (running)
	{

		//TODO:

		//INPUT MANAGERISH CLASS HERE AND FEED RUNNING VARAIBEL ADDRESS TO IT TO CHANGE IT WHEN CLOSE WINDOW


		//DEBUG
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:

			running = false;
			break;
		}
		//DEBUG



		//scene->onUpdate(deltatime)
		Camera *cam = m_scene->getCam();

		//cam->updateCam();

		m_renderer->renderScene(m_scene);

		m_window->onUpdate();//Swap buffers

	}

}