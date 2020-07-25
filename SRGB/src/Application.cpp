#include "Application.h"


Application::Application():m_window(nullptr) , m_renderer(nullptr), m_scene(nullptr){


	WindowProps windProperties = WindowProps("WINDOOOOOOW", 800, 800);

	m_window = new Window(windProperties);

	m_renderer = m_window->getGraphicContext();

	m_scene = new Scene();//SET NEW SCENE HERER. CHANGE LATER TO BE ABLE TO SWAP SCENES OR SOMETHING


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

		m_renderer->renderScene(m_scene);

		m_window->onUpdate();//Swap buffers

	}

}