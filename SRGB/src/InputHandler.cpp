#include "InputHandler.h"



InputHandler::InputHandler(Camera *cam, Renderer *renderer) : m_sceneCam(cam), m_renderer(renderer)
{
	SDL_SetRelativeMouseMode(SDL_TRUE); 
};


InputHandler::~InputHandler() {};


void InputHandler::onUpdate(bool &running)
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				running = false;
				break;

			default:
				handleSDLevent(running, event);
				break;
		}
	}
	
	
}


void InputHandler::handleSDLevent(bool &running, SDL_Event &event)
{

	float speed = m_sceneCam->camSpeed;// * deltaT;

	//Handling keyboard input 
	if (event.type == SDL_KEYDOWN)
	{

		if (event.key.keysym.sym == SDLK_w)
		{
			m_sceneCam->m_pos += m_sceneCam->m_front * speed;
		}

		if (event.key.keysym.sym == SDLK_s)
		{
			m_sceneCam->m_pos -= m_sceneCam->m_front * speed;
		}

		if (event.key.keysym.sym == SDLK_a)
		{
			m_sceneCam->m_pos -= m_sceneCam->m_right * speed;
		}

		if (event.key.keysym.sym == SDLK_d)
		{
			m_sceneCam->m_pos += m_sceneCam->m_right * speed;
		}

		if (event.key.keysym.sym == SDLK_SPACE)
		{
			m_sceneCam->m_pos += m_sceneCam->m_world_up * speed;
		}

		if (event.key.keysym.sym == SDLK_LSHIFT)
		{
			m_sceneCam->m_pos -= m_sceneCam->m_world_up * speed;
		}

		if (event.key.keysym.sym == SDLK_LALT) 
		{

			if (SDL_GetRelativeMouseMode() == 0) 
			{
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
			else 
			{
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
			
		if (event.key.keysym.sym == SDLK_r)
		{
			//RESET CAMERA FUNCTION
		}

		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			running = false;
		}


		//Update camera
		m_sceneCam->updateCam();




		//Change shader in renderer
		if (event.key.keysym.sym == SDLK_1)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<FlatShader>(new FlatShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_2)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<GouradShader>(new GouradShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_3)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<PhongShader>(new PhongShader());
			m_renderer->loadShader(shader);
		}
		/*if (event.key.keysym.sym == SDLK_4)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<DebugShader>(new DebugShader());
			m_renderer->loadShader(shader);
		}*/



	}
	else if ((event.motion.state & SDL_BUTTON_LMASK) || (event.motion.state & SDL_BUTTON_RMASK))
	{
		if (event.type == SDL_MOUSEMOTION)
		{ 

			float sens = 0.05f;
			float xOffset = (float)event.motion.xrel * sens;
			float yOffset = -(float)event.motion.yrel * sens;

			m_sceneCam->m_yaw += xOffset;
			m_sceneCam->m_pitch += yOffset;

			//Constraint camera
			if (m_sceneCam->m_pitch > 89.0f)
			{
				m_sceneCam->m_pitch = 89.0f;
			}
			else if (m_sceneCam->m_pitch < -89.0f)
			{
				m_sceneCam->m_pitch = -89.0f;
			}

			m_sceneCam->updateCam();
		}
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{

		float zoom = 5.0f;
		float fov = m_sceneCam->m_fov;

		if (event.wheel.y > 0)// scroll up
		{ 
			fov -= zoom;
		}
		else if (event.wheel.y < 0)// scroll down
		{
			fov += zoom;
		}

		//Limit FOV range
		if (fov < 20)
		{
			fov = 20;
		}
		else if (fov > 130)
		{
			fov = 130;
		}

		//Updating the camera
		m_sceneCam->updateCamFOV(fov);
		m_sceneCam->updateCam();
	}
}