#include "InputHandler.h"



InputHandler::InputHandler(Camera *cam) : m_sceneCam(cam) 
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
				handleSDLevent(event);
				break;
		}
	}
	
	
}


void InputHandler::handleSDLevent(SDL_Event &event)
{

	float speed = m_sceneCam->camSpeed;// * deltaT;

	//Handling keyboard input
	if (event.type == SDL_KEYDOWN)
	{

		switch (event.key.keysym.sym)
		{
			case SDLK_w:
				m_sceneCam->m_pos += m_sceneCam->m_front * speed;
				break;

			case SDLK_s:
				m_sceneCam->m_pos -= m_sceneCam->m_front * speed;
            
				break;

			case SDLK_a:
				m_sceneCam->m_pos -= m_sceneCam->m_right * speed;
				break;

			case SDLK_d:
				m_sceneCam->m_pos += m_sceneCam->m_right * speed;
				break;

			case SDLK_q:
				m_sceneCam->m_pos += m_sceneCam->m_up * speed;
				break;

			case SDLK_e:
				m_sceneCam->m_pos -= m_sceneCam->m_up * speed;
				break;

		}	
	}
	else if (event.motion.state & SDL_BUTTON_RMASK)
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
}