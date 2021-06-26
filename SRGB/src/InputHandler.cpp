#include "InputHandler.h"



InputHandler::InputHandler(Scene *scene, Renderer *renderer) : m_scene(scene), m_renderer(renderer)
{
	SDL_SetRelativeMouseMode(SDL_TRUE); 
};


InputHandler::~InputHandler() {};


void InputHandler::onUpdate(bool &running, float delta_t)
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
				handleSDLevent(running, delta_t, event);
				break;
		}
	}
}


void InputHandler::handleSDLevent(bool &running, float delta_t, SDL_Event &event)
{
	Camera *sceneCam = m_scene->getCam();
	float speed = sceneCam->camSpeed ; //*delta_t ??? this breaks it more than fixes anything atm

	//Handling keyboard input 
	if (event.type == SDL_KEYDOWN)
	{
		
		if (event.key.keysym.sym == SDLK_w)
		{
			if (sceneCam->m_cam_type == 1)
			{
				sceneCam->m_sph_coords.x -= speed;
			}
			else if(sceneCam->m_cam_type == 0)
			{
				sceneCam->m_pos += sceneCam->m_front * speed;
			}
		}

		if (event.key.keysym.sym == SDLK_s)
		{
			if (sceneCam->m_cam_type == 1)
			{
				sceneCam->m_sph_coords.x += speed;
			}
			else if(sceneCam->m_cam_type == 0)
			{
				sceneCam->m_pos -= sceneCam->m_front * speed;
			}
		}

		//pan
		if (event.key.keysym.sym == SDLK_a)
		{
			sceneCam->m_pos -= sceneCam->m_right * speed;
			sceneCam->m_origin -= sceneCam->m_right * speed;
		}

		if (event.key.keysym.sym == SDLK_d)
		{
			sceneCam->m_pos += sceneCam->m_right * speed;
			sceneCam->m_origin += sceneCam->m_right * speed;
		}

		if (event.key.keysym.sym == SDLK_SPACE)
		{
			sceneCam->m_pos += sceneCam->m_world_up * speed;
			sceneCam->m_origin += sceneCam->m_world_up * speed;
		}

		if (event.key.keysym.sym == SDLK_LSHIFT)
		{
			sceneCam->m_pos -= sceneCam->m_world_up * speed;
			sceneCam->m_origin -= sceneCam->m_world_up * speed;
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
			sceneCam->resetCam();
		}

		if (event.key.keysym.sym == SDLK_t)
		{
			sceneCam->changeCamType();
		}

		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			running = false;
		}


		//Update camera
		sceneCam->updateCam(); //Maybe in scene update
		
		




		//Change shader in renderer
		/*if (event.key.keysym.sym == SDLK_1)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<FlatShader>(new FlatShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_2)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<GouradShader>(new GouradShader());
			m_renderer->loadShader(shader);
		}*/
		if (event.key.keysym.sym == SDLK_3)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<PhongShader>(new PhongShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_4)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<BlinnPhongShader>(new BlinnPhongShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_5)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<PBRShader>(new PBRShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_6)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<AlbedoShader>(new AlbedoShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_7)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<NormalMapShader>(new NormalMapShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_8)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<MetallicShader>(new MetallicShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_9)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<RoughnessShader>(new RoughnessShader());
			m_renderer->loadShader(shader);
		}
		if (event.key.keysym.sym == SDLK_0)
		{
			std::unique_ptr<IShader> shader = std::unique_ptr<AOShader>(new AOShader());
			m_renderer->loadShader(shader);
		}

		//Change scene
		if (event.key.keysym.sym == SDLK_RIGHT)
		{
			m_scene->m_scene_ID += 1;
			if (m_scene->m_scene_ID > m_scene->max_scenes) {
				m_scene->m_scene_ID = 0;
			}
			m_scene->changeScene();
		}
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			m_scene->m_scene_ID -= 1;
			if (m_scene->m_scene_ID < 0) {
				m_scene->m_scene_ID = m_scene->max_scenes;
			}
			m_scene->changeScene();
		}

	}
	else if ((event.motion.state & SDL_BUTTON_LMASK) || (event.motion.state & SDL_BUTTON_RMASK))
	{
		if (event.type == SDL_MOUSEMOTION)
		{ 
			
			float sens = 0.05f;
			float xOffset = (float)event.motion.xrel * sens;
			float yOffset = -(float)event.motion.yrel * sens;

			

			sceneCam->m_yaw += xOffset;
			sceneCam->m_pitch += yOffset;

			//Constraint camera
			if (sceneCam->m_pitch > 89.0f)
			{
				sceneCam->m_pitch = 89.0f;
			}
			else if (sceneCam->m_pitch < -89.0f)
			{
				sceneCam->m_pitch = -89.0f;
			}

			sceneCam->updateCam();
	
		}
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{

		float zoom = 5.0f;
		float fov = sceneCam->m_fov;

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
		sceneCam->updateCamFOV(fov);
		sceneCam->updateCam();
	}
}