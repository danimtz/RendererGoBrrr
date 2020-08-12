#pragma once
#include "SDL.h"
#include "Camera.h"


class InputHandler {
public:

	InputHandler(Camera *cam);
	~InputHandler();

	void onUpdate(bool &running);

private:

	Camera *m_sceneCam;
	
	void handleSDLevent(bool &running, SDL_Event &event);

};

