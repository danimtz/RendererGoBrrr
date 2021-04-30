#include <iostream>



#include "Application.h"



#include "Mesh.h"
#include "OBJLoader.h"
int main(int argc, char* args[])
{
	

	
	
	Application *app = new Application();

	app->run();

	delete app;
	

	//Mesh *aaa = OBJLoader::loadMesh("assets\\cube2.obj");
	return 0;
}


	
