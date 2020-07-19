#include "Renderer.h"


//Constructor
Renderer::Renderer(int buff_width, int buff_height) : m_px_buff(nullptr)
{
	m_px_buff = new Buffer<uint32_t>(buff_width, buff_height);
}

//Destructor
Renderer::~Renderer()
{
	delete m_px_buff;
}


Buffer<uint32_t>* Renderer::getRenderTarget() 
{
	return m_px_buff;
}