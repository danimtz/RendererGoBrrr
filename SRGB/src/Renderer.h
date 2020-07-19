#pragma once

#include "Buffer.h"


class Renderer {
public:

	Renderer(int buff_width, int buff_height);
	~Renderer();

	Buffer<uint32_t>* getRenderTarget() ;




private:

	Buffer<uint32_t> *m_px_buff;



};

