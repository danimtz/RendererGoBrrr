#pragma once

#include <iostream>
#include <type_traits>
#include <SDL.h>

//Templated class to hold framebuffer and z-buffer
template<class T>
class Buffer {
public:
	


	int m_width;
	int m_height;
	int m_pitch;
	int m_px_count;
	size_t m_buff_sz;
	int m_botleft;//NEEDED IF BUFFER IS PLACED WITH ORIGIN AT BOTTOM LEFT
	T *buffer;


	//BUFFER CONSTRUCTOR AND DECONSTRUCTOR FOR *buffer
	Buffer(int width, int height) :
		m_width(width),
		m_height(height),
		m_pitch((int)width*sizeof(T)),
		m_px_count(width*height),
		m_buff_sz(height * m_pitch),
		m_botleft(width*height - width)

	{
		buffer = new T[m_px_count];
		clear();
	}

	~Buffer(){ delete[] buffer; }
	

	//Overload () operator to access specific pixel in array
	T& operator()(int x, int y)
	{
		return buffer[x + m_width*y];
		//return buffer[m_botleft + -m_width*y + x];
	}

	
	const T& operator()(int x, int y) const
	{
		return buffer[x + m_width*y];
		//return buffer[m_botleft + -m_width * y + x];
	}

	void clear()
	{
	//This could use memset
		for (int i = 0; i < m_px_count; i++)
		{
			buffer[i] = 0;
		}	
	}

	
	/*
private:
	inline static const uint32_t PX_FORMAT = SDL_PIXELFORMAT_RGB888;
	inline static uint32_t m_colour = SDL_MapRGB(SDL_AllocFormat(PX_FORMAT), 50, 50, 50);
*/

};

