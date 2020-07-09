#pragma once

#include <iostream>
#include <type_traits>


//Templated class to hold framebuffer and z-buffer
template<class T>
class Buffer {
	public:

	int m_width;
	int m_height;
	int m_pitch;
	int m_px_count;
	size_t m_buff_sz;
	T *buffer;

	Buffer(int width, int height) :
		m_width(width),
		m_height(height),
		m_pitch(width*sizeof(T)),
		m_px_count(width*height),
		m_buff_sz(height * m_pitch)
	{
		buffer = new T[m_px_count];
		clear();
	}

	~Buffer(){ delete[] buffer; }
	//BUFFER CONSTRUCTOR AND DECONSTRUCTOR FOR *buffer

	//Overload () operator to access specific pixel in array
	T& operator()(int x, int y)
	{
		return buffer[x + m_width*y];
	}

	void clear()
	{
	//This could use memset
		for (int i = 0; i < m_px_count; i++)
		{
			buffer[i] = 0;
		}	
	}
};

