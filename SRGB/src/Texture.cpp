
#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture():m_width(0), m_height(0), m_nchannels(0),m_data(nullptr){}

Texture::Texture(const char* filename)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* raw_data = stbi_load(filename, &m_width, &m_height, &m_nchannels, 0);
	
	/*if (stbi_failure_reason()){ 
		std::cout << stbi_failure_reason() << std::endl;
		//assert(false);
	}*/

	m_data = new float[m_width * m_height * m_nchannels];

	for (int i = 0; i < m_width * m_height * m_nchannels; ++i)
	{
		//Scale to 0 to 1
		m_data[i] = (float)raw_data[i] * (1 / 255.0f);
	}

	stbi_image_free(raw_data);

}

Texture::Texture(const Texture &other) : m_width(other.m_width), m_height(other.m_height), m_nchannels(other.m_nchannels)
{
	int size = m_width * m_height * m_nchannels;
	m_data = new float[size];
	memcpy(m_data, other.m_data, sizeof(float)*size);
}

Texture &Texture::operator=(const Texture& other) 
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_nchannels = other.m_nchannels;

	//copy dynamic memory
	if (m_data!=nullptr)
	{ 
		delete[] m_data; 
	}
	int size = m_width * m_height * m_nchannels;
	m_data = new float[size];
	memcpy(m_data, other.m_data, sizeof(float) * size);
	
	return *this;
}

Texture::~Texture()
{
	delete[] m_data;
}


//Returns corresponding texel
Vec3f Texture::getTexel(float u, float v) const
{
	int x = u*(m_width-1);
	int y = v*(m_height-1);


	int index = ((x + y * m_width) * m_nchannels);

	Vec3f rgb{m_data[index]*255,  m_data[index+1]*255, m_data[index+2]*255};

	return rgb;
}