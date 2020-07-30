
#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* filename)
{
	stbi_set_flip_vertically_on_load(true);
	m_raw_data = stbi_load(filename, &m_width, &m_height, &m_nchannels, 0);

	m_data = new float[m_width * m_height * m_nchannels];

	for (int i = 0; i < m_width * m_height * m_nchannels; ++i)
	{
		//Scale to 0 to 1 and gamma correct ^2.2 CURRENTLY TURNED OFF
		m_data[i] = std::pow((float)m_raw_data[i] * (1 / 255.0f), 1.0f);
	}


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