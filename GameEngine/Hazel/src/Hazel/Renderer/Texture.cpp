#include "hzpch.h"
#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Helper/std_image.h>

namespace Hazel {
	Hazel::Texture::Texture(const std::string& path,bool isRGB)
	{	
		m_isRGB = isRGB;
		//����Texture
		//------------
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		//����Texture��wrapģʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// ����Texture����(����)ģʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//����Texture,ͬʱ����mipmap
		stbi_set_flip_vertically_on_load(true);//��һ������ΪOpenGL��ͼƬ0.0�������ڵײ���,��ͨ��ͼƬ��ԭ���ڶ���
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, m_isRGB ?GL_RGB:GL_RGBA, width, height, 0, m_isRGB ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
			//ֱ�ӵ��������������mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			HZ_CORE_ERROR("Failed to load texture");
		}
		//����mipmap���ͷż��ص�ͼƬ
		HZ_CORE_INFO("ͼƬ{0}���سɹ�",path.c_str());
		stbi_image_free(data);
		//-------------
	}

	Hazel::Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	uint32_t Texture::GetID() const
	{
		return m_TextureID;
	}
}