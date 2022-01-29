#include "hzpch.h"
#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Helper/std_image.h>

namespace Hazel {
	Hazel::Texture::Texture(const std::string& path,bool isRGB)
	{	
		m_isRGB = isRGB;
		//加载Texture
		//------------
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		//设置Texture的wrap模式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// 设置Texture过滤(采样)模式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//加载Texture,同时创建mipmap
		stbi_set_flip_vertically_on_load(true);//这一句是因为OpenGL的图片0.0坐标是在底部的,但通常图片的原点在顶部
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, m_isRGB ?GL_RGB:GL_RGBA, width, height, 0, m_isRGB ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
			//直接调用这个函数创建mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			HZ_CORE_ERROR("Failed to load texture");
		}
		//创建mipmap后释放加载的图片
		HZ_CORE_INFO("图片{0}加载成功",path.c_str());
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