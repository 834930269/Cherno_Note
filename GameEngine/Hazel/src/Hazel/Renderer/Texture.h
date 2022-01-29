#pragma once
#include <string>
namespace Hazel {
	class Texture {
	public:
		/// <summary>
		/// 第一个是路径
		/// </summary>
		/// <param name="path"></param>
		Texture(const std::string &path, bool isRGB);
		~Texture();

		uint32_t GetID() const;
		void EnableRGBA() { m_isRGB = false; }
	private:
		uint32_t m_TextureID;
		int width, height, nrChannels;
		bool m_isRGB = true;
	};
}