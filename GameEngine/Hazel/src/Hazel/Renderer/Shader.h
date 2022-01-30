#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Hazel {
	class Shader {
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind() const;
		void UnBind() const;
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		uint32_t GetID() const;
	private:
		uint32_t m_RendererID;
	};
}