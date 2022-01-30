#include "hzpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Hazel {
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		// 1. 从文件路径中获取顶点/片段着色器
		std::string vertexSrc;
		std::string fragmentSrc;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//保证ifStream对象可以抛出异常
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//读取文件中的数据到数据流中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();
			// 转换数据流到string
			vertexSrc = vShaderStream.str();
			fragmentSrc = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::文件读取失败 "<< std::endl;
		}

		//创建一个空的vertexShader句柄
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		//将顶点着色器发送到GL
		//如果vertexSrc.c_str为空的话,程序就会终结
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		//编译顶点shader
		glCompileShader(vertexShader);

		//编译错误
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			//不再需要shader了
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}",infoLog.data());
			HZ_CORE_ASSERT(false, "顶点着色器编译失败");
		}

		//创建一个空的片元shader句柄
		GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//将片元着色器发送到GL
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		//编译顶点shader
		glCompileShader(fragmentShader);
		//编译错误
		isCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			//不再需要shader了
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "片元着色器编译失败");
		}
		//顶点着色器编译完成,接下来需要加载到程序中
		//先获取程序对象
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		//将我们的Shader连接到程序中
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		//连接程序
		glLinkProgram(program);
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "片元着色器和顶点着色器连接失败");
		}
		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);	 
	}
	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void Shader::UnBind() const
	{
		glUseProgram(0);
	}
	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}
	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	uint32_t Shader::GetID() const
	{
		return m_RendererID;
	}
}