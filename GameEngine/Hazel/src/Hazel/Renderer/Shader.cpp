#include "hzpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Hazel {
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
		std::string vertexSrc;
		std::string fragmentSrc;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//��֤ifStream��������׳��쳣
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//��ȡ�ļ��е����ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			// ת����������string
			vertexSrc = vShaderStream.str();
			fragmentSrc = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::�ļ���ȡʧ�� "<< std::endl;
		}

		//����һ���յ�vertexShader���
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		//��������ɫ�����͵�GL
		//���vertexSrc.c_strΪ�յĻ�,����ͻ��ս�
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		//���붥��shader
		glCompileShader(vertexShader);

		//�������
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			//������Ҫshader��
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}",infoLog.data());
			HZ_CORE_ASSERT(false, "������ɫ������ʧ��");
		}

		//����һ���յ�ƬԪshader���
		GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//��ƬԪ��ɫ�����͵�GL
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		//���붥��shader
		glCompileShader(fragmentShader);
		//�������
		isCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			//������Ҫshader��
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "ƬԪ��ɫ������ʧ��");
		}
		//������ɫ���������,��������Ҫ���ص�������
		//�Ȼ�ȡ�������
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		//�����ǵ�Shader���ӵ�������
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		//���ӳ���
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
			HZ_CORE_ASSERT(false, "ƬԪ��ɫ���Ͷ�����ɫ������ʧ��");
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