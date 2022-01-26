#include "hzpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "���ھ���ǿ�");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		//��glfw��glfwGetProcAddress��������glad(ʹ�ú���ָ��ת��)
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "�޷���ʼ��Glad!");
	
		HZ_CORE_INFO("OpenGL Vendor:{0}", glGetString(GL_VENDOR));
		HZ_CORE_INFO("OpenGL Renderer:{0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("OpenGL Version:{0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}