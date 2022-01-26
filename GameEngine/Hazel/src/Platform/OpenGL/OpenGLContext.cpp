#include "hzpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "窗口句柄是空");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		//把glfw的glfwGetProcAddress函数赋给glad(使用函数指针转型)
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "无法初始化Glad!");
	
		HZ_CORE_INFO("OpenGL Vendor:{0}", glGetString(GL_VENDOR));
		HZ_CORE_INFO("OpenGL Renderer:{0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("OpenGL Version:{0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}