#include "hzpch.h"
#include "Application.h"

#include "Hazel/Events/ApplicationEvents.h"
#include "Hazel/Log.h"
#include "Hazel/Input.h"

#include <Glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	Application::Application() {
		m_TextureDict.clear();
		HZ_CORE_ASSERT(!s_Instance,"Application全局应该只有一个,但这里不为空.")
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		//先向Window实例注册事件回调函数
		//Window实例再向WindowData实例注册回调函数
		//WindowsWindow在实例化时向glfw层注册了很多回调函数
		//这些回调函数最终会走到Application的Event中来
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] = {
			 // 顶点              // 颜色             // UV坐标
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		uint32_t indices[2*3] = { 
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		CreateTexture("ShaderSource/static/container.jpg",true);
		CreateTexture("ShaderSource/static/awesomeface.png",false);

		m_Shader.reset(new Shader("ShaderSource/Chapter2/vertex.vs","ShaderSource/Chapter2/fragment.fs"));
		m_Shader->Bind();
		//index和texture映射起来
		m_Shader->setInt("texture1", 0);
		m_Shader->setInt("texture2", 1);
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	uint32_t Application::CreateTexture(const std::string& path,bool isRGB= true)
	{
		Texture* texture = GetTexture(path);
		if (texture == nullptr) {
			texture = new Texture(path,isRGB);
			m_TextureDict.insert({path, texture });
		}
		return texture->GetID();
	}

	Texture* Application::GetTexture(const std::string& path) const
	{
		auto iter = m_TextureDict.find(path);
		if (iter == m_TextureDict.end()) {
			return nullptr;
		}
		return iter->second;
	}

	void Application::Run() {

		while (m_Running)
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// bind Texture
			if (GetTexture("ShaderSource/static/container.jpg")) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, GetTexture("ShaderSource/static/container.jpg")->GetID());
			}
			if (GetTexture("ShaderSource/static/awesomeface.png")) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, GetTexture("ShaderSource/static/awesomeface.png")->GetID());
			}

			// create transformations
			glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
			transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			m_Shader->Bind();

			//先找到uniform地址
			unsigned int transformLoc = glGetUniformLocation(m_Shader->GetID(), "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			glBindVertexArray(m_VertexArray);
			//表明我们想要绘制的是三角
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//HZ_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}