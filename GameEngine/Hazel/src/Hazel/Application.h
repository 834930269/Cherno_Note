#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Windows.h"
#include "LayerStack.h"
#include "Events/ApplicationEvents.h"
#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
	//使用HAZEL_API宏代替dllimport和dllexport可以增加代码的复用性
	//比如可以在沙盒环境下使用dllimport
	//而在HazelEngine环境下使用dllexport
	//且是一个单例
	class HAZEL_API Application {
	public:
		Application();
		//因为是沙盒,所以析构函数设为虚函数
		virtual ~Application();

		void Run();
#pragma region Layer相关
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
#pragma endregion
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	//应该由客户端定义
	Application* CreateApplication();
}