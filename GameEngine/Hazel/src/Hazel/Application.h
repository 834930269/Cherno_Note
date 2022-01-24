#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Windows.h"
#include "LayerStack.h"
#include "Events/ApplicationEvents.h"
#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
	//ʹ��HAZEL_API�����dllimport��dllexport�������Ӵ���ĸ�����
	//���������ɳ�л�����ʹ��dllimport
	//����HazelEngine������ʹ��dllexport
	//����һ������
	class HAZEL_API Application {
	public:
		Application();
		//��Ϊ��ɳ��,��������������Ϊ�麯��
		virtual ~Application();

		void Run();
#pragma region Layer���
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

	//Ӧ���ɿͻ��˶���
	Application* CreateApplication();
}