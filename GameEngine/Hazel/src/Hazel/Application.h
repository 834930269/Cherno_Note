#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Windows.h"
#include "LayerStack.h"
#include "Events/ApplicationEvents.h"

namespace Hazel
{
	//ʹ��HAZEL_API�����dllimport��dllexport�������Ӵ���ĸ�����
	//���������ɳ�л�����ʹ��dllimport
	//����HazelEngine������ʹ��dllexport
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

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//Ӧ���ɿͻ��˶���
	Application* CreateApplication();
}