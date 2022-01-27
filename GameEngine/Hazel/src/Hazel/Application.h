#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Windows.h"
#include "LayerStack.h"
#include "Events/ApplicationEvents.h"
#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include <Hazel/Renderer/Buffer.h>

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

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	//Ӧ���ɿͻ��˶���
	Application* CreateApplication();
}