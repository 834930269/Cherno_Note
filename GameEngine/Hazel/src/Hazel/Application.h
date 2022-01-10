#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Windows.h"

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
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//Ӧ���ɿͻ��˶���
	Application* CreateApplication();
}