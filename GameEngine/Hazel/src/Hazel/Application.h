#pragma once

#include "Core.h"

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
	};

	//Ӧ���ɿͻ��˶���
	Application* CreateApplication();
}