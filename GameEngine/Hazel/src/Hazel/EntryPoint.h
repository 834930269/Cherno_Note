#pragma once

//ע��,���ʹ�õ�˼ά���ļ�����Ľ����

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

/// <summary>
/// ��main������Sandbox�����
/// ������Ժ�,Sandbox��ֻ��Ҫʵ�ִ���Ӧ��,������Ҫʵ����������
/// Ȼ��ʵ��CreateApplication����
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc,char** argv) {
	Hazel::Log::Init();
	HZ_CORE_WARN("��ʼ��Log!");
	HZ_INFO("���! Var={0}",5);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif
