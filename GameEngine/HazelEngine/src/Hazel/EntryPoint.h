#pragma once

//ע��,���ʹ�õ�˼ά���ļ�����Ľ����

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel:: CreateApplication();

/// <summary>
/// ��main������Sandbox�����
/// ��Ȼÿ�α��붼��Ҫ��HazelEngine.dll����Sandbox��Ŀ��
/// ������Ժ�,Sandbox��ֻ��Ҫ����Hazel.h
/// Ȼ��ʵ��CreateApplication����
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc,char** argv) {
	printf("Hazel Engine\n");
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif
