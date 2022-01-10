#pragma once

//注意,这点使用的思维是文件层面的解耦合

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

/// <summary>
/// 将main函数和Sandbox解耦和
/// 解耦合以后,Sandbox就只需要实现创建应用,而不需要实现启动流程
/// 然后实现CreateApplication即可
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc,char** argv) {
	Hazel::Log::Init();
	HZ_CORE_WARN("初始化Log!");
	HZ_INFO("你好! Var={0}",5);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif
