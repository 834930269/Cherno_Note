#pragma once

//注意,这点使用的思维是文件层面的解耦合

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel:: CreateApplication();

/// <summary>
/// 将main函数和Sandbox解耦和
/// 不然每次编译都需要把HazelEngine.dll拉到Sandbox项目里
/// 解耦合以后,Sandbox就只需要引用Hazel.h
/// 然后实现CreateApplication即可
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
