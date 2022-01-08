#pragma once

#include "Core.h"

namespace Hazel
{
	//使用HAZEL_API宏代替dllimport和dllexport可以增加代码的复用性
	//比如可以在沙盒环境下使用dllimport
	//而在HazelEngine环境下使用dllexport
	class HAZEL_API Application {
	public:
		Application();
		//因为是沙盒,所以析构函数设为虚函数
		virtual ~Application();

		void Run();
	};

	//应该由客户端定义
	Application* CreateApplication();
}