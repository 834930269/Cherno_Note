#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/ApplicationEvents.h"
#include "Hazel/Log.h"

namespace Hazel {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);

		if (e.IsInCategory(EventCategoryApplication)) {
			HZ_TRACE("����Ӧ���鷢������Ϣ {0}", e);
		}
		if (e.IsInCategory(EventCategoryInput)) {
			HZ_TRACE("������������鷢������Ϣ {0}", e);
		}

		while (true);
	}
}