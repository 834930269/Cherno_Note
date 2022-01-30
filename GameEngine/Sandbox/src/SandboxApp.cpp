#include<Hazel.h>

#include "imgui/imgui.h"

class ExampleLayer :public Hazel::Layer {
public:
	ExampleLayer() : Layer("Example"){

	}
	void OnUpdate() override {

	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Test");
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override {
		if (event.GetEventType() == Hazel::EventType::KeyPressed) {
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if(e.GetKeyCode()==HZ_KEY_TAB)
				HZ_TRACE("Tab is pressed!");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

///在这里实现EntryPoint里的入口函数
Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}