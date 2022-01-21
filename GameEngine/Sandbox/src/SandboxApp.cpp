#include<Hazel.h>

class ExampleLayer :public Hazel::Layer {
public:
	ExampleLayer() : Layer("Example"){}
	void OnUpdate() override {
		HZ_INFO("ExampleLayer::Update");
	}
	void OnEvent(Hazel::Event& event) override {
		HZ_TRACE("{0}", event);
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