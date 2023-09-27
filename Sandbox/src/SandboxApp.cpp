#include <Steins.h>

class ExampleLayer : public Steins::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

	}

	void OnUpdate() override
	{
		STS_INFO("ExampleLayer::Update");
	}

	void OnEvent(Steins::Event& event) override
	{
		STS_TRACE("{0}", event);
	}
};

class Sandbox : public Steins::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Steins::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Steins::Application* Steins::CreateApplication()
{
	return new Sandbox();
}