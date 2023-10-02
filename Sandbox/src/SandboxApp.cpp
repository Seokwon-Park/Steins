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
		if (Steins::Input::IsKeyPressed(STS_KEY_TAB))
		{
			STS_TRACE("Tab key is pressed (poll)!");
		}
	}

	void OnEvent(Steins::Event& event) override
	{
		if (event.GetEventType() == Steins::EventType::KeyPressed)
		{
			Steins::KeyPressedEvent& e = (Steins::KeyPressedEvent&)event;
			if (e.GetKeyCode() == STS_KEY_TAB)
			{
				STS_TRACE("Tab key is pressed (event)!");
			}
			STS_TRACE("{0}", (char)e.GetKeyCode());
		}
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