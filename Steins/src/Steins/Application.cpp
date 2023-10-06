#include "stspch.h"
#include "Application.h"

#include "Steins/Log.h"

#include <glad/glad.h>
#include "Steins/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Steins {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		STS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init(m_Window->GetContext());

		m_ImGuiLayer = new ImGuiLayer(m_Window->GetContext());
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime(); // Platform::GetTime
			Timestep timestep = ImGui::GetIO().DeltaTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}