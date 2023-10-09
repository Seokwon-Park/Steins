#include "stspch.h"
#include "Application.h"

#include "Log.h"

#include <glad/glad.h>
#include "Steins/Renderer/Renderer.h"
#include "Platform/DirectX11/D3D11Context.h"

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

		m_Context = m_Window->GetContext();
		Renderer::Init(m_Context);
		
		m_ImGuiLayer = new ImGuiLayer(m_Window->GetContext());
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		//STS_CORE_INFO("{0}", e);
		//STS_CORE_INFO("{0}, {1}", m_Window->GetWidth(), m_Window->GetHeight());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

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

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		if (Renderer::GetAPI() == RendererAPI::API::Direct3D11)
		{
			static_cast<D3D11Context*>(m_Window->GetContext())->
				ResizeSwapChain(e.GetWidth(), e.GetHeight());
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;
	}

}