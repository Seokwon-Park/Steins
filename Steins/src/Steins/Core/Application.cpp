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

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		STS_PROFILE_FUNCTION();

		STS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(STS_BIND_EVENT_FN(OnEvent));

		m_Context = m_Window->GetContext();
		Renderer::Init(m_Context);

		m_ImGuiLayer = new ImGuiLayer(m_Window->GetContext());
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{
		STS_PROFILE_FUNCTION();

		Renderer::Shutdown();
		if (Renderer::GetAPI() == RendererAPI::API::Direct3D11)
		{
			D3D11Context* m_Context = static_cast<D3D11Context*>(s_Instance->GetWindow().GetContext());
			m_Context->ReleaseCall();
		}
	}

	void Application::OnEvent(Event& e)
	{
		STS_PROFILE_FUNCTION();

		//STS_CORE_INFO("{0}", e);
		//STS_CORE_INFO("{0}, {1}", m_Window->GetWidth(), m_Window->GetHeight());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(STS_BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(STS_BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		STS_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		STS_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		STS_PROFILE_FUNCTION();

		while (m_Running)
		{
			float time = (float)glfwGetTime(); // Platform::GetTime
			Timestep timestep = ImGui::GetIO().DeltaTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					STS_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}
			m_ImGuiLayer->Begin();
			{
				STS_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		STS_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		if (Renderer::GetAPI() == RendererAPI::API::Direct3D11)
		{
			D3D11Context* m_Context = static_cast<D3D11Context*>(s_Instance->GetWindow().GetContext());
			m_Context->ResizeSwapChain(e.GetWidth(), e.GetHeight());
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}