#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "Steins/Events/Event.h"
#include "Steins/Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Steins/ImGui/ImGuiLayer.h"

namespace Steins {
	class Application
	{
	public:
		Application(const std::string& name = "Steins App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		GraphicsContext* m_Context;
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;			
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}

