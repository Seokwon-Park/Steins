#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "Steins/Events/Event.h"
#include "Steins/Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Steins/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Steins {
	class Application
	{
	public:
		Application(const std::string& name = "Steins App");
		virtual ~Application();


		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void UI_DrawTitlebar(float& outTitlebarHeight);

		GraphicsContext* m_Context;
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;			
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Application* CreateApplication();
}

