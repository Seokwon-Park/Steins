#pragma once

#include "Core.h"

#include "Window.h"
#include "Steins/LayerStack.h"
#include "Steins/Events/Event.h"
#include "Steins/Events/ApplicationEvent.h"

#include "Steins/ImGui/ImGuiLayer.h"

#include "Steins/Renderer/Shader.h"
#include "Steins/Renderer/Buffer.h"
#include "Steins/Renderer/VertexArray.h"

#include "Steins/Renderer/OrthographicCamera.h"

namespace Steins {
	class STEINS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}

