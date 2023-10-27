#pragma once

#include "Steins/Core/Window.h"
#include "Steins/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Steins
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline std::string GetTitle() const override { return m_Data.Title; }
		inline u32 GetWidth() const override { return m_Data.Width; }
		inline u32 GetHeight() const override { return m_Data.Height; }

		//Window attr
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		// rty3394 - �ڵ忡 ������ ����� ���� �� �߸� �� ����;
		inline GraphicsContext* GetContext() const { return m_Context; }
		///////////

		inline virtual void* GetNativeWindow() const { return m_glfwWindow; }
	private:
		// for GLFW
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_glfwWindow;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			u32 Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}