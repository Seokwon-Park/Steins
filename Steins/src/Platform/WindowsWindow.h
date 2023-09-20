#pragma once

#include "Steins/Window.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline u32 GetWidth() const override { return m_Data.Width; }
		inline u32 GetHeight() const override { return m_Data.Height; }

		//Window attr
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		bool InitWindow(const WindowProps& props);

		// for GLFW
		GLFWwindow* m_glfwWindow;

		//for Win32
		HWND m_Window;

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