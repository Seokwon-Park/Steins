#pragma once

#include "Steins/Window.h"

#include <GLFW/glfw3.h>

#include <d3dcompiler.h>
#include <d3d11.h>

#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

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
	private:
		// for GLFW
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_glfwWindow;

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