#include "stspch.h"
#include "WindowsWindow.h"

#include "Steins/Events/KeyEvent.h"
#include "Steins/Events/MouseEvent.h"
#include "Steins/Events/ApplicationEvent.h"

#include "Steins/Renderer/Renderer.h"
#include "Steins/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX11/D3D11Context.h"

namespace Steins
{
	namespace
	{
		static bool s_GLFWInitialized = false;

		static void GLFWErrorCallback(int error, const char* description)
		{
			STS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		}
	}//anonymous namespace

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		STS_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		STS_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		STS_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		STS_CORE_INFO("Create Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//glfw
		if (!s_GLFWInitialized)
		{
			STS_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			STS_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		{
			STS_PROFILE_SCOPE("glfwCreateWindow");
			m_glfwWindow = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			m_Context = new D3D11Context(m_glfwWindow, props);
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			m_Context = new OpenGLContext(m_glfwWindow);
			break;
		}
		STS_CORE_ASSERT(false, "API::None currently not supported");
		return;
		}
		m_Context->Init();
		// ^

		glfwSetWindowUserPointer(m_glfwWindow, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_glfwWindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_glfwWindow, [](GLFWwindow* window, u32 keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		STS_PROFILE_FUNCTION();

		glfwDestroyWindow(m_glfwWindow);

	}

	void WindowsWindow::OnUpdate()
	{
		STS_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		STS_PROFILE_FUNCTION();

		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}