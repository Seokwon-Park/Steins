#include "stspch.h"
#include "WindowsWindow.h"

#include "Steins/Events/KeyEvent.h"
#include "Steins/Events/MouseEvent.h"
#include "Steins/Events/ApplicationEvent.h"

namespace Steins
{

	namespace
	{
		static bool s_GLFWInitialized = false;

		static void GLFWErrorCallback(int error , const char* description)
		{
			STS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		}

		WindowsWindow* g_WindowsBase = nullptr;

		// RegisterClassEX()에 실제로 등록될 콜백함수
		LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			// g_appBase를 이용해서 간접적으로 멤버 함수 호출
			return g_WindowsBase->MsgProc(hwnd, msg, wParam, lParam);
		}

	}//anonymous namespace

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		std::wstring title(m_Data.Title.begin(), m_Data.Title.end());

		STS_CORE_INFO("Create Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//if (!InitWindow(props))
		//{			
		//	STS_CORE_ASSERT(InitWindow(props), "Failed to Initialize Window");
		//	return;
		//}

		//glfw
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			STS_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_glfwWindow = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_glfwWindow);
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
		glfwDestroyWindow(m_glfwWindow);

	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_glfwWindow);
	}
	
	void WindowsWindow::SetVSync(bool enabled)
	{
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

	bool WindowsWindow::InitWindow(const WindowProps& props)
	{
		std::wstring wTitle(props.Title.begin(), props.Title.end());

		WNDCLASSEX wc = { sizeof(WNDCLASSEX),
						CS_CLASSDC,
						WndProc,
						0L,
						0L,
						GetModuleHandle(NULL),
						NULL,
						NULL,
						NULL,
						NULL,
						L"SteinsEngine", // lpszClassName, L-string
						NULL };

		// The RegisterClass function has been superseded by the RegisterClassEx function.
		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
		if (!RegisterClassEx(&wc)) {
			std::cout << "RegisterClassEx() failed.\n";
			return false;
		}

		// 원하는 크기의 해상도(툴바 제외)
		RECT windowRect = { 0, 0, props.Width, props.Height };

		// 필요한 윈도우 크기(해상도) 계산 (툴바를 포함)
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

		// 윈도우를 만들때 위에서 계산한 windowRect 사용
		m_Window = CreateWindow(
			wc.lpszClassName,
			wTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			//윈도우의 생성 위치
			100,                // 윈도우 좌측 상단의 x 좌표
			100,                // 윈도우 좌측 상단의 y 좌표

			// 툴바를 포함한 실제 윈도우의 크기 계산
			windowRect.right - windowRect.left, // 윈도우 가로 방향 해상도
			windowRect.bottom - windowRect.top, // 윈도우 세로 방향 해상도
			NULL, NULL, wc.hInstance, NULL);

		if (!m_Window) {
			std::cout << "CreateWindow() failed.\n";
			return false;
		}

		// 윈도우 표시
		ShowWindow(m_Window, SW_SHOWDEFAULT);
		UpdateWindow(m_Window);

		return true;
	}

	LRESULT WindowsWindow::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		//if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		//	return true;

		switch (msg) {
		case WM_SIZE:
			break;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_MOUSEMOVE:
			//std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;

			break;
		case WM_LBUTTONDOWN:
			std::cout << "WM_LBUTTONUP Left mouse button down\n";
			
			break;
		case WM_LBUTTONUP:
			std::cout << "WM_LBUTTONUP Left mouse button up\n";
			
			break;
		case WM_RBUTTONDOWN:
			std::cout << "WM_RBUTTONUP Right mouse button\n";
			
			break;
		case WM_RBUTTONUP:
			std::cout << "WM_RBUTTONUP Right mouse button\n";
			
			break;
		case WM_KEYDOWN:
			std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;		
		
			break;
		case WM_KEYUP:
		
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}

		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

}