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

	//void Application::UI_DrawTitlebar(float& outTitlebarHeight)
	//{
	//	const float titlebarHeight = 58.0f;
	//	const bool isMaximized = IsMaximized();
	//	float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
	//	const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

	//	ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
	//	const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
	//	const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
	//								 ImGui::GetCursorScreenPos().y + titlebarHeight };
	//	auto* bgDrawList = ImGui::GetBackgroundDrawList();
	//	auto* fgDrawList = ImGui::GetForegroundDrawList();
	//	bgDrawList->AddRectFilled(titlebarMin, titlebarMax, UI::Colors::Theme::titlebar);
	//	// DEBUG TITLEBAR BOUNDS
	//	// fgDrawList->AddRect(titlebarMin, titlebarMax, UI::Colors::Theme::invalidPrefab);

	//	// Logo
	//	{
	//		const int logoWidth = 48;// m_LogoTex->GetWidth();
	//		const int logoHeight = 48;// m_LogoTex->GetHeight();
	//		const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
	//		const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
	//		const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };
	//		fgDrawList->AddImage(m_AppHeaderIcon->GetDescriptorSet(), logoRectStart, logoRectMax);
	//	}

	//	ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

	//	static float moveOffsetX;
	//	static float moveOffsetY;
	//	const float w = ImGui::GetContentRegionAvail().x;
	//	const float buttonsAreaWidth = 94;

	//	// Title bar drag area
	//	// On Windows we hook into the GLFW win32 window internals
	//	ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
	//	// DEBUG DRAG BOUNDS
	//	// fgDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + w - buttonsAreaWidth, ImGui::GetCursorScreenPos().y + titlebarHeight), UI::Colors::Theme::invalidPrefab);
	//	ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

	//	m_TitleBarHovered = ImGui::IsItemHovered();

	//	if (isMaximized)
	//	{
	//		float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
	//		if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
	//			m_TitleBarHovered = true; // Account for the top-most pixels which don't register
	//	}

	//	// Draw Menubar
	//	if (m_MenubarCallback)
	//	{
	//		ImGui::SuspendLayout();
	//		{
	//			ImGui::SetItemAllowOverlap();
	//			const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
	//			ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
	//			UI_DrawMenubar();

	//			if (ImGui::IsItemHovered())
	//				m_TitleBarHovered = false;
	//		}

	//		ImGui::ResumeLayout();
	//	}

	//	{
	//		// Centered Window title
	//		ImVec2 currentCursorPos = ImGui::GetCursorPos();
	//		ImVec2 textSize = ImGui::CalcTextSize(m_Specification.Name.c_str());
	//		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
	//		ImGui::Text("%s", m_Specification.Name.c_str()); // Draw title
	//		ImGui::SetCursorPos(currentCursorPos);
	//	}

	//	// Window buttons
	//	const ImU32 buttonColN = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
	//	const ImU32 buttonColH = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
	//	const ImU32 buttonColP = UI::Colors::Theme::textDarker;
	//	const float buttonWidth = 14.0f;
	//	const float buttonHeight = 14.0f;

	//	// Minimize Button

	//	ImGui::Spring();
	//	UI::ShiftCursorY(8.0f);
	//	{
	//		const int iconWidth = m_IconMinimize->GetWidth();
	//		const int iconHeight = m_IconMinimize->GetHeight();
	//		const float padY = (buttonHeight - (float)iconHeight) / 2.0f;
	//		if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
	//		{
	//			// TODO: move this stuff to a better place, like Window class
	//			if (m_WindowHandle)
	//			{
	//				Application::Get().QueueEvent([windowHandle = m_WindowHandle]() { glfwIconifyWindow(windowHandle); });
	//			}
	//		}

	//		UI::DrawButtonImage(m_IconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
	//	}


	//	// Maximize Button
	//	ImGui::Spring(-1.0f, 17.0f);
	//	UI::ShiftCursorY(8.0f);
	//	{
	//		const int iconWidth = m_IconMaximize->GetWidth();
	//		const int iconHeight = m_IconMaximize->GetHeight();

	//		const bool isMaximized = IsMaximized();

	//		if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight)))
	//		{
	//			Application::Get().QueueEvent([isMaximized, windowHandle = m_WindowHandle]()
	//				{
	//					if (isMaximized)
	//						glfwRestoreWindow(windowHandle);
	//					else
	//						glfwMaximizeWindow(windowHandle);
	//				});
	//		}

	//		UI::DrawButtonImage(isMaximized ? m_IconRestore : m_IconMaximize, buttonColN, buttonColH, buttonColP);
	//	}

	//	// Close Button
	//	ImGui::Spring(-1.0f, 15.0f);
	//	UI::ShiftCursorY(8.0f);
	//	{
	//		const int iconWidth = m_IconClose->GetWidth();
	//		const int iconHeight = m_IconClose->GetHeight();
	//		if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
	//			Application::Get().Close();

	//		UI::DrawButtonImage(m_IconClose, UI::Colors::Theme::text, UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f), buttonColP);
	//	}

	//	ImGui::Spring(-1.0f, 18.0f);
	//	ImGui::EndHorizontal();

	//	outTitlebarHeight = titlebarHeight;
	//}

}