#include "stspch.h"
#include "ImGuiLayer.h"

#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_dx11.h>

#include "Steins/Core/Application.h"
#include "Steins/Renderer/Renderer.h"
#include "Steins/Renderer/GraphicsContext.h"
#include "Platform/DirectX11/D3D11Context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace Steins
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer"), m_Context(nullptr)
	{
	}

	ImGuiLayer::ImGuiLayer(GraphicsContext* context)
		: Layer("ImGuiLayer"), m_Context(context)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		STS_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.Fonts->AddFontFromFileTTF("assets/fonts/Naver/NanumBarun/NanumBarunGothicBold.ttf", 15.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Naver/NanumBarun/NanumBarunGothic.ttf", 15.0f);


		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			auto device = static_cast<D3D11Context*>(m_Context)->GetD3DDevice();
			auto context = static_cast<D3D11Context*>(m_Context)->GetD3DContext();
			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX11_Init(device.Get(), context.Get());
			device->Release();
			context->Release();
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
			break;
		}
		STS_CORE_ASSERT(false, "API::None currently not supported");
		break;
		}
	}

	void ImGuiLayer::OnDetach()
	{
		STS_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			ImGui_ImplDX11_Shutdown();
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			ImGui_ImplOpenGL3_Shutdown();
			break;
		}
		}
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		STS_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			ImGui_ImplDX11_NewFrame();
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			ImGui_ImplOpenGL3_NewFrame();
			break;
		}
		}
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		STS_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//Rendering
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			ImGui::Render();
			static_cast<D3D11Context*>(m_Context)->GetD3DContext()->OMSetRenderTargets(1, static_cast<D3D11Context*>(m_Context)->GetRTV().GetAddressOf(), nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		}
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

}