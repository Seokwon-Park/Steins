#include "stspch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_dx11.h"

#include "Steins/Core/Application.h"
#include "Steins/Renderer/Renderer.h"
#include "Steins/Renderer/GraphicsContext.h"
#include "Platform/DirectX11/D3D11Context.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

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
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			auto device = static_cast<D3D11Context*>(m_Context)->GetD3DDevice();
			auto context = static_cast<D3D11Context*>(m_Context)->GetD3DContext();
			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX11_Init(device.Get(), context.Get());
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

	void ImGuiLayer::Begin()
	{
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
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//Rendering
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::Direct3D11:
		{
			ImGui::Render();
			ID3D11RenderTargetView* rtvs[] =
			{
				dynamic_cast<D3D11Context*>(m_Context)->GetRTV().Get()
			};
			dynamic_cast<D3D11Context*>(m_Context)->GetD3DContext()->OMSetRenderTargets(1, rtvs, nullptr);
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

}