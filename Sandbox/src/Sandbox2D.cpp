#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Snadbox2D"), m_CameraController(16.f / 9.f)
{
}

void Sandbox2D::OnAttach()
{
	STS_PROFILE_FUNCTION();

	m_CheckerboardTexture = Steins::Texture2D::Create("assets/textures/Checkerboard2.png");
	m_LogoTexture = Steins::Texture2D::Create("assets/textures/Logo.png");
}
void Sandbox2D::OnDetach()
{
	STS_PROFILE_FUNCTION();

}


void Sandbox2D::OnUpdate(Steins::Timestep dt)
{
	STS_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	Steins::Renderer2D::ResetStats();
	{
		STS_PROFILE_SCOPE("Renderer Prep");
		Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Steins::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += 20.0f * dt;

		STS_PROFILE_SCOPE("Renderer Draw");

		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Steins::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f }, { .8f,.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Steins::Renderer2D::DrawQuad({ -1.0f,0.0f }, { .8f,.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { .5f,0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.0f, 0.0f, -.1f }, { 20.0f,20.0f }, m_CheckerboardTexture, 10.0f);
		Steins::Renderer2D::DrawRotatedQuad({ -2.0f,-0.0f }, { 1.0f,1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		Steins::Renderer2D::EndScene();

		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y+5.0f)/10.0f, 0.7f};
				Steins::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}

		}
		Steins::Renderer2D::EndScene();
	}

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Steins::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Steins::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	STS_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Steins::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Steins::Event& e)
{
	m_CameraController.OnEvent(e);
}
