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

	m_CheckerboardTexture = Steins::Texture2D::Create("assets/textures/Checkerboard.png");
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
	{
		STS_PROFILE_SCOPE("Renderer Prep");
		Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Steins::RenderCommand::Clear();
	}

	{
		STS_PROFILE_SCOPE("Renderer Draw");

		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Steins::Renderer2D::DrawQuad({ -1.0f,0.0f }, { .8f,.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { .5f,0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.0f,0.0f , -0.1f }, { 10.0f,10.0f }, m_CheckerboardTexture);
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
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Steins::Event& e)
{
	m_CameraController.OnEvent(e);
}
