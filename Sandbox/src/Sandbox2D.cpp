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
	m_SpriteSheet = Steins::Texture2D::Create("game/textures/RPGpack_sheet_2X.png");

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
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

#if 0
	{
		static float rotation = 0.0f;
		rotation += dt * 50.0f;

		STS_PROFILE_SCOPE("Renderer Draw");

		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Steins::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f, -.1f }, { .8f,.8f }, glm::radians(-45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
		Steins::Renderer2D::DrawQuad({ -1.0f,0.0f, -.1f}, { .8f,.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.5f,-0.5f, -.1f}, { .5f,0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.0f, 0.0f, -.2f}, { 20.0f,20.0f }, m_CheckerboardTexture, 10.0f);
		Steins::Renderer2D::DrawRotatedQuad({ -2.0f,-0.0f, -.1f }, { 1.0f,1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
		Steins::Renderer2D::EndScene();

		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y+5.0f)/10.0f, 0.7f};
				Steins::Renderer2D::DrawQuad({ x, y , -.1f}, { 0.45f, 0.45f }, color);
			}
		}
		Steins::Renderer2D::EndScene();
	}
#endif

	if (Steins::Input::IsMouseButtonPressed(STS_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Steins::Input::GetMousePosition();
		auto width = Steins::Application::Get().GetWindow().GetWidth();
		auto height = Steins::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(dt);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Steins::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, m_SpriteSheet);
	Steins::Renderer2D::EndScene();	
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
