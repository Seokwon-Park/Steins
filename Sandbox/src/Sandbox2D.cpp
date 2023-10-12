#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const u32 s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDWWWWWWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDWWWDDDDDDDDDDDDWWW"
"WDDDDDWWWDDDDDDDDDDDDDWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWWDDDDDDDDDDDDDDWWWWW"
"WWWWWWDDDDDDDDDDDWWWWWWW"
"WWWWWWWDDDDDDDDDWWWWWWWW"
"WWWWWWWWWWDDDDWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
	:Layer("Snadbox2D"), m_CameraController(16.f / 9.f)
{
}

void Sandbox2D::OnAttach()
{
	STS_PROFILE_FUNCTION();

	m_CheckerboardTexture = Steins::Texture2D::Create("assets/textures/Checkerboard2.png");
	m_SpriteSheet = Steins::Texture2D::Create("game/textures/RPGpack_sheet_2X.png");

	Steins::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height= 720;
	m_Framebuffer = Steins::Framebuffer::Create(fbSpec);

	m_TextureStairs = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0,11 }, { 128,128 });
	m_TextureTree = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 }, { 1,2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = (u32)(strlen(s_MapTiles) / s_MapWidth);

	s_TextureMap['D'] = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6,11}, {128,128});
	s_TextureMap['W'] = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,11}, {128,128});

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f);
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
		m_Framebuffer->Bind();
		Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Steins::RenderCommand::Clear();
	}

#if 1
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
		m_Framebuffer->Unbind();

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
#if 0
	Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (u32 y = 0; y < m_MapHeight; y++)
	{
		for (u32 x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Steins::Ref<Steins::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else
				{
					texture = m_TextureBarrel;
				}
				Steins::Renderer2D::DrawQuad({ x - m_MapWidth/2.0f,  y-m_MapHeight/2.0f, 0.0f }, { 1.0f,1.0f }, texture);

		}
	}
	//Steins::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, m_TextureStairs);
	//Steins::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, m_TextureBarrel);
	//Steins::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f,2.0f }, m_TextureTree);
	Steins::Renderer2D::EndScene();	
#endif
}

void Sandbox2D::OnImGuiRender()
{
	STS_PROFILE_FUNCTION();

	static bool dockingEnabled = false;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Steins::Application::Get().Close();
				}
				//ImGui::Separator();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Steins::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		//auto textureID = m_CheckerboardTexture->GetSRV();
		ImGui::Image((void*)m_CheckerboardTexture->GetSRV(), ImVec2{ 256.0f, 256.0f });
		//auto textureID = m_CheckerboardTexture->GetRendererID();
		//ImGui::Image((void*)&textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");

		auto stats = Steins::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		//auto textureID = m_CheckerboardTexture->GetSRV();
		ImGui::Image((void*)m_CheckerboardTexture->GetSRV(), ImVec2{ 256.0f, 256.0f });
		//auto textureID = m_CheckerboardTexture->GetRendererID();
		//ImGui::Image((void*)&textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Steins::Event& e)
{
	m_CameraController.OnEvent(e);
}
