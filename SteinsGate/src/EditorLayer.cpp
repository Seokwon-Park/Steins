#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Steins/Scene/SceneSerializer.h"
#include "Steins/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Steins/Math/Math.h"

namespace Steins
{
	namespace
	{
		static void RenderWindowOuterBorders(ImGuiWindow* window)
		{
			struct ImGuiResizeBorderDef
			{
				ImVec2 InnerDir;
				ImVec2 SegmentN1, SegmentN2;
				float  OuterAngle;
			};

			static const ImGuiResizeBorderDef resize_border_def[4] =
			{
				{ ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f }, // Left
				{ ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f }, // Right
				{ ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f }, // Up
				{ ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f }  // Down
			};

			auto GetResizeBorderRect = [](ImGuiWindow* window, int border_n, float perp_padding, float thickness)
				{
					ImRect rect = window->Rect();
					if (thickness == 0.0f)
					{
						rect.Max.x -= 1;
						rect.Max.y -= 1;
					}
					if (border_n == ImGuiDir_Left) { return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding); }
					if (border_n == ImGuiDir_Right) { return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness, rect.Max.y - perp_padding); }
					if (border_n == ImGuiDir_Up) { return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding, rect.Min.y + thickness); }
					if (border_n == ImGuiDir_Down) { return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y + thickness); }
					IM_ASSERT(0);
					return ImRect();
				};


			ImGuiContext& g = *GImGui;
			float rounding = window->WindowRounding;
			float border_size = 1.0f; // window->WindowBorderSize;
			if (border_size > 0.0f && !(window->Flags & ImGuiWindowFlags_NoBackground))
				window->DrawList->AddRect(window->Pos, { window->Pos.x + window->Size.x,  window->Pos.y + window->Size.y }, ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);

			int border_held = window->ResizeBorderHeld;
			if (border_held != -1)
			{
				const ImGuiResizeBorderDef& def = resize_border_def[border_held];
				ImRect border_r = GetResizeBorderRect(window, border_held, rounding, 0.0f);
				ImVec2 p1 = ImLerp(border_r.Min, border_r.Max, def.SegmentN1);
				const float offsetX = def.InnerDir.x * rounding;
				const float offsetY = def.InnerDir.y * rounding;
				p1.x += 0.5f + offsetX;
				p1.y += 0.5f + offsetY;

				ImVec2 p2 = ImLerp(border_r.Min, border_r.Max, def.SegmentN2);
				p2.x += 0.5f + offsetX;
				p2.y += 0.5f + offsetY;

				window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
				window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
				window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0, ImMax(2.0f, border_size)); // Thicker than usual
			}
			if (g.Style.FrameBorderSize > 0 && !(window->Flags & ImGuiWindowFlags_NoTitleBar) && !window->DockIsActive)
			{
				float y = window->Pos.y + window->TitleBarHeight() - 1;
				window->DrawList->AddLine(ImVec2(window->Pos.x + border_size, y), ImVec2(window->Pos.x + window->Size.x - border_size, y), ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
			}
		}
		// From imgui_widgets.cpp BeginMenuBar(), EndMenuBar()
		// (remove check ImGuiWindowFlags_Menubar) for custom titlebar
		bool BeginMenubar(const ImRect& barRectangle)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;
			/*if (!(window->Flags & ImGuiWindowFlags_MenuBar))
				return false;*/

			IM_ASSERT(!window->DC.MenuBarAppending);
			ImGui::BeginGroup(); // Backup position on layer 0 // FIXME: Misleading to use a group for that backup/restore
			ImGui::PushID("##menubar");

			const ImVec2 padding = window->WindowPadding;

			// We don't clip with current window clipping rectangle as it is already set to the area below. However we clip with window full rect.
			// We remove 1 worth of rounding to Max.x to that text in long menus and small windows don't tend to display over the lower-right rounded area, which looks particularly glitchy.
			ImRect bar_rect = barRectangle;// window->MenuBarRect();
			ImRect clip_rect(IM_ROUND(ImMax(window->Pos.x, bar_rect.Min.x + window->WindowBorderSize + window->Pos.x - 10.0f)), IM_ROUND(bar_rect.Min.y + window->WindowBorderSize + window->Pos.y),
				IM_ROUND(ImMax(bar_rect.Min.x + window->Pos.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))), IM_ROUND(bar_rect.Max.y + window->Pos.y));

			clip_rect.ClipWith(window->OuterRectClipped);
			ImGui::PushClipRect(clip_rect.Min, clip_rect.Max, false);

			// We overwrite CursorMaxPos because BeginGroup sets it to CursorPos (essentially the .EmitItem hack in EndMenuBar() would need something analogous here, maybe a BeginGroupEx() with flags).
			window->DC.CursorPos = window->DC.CursorMaxPos = ImVec2(bar_rect.Min.x + window->Pos.x, bar_rect.Min.y + window->Pos.y);
			window->DC.LayoutType = ImGuiLayoutType_Horizontal;
			window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
			window->DC.MenuBarAppending = true;
			ImGui::AlignTextToFramePadding();
			return true;
		}

		void EndMenubar()
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return;
			ImGuiContext& g = *GImGui;

			// Nav: When a move request within one of our child menu failed, capture the request to navigate among our siblings.
			if (ImGui::NavMoveRequestButNoResultYet() && (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) && (g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu))
			{
				// Try to find out if the request is for one of our child menu
				ImGuiWindow* nav_earliest_child = g.NavWindow;
				while (nav_earliest_child->ParentWindow && (nav_earliest_child->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
					nav_earliest_child = nav_earliest_child->ParentWindow;
				if (nav_earliest_child->ParentWindow == window && nav_earliest_child->DC.ParentLayoutType == ImGuiLayoutType_Horizontal && (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0)
				{
					// To do so we claim focus back, restore NavId and then process the movement request for yet another frame.
					// This involve a one-frame delay which isn't very problematic in this situation. We could remove it by scoring in advance for multiple window (probably not worth bothering)
					const ImGuiNavLayer layer = ImGuiNavLayer_Menu;
					IM_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer)); // Sanity check
					ImGui::FocusWindow(window);
					ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);
					g.NavDisableHighlight = true; // Hide highlight for the current frame so we don't see the intermediary selection.
					g.NavDisableMouseHover = g.NavMousePosDirty = true;
					ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags); // Repeat
				}
			}

			IM_MSVC_WARNING_SUPPRESS(6011); // Static Analysis false positive "warning C6011: Dereferencing NULL pointer 'window'"
			// IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar); // NOTE(Yan): Needs to be commented out because Jay
			IM_ASSERT(window->DC.MenuBarAppending);
			ImGui::PopClipRect();
			ImGui::PopID();
			window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x; // Save horizontal position so next append can reuse it. This is kinda equivalent to a per-layer CursorPos.
			g.GroupStack.back().EmitItem = false;
			ImGui::EndGroup(); // Restore position on layer 0
			window->DC.LayoutType = ImGuiLayoutType_Vertical;
			window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
			window->DC.MenuBarAppending = false;
		}
	}
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		:Layer("Snadbox2D"), m_CameraController(16.f / 9.f)
	{
	}

	void EditorLayer::OnAttach()
	{
		STS_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard2.png");
		m_LogoTexture = Texture2D::Create("Resources/Icons/Logo.png");
		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
		// Entity
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		// Entity
		auto square2 = m_ActiveScene->CreateEntity("Red Square");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;
		m_SquareEntity2 = square2;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep dt)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					translation.x -= speed * dt;
				if (Input::IsKeyPressed(Key::D))
					translation.x += speed * dt;
				if (Input::IsKeyPressed(Key::W))
					translation.y += speed * dt;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= speed * dt;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OnDetach()
	{
		STS_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep dt)
	{
		STS_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);

		// Resize
#if RENDER_API == 0
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
#endif
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);
		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Steins::Renderer2D::DrawQuad({ -1.0f,0.0f, -.05f }, { .8f,.8f }, m_SquareColor);
		//Renderer2D::EndScene();

		// Update
		// Update scene
		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			if (m_ViewportFocused)
			{
				m_CameraController.OnUpdate(dt);
			}
			m_EditorCamera.OnUpdate(dt);

			m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActiveScene->OnUpdateRuntime(dt);
			break;
		}
		}

#if RENDER_API == 0
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
#else
		//auto [mx, my] = ImGui::GetMousePos();
		//mx -= m_DockSpacePos.x;
		//my -= m_DockSpacePos.y;
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		//STS_CORE_WARN("Mouse Coord = {0}, {1}", mx, my);

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		//my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
#endif
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		STS_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Application::Get().IsMaximized() ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		//m_DockSpacePos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		//STS_CORE_WARN("Dock Space Coord = {0}, {1}", m_DockSpacePos.x, m_DockSpacePos.y);
		ImGui::PopStyleColor(); // MenuBarBg
		ImGui::PopStyleVar(2);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		RenderWindowOuterBorders(ImGui::GetCurrentWindow());

		float titleBarHeight;
		UI_DrawTitlebar(titleBarHeight);
		ImGui::SetCursorPosY(titleBarHeight);

		// STS_CORE_TRACE("{0}, {1}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		// STS_CORE_TRACE("{0}, {1}", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos(); // Includes tab bar(height 21)
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x ,viewportPanelSize.y };

#if RENDER_API	== 0
		auto textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x ,m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
#elif RENDER_API == 1
		auto textureID = m_Framebuffer->GetSRV(0);
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x ,m_ViewportSize.y });
#endif

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			// Runtime camera from entity
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4 cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // snap to 0.5m for translation/scale
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(STS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(STS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
				NewScene();
			break;
		}
		case Key::O:
		{
			if (control)
				OpenScene();
			break;
		}
		case Key::S:
		{
			if (control)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}
			break;
		}

		// Scene Commands
		case Key::D:
		{
			if (control)
				OnDuplicateEntity();
			break;
		}

		case Key::Q:
		{
			m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}

	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}
	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			//BoxColliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 0.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			//CircleColliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}
		Renderer2D::EndScene();
	}
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Steins Scene (*.steins)\0*.steins\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}
	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".steins")
		{
			STS_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_ActiveScene->OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}
	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		}
		else
			SaveSceneAs();
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Steins Scene (*.steins)\0*.steins\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}
	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}
	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_EditorScene);
	}
	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;

		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_EditorScene);
	}
	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}
	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiViewportFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
#if RENDER_API == 0
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
#elif RENDER_API == 1
		if (ImGui::ImageButton((ImTextureID)icon->GetSRV(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
#endif
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::UI_DrawTitlebar(float& outTitlebarHeight)
	{
		auto& app = Application::Get();
		const float titlebarHeight = 58.0f;
		const bool isMaximized = app.IsMaximized();
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									 ImGui::GetCursorScreenPos().y + titlebarHeight };
		auto* bgDrawList = ImGui::GetBackgroundDrawList();
		auto* fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, IM_COL32(21, 21, 21, 255));
		// DEBUG TITLEBAR BOUNDS
		//fgDrawList->AddRect(titlebarMin, titlebarMax, IM_COL32(222, 43, 43, 255));

		// Logo
		{
			const int logoWidth = 36;// m_LogoTex->GetWidth();
			const int logoHeight = 48;// m_LogoTex->GetHeight();
			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
			const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };
#if RENDER_API == 0
			fgDrawList->AddImage((ImTextureID)m_LogoTexture->GetRendererID(), logoRectStart, logoRectMax, ImVec2(0, 1), ImVec2(1, 0));
#elif RENDER_API == 1
			fgDrawList->AddImage((ImTextureID)m_LogoTexture->GetSRV(), logoRectStart, logoRectMax, ImVec2(0, 1), ImVec2(1, 0));
#endif
		}
		
		ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 96;

		// Title bar drag area
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth-20, titlebarHeight), ImGuiButtonFlags_AllowOverlap);

		app.SetTitleBarHovered(ImGui::IsItemHovered());

		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				app.SetTitleBarHovered(true);// Account for the top-most pixels which don't register
		}

		// On Windows we hook into the GLFW win32 window internals
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
		// DEBUG DRAG BOUNDS
		//fgDrawList->AddRect(ImGui::GetCursorScreenPos(), 
		//	 ImVec2(ImGui::GetCursorScreenPos().x + w - buttonsAreaWidth-20, ImGui::GetCursorScreenPos().y + titlebarHeight), IM_COL32(222, 43, 43, 255));
		{
			// Centered Window title
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(app.GetWindow().GetTitle().c_str());
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", app.GetWindow().GetTitle().c_str()); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}
		
		// Window buttons
		const float buttonWidth = 32.0f;
		const float buttonHeight = 28.0f;

		// Minimize Button
		//ImGui::Spring();
		{
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - buttonsAreaWidth, 0.0f));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f });
			if (ImGui::Button("_", ImVec2(buttonWidth, buttonHeight)))
			{
				// Application::Get().QueueEvent([windowHandle = m_WindowHandle]() { glfwIconifyWindow(windowHandle); });
			}
			ImGui::SetCursorPos(currentCursorPos);
		}
		{
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - buttonsAreaWidth + 32.0f, 0.0f));
			if (ImGui::Button("[]", ImVec2(buttonWidth, buttonHeight)))
			{
				//glfwMaximizeWindow((GLFWwindow*)m_Window->GetNativeWindow() );
				//glfwRestoreWindow((GLFWwindow*)m_Window->GetNativeWindow());
			}

			ImGui::SetCursorPos(currentCursorPos);
		}
		{
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - buttonsAreaWidth + 64.0f, 0.0f));

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.8f, 0.3f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.2f, 0.2f, 1.0f });
			if (ImGui::Button("X", ImVec2(buttonWidth, buttonHeight)))
			{
				app.Close();
			}
			ImGui::PopStyleColor(3);
			ImGui::SetCursorPos(currentCursorPos);
		}
		bool menuHovered = false;


		ImGui::SuspendLayout();
		{
			ImGui::SetItemAllowOverlap();
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(70.0f,4.0f));

			const ImRect menuBarRect = { ImGui::GetCursorPos(),
				{ ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x,
				30.0f } };
			ImGui::BeginGroup();
			if (BeginMenubar(menuBarRect))
			{
				if (ImGui::BeginMenu("File"))
				{
					//menuHovered = ImGui::IsItemHovered();
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}

					if (ImGui::MenuItem("Save...", "Ctrl+S"))
					{
						SaveScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						SaveSceneAs();
					}

					ImGui::Separator();
					if (ImGui::MenuItem("Exit"))
					{
						Application::Get().Close();
					}

					ImGui::EndMenu();
				}
			}
			EndMenubar();
			ImGui::EndGroup();
			if (ImGui::IsItemHovered())
				app.SetTitleBarHovered(false);
			ImGui::SetCursorPos(currentCursorPos);
		}
		ImGui::ResumeLayout();

		ImGui::EndHorizontal();
		outTitlebarHeight = titlebarHeight;
	}

}