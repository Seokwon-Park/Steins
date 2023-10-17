#pragma once

#include "Steins/Core/Layer.h"

#include "Steins/Events/KeyEvent.h"
#include "Steins/Events/MouseEvent.h"
#include "Steins/Events/ApplicationEvent.h"

#include "Steins/Renderer/GraphicsContext.h"

namespace Steins
{
	class  ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(GraphicsContext* context);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents= true;
		float m_Time = 0.0f;
		GraphicsContext* m_Context;
	};
}
