#pragma once

#include "Steins/Core/Layer.h"

#include "Steins/Events/KeyEvent.h"
#include "Steins/Events/MouseEvent.h"
#include "Steins/Events/ApplicationEvent.h"

#include "Steins/Renderer/GraphicsContext.h"

namespace Steins
{
	class STEINS_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(GraphicsContext* context);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
		GraphicsContext* m_Context;
	};
}
