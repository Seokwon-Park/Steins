#pragma once

#include "RendererAPI.h"

namespace Steins
{
	class RenderCommand
	{
	public:
		inline static void Init(GraphicsContext* context)
		{
			s_RendererAPI->Init(context);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		//static void SetAPI();

	private:
		static RendererAPI* s_RendererAPI;
	};
}