#pragma once

#include "RendererAPI.h"

namespace Steins
{
	class RenderCommand
	{
	public:
		inline static void Init(GraphicsContext* context)
		{
			STS_PROFILE_FUNCTION();

			s_RendererAPI->Init(context);
		}

		inline static void SetViewport(u32 x, u32 y, u32 width, u32 height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, u32 vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}
		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}


	private:
		static RendererAPI* s_RendererAPI;
	};
}