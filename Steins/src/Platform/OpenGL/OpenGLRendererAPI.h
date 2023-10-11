#pragma once

#include "Steins/Renderer/RendererAPI.h"

namespace Steins
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init(GraphicsContext* context) override;
		virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 indexCount) override;
	};
}
