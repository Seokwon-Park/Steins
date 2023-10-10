#pragma once

#include "Steins/Renderer/RendererAPI.h"
#include "D3D11Context.h"

namespace Steins
{
	class D3D11RendererAPI : public RendererAPI
	{
	public:
		virtual void Init(GraphicsContext* context) override;
		virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	private:
		float m_ClearColor[4];
		D3D11Context* m_Context;
	};
}