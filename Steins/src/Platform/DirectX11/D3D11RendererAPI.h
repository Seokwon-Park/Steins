#pragma once

#include "Steins/Renderer/RendererAPI.h"

namespace Steins
{
	class D3D11RendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	private:
		glm::vec4 m_ClearColor;
	};
}