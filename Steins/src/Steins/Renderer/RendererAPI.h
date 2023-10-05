#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "GraphicsContext.h"

namespace Steins
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			Direct3D11 = 2,
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		void SetContext(GraphicsContext* context) { m_Context = context; }
		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
		GraphicsContext* m_Context;
	};
}
