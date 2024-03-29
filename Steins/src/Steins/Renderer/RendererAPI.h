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
		virtual void Init(GraphicsContext* context) = 0;
		virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, u32 indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, u32 vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}
