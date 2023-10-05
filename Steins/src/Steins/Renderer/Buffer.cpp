#include "stspch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Steins
{
	VertexBuffer* VertexBuffer::Create(float* vertices, u32 size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			STS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}

		STS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(u32* indices, u32 size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			STS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}

		STS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}