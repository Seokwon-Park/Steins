#include "stspch.h"
#include "Texture.h"

#include "Steins/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/DirectX11/D3D11Texture.h"

namespace Steins
{
	Ref<Texture2D> Texture2D::Create(u32 width, u32 height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			STS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		case RendererAPI::API::Direct3D11:
			return CreateRef<D3D11Texture2D>(width, height);
		}

		STS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			STS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		case RendererAPI::API::Direct3D11:
			return CreateRef<D3D11Texture2D>(path);
		}

		STS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
