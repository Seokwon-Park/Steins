#include "stspch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Steins {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			STS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		/*case RendererAPI::API::Direct3D11:
			return new D3D11VertexArray();*/
		}

		STS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}