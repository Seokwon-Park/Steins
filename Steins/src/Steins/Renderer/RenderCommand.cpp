#include "stspch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX11/D3D11RendererAPI.h"

namespace Steins
{
#if RENDER_API	== 0
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
#elif RENDER_API == 1
	RendererAPI* RenderCommand::s_RendererAPI = new D3D11RendererAPI;
#endif
}