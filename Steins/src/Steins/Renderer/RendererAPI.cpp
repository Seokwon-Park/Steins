#include "stspch.h"
#include "RendererAPI.h"

namespace Steins
{
#if RENDER_API == 0
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#elif RENDER_API == 1
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Direct3D11;
#endif
}