#include "stspch.h"
#include "RendererAPI.h"

namespace Steins
{
#if APITYPE == 0
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#elif APITYPE == 1
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Direct3D11;
#endif
}