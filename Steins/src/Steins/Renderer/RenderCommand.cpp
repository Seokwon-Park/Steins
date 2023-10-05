#include "stspch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Steins
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}