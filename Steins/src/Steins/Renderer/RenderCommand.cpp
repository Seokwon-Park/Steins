#include "stspch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX11/D3D11RendererAPI.h"

namespace Steins
{
#if APITYPE	== 0
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
#elif APITYPE == 1
	RendererAPI* RenderCommand::s_RendererAPI = new D3D11RendererAPI;
#endif

	//void RenderCommand::SetAPI()
	//{
	//	switch (RendererAPI::GetAPI())
	//	{
	//		case RendererAPI::API::OpenGL:
	//		{
	//			RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
	//			break;
	//		}
	//		case RendererAPI::API::Direct3D11:
	//		{
	//			RenderCommand::s_RendererAPI = new D3D11RendererAPI;
	//			break;
	//		}
	//		break;
	//	}
	//}
}