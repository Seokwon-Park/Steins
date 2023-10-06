#include "stspch.h"
#include "D3D11RendererAPI.h"
#include "D3D11Context.h"

namespace Steins
{
	void D3D11RendererAPI::Init(GraphicsContext* context)
	{
		m_Context = dynamic_cast<D3D11Context*>(context);
	}

	void D3D11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		m_ClearColor[0] = color.r;
		m_ClearColor[1] = color.g;
		m_ClearColor[2] = color.b;
		m_ClearColor[3] = color.a;
	}
	void D3D11RendererAPI::Clear()
	{
		m_Context->GetD3DContext()->ClearRenderTargetView(m_Context->GetRTV().Get(), m_ClearColor);
		m_Context->GetD3DContext()->ClearDepthStencilView(m_Context->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void D3D11RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
	}
}