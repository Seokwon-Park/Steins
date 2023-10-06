#include "stspch.h"
#include "D3D11RendererAPI.h"
#include "D3D11Context.h"

namespace Steins
{
	void D3D11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		m_ClearColor = color;
	}
	void D3D11RendererAPI::Clear()
	{
		//m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), m_ClearColor);
		//m_d3dContext->ClearRenderTargetView(m_indexRenderTargetView.Get(), m_ClearColor);
	}
	void D3D11RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
	}
}