#include "stspch.h"
#include "D3D11RendererAPI.h"
#include "D3D11Context.h"

#include "Steins/Core/Application.h"

namespace Steins
{
	void D3D11RendererAPI::Init(GraphicsContext* context)
	{
		//m_Context = static_cast<D3D11Context*>(context);
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
		m_Context->GetD3DContext()->OMSetDepthStencilState(m_Context->GetDSS().Get(), 0);
		m_Context->GetD3DContext()->OMSetBlendState(m_Context->GetBS().Get(), m_ClearColor, 0xFFFFFFFF);
	}

	void D3D11RendererAPI::SetViewport(u32 x, u32 y, u32 width, u32 height)
	{
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
		//m_Context->GetD3DContext()->ClearRenderTargetView(m_Context->GetRTV().Get(), m_ClearColor);
		for(auto rtv : m_Context->GetRTVs())
		{
			m_Context->GetD3DContext()->ClearRenderTargetView(rtv.Get(), m_ClearColor);
		}
		m_Context->GetD3DContext()->ClearDepthStencilView(m_Context->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void D3D11RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, u32 indexCount)
	{
		u32 count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		m_Context->GetD3DContext()->OMSetBlendState(m_Context->GetBS().Get(), m_ClearColor, 0xFFFFFFFF);
		//m_Context->GetD3DContext()->OMSetRenderTargets(1, m_Context->GetRTV().GetAddressOf(), m_Context->GetDSV().Get());
		m_Context->GetD3DContext()->OMSetRenderTargets(m_Context->GetRTVs().size(), m_Context->GetRTVs().data()->GetAddressOf(), m_Context->GetDSV().Get());
		m_Context->GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//TODO:
		vertexArray->GetVertexBuffers()[0]->Bind();
		if (indexCount != 0)
		{
			m_Context->GetD3DContext()->DrawIndexed(count, 0, 0);
		}
		//ComPtr<ID3D11Texture2D> backBuffer;
		//m_Context->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		//m_Context->GetD3DContext()->CopyResource(m_Context->GetBackbuffer().Get(), backBuffer.Get());
		//backBuffer.Reset();


	}
	void D3D11RendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, u32 vertexCount)
	{
		m_Context->GetD3DContext()->OMSetBlendState(m_Context->GetBS().Get(), m_ClearColor, 0xFFFFFFFF);
		m_Context->GetD3DContext()->OMSetRenderTargets(m_Context->GetRTVs().size(), m_Context->GetRTVs().data()->GetAddressOf(), m_Context->GetDSV().Get());
		vertexArray->GetVertexBuffers()[0]->Bind();
		m_Context->GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_Context->GetD3DContext()->Draw(vertexCount, 0);

	}

	void D3D11RendererAPI::SetLineWidth(float width)
	{

	}
}