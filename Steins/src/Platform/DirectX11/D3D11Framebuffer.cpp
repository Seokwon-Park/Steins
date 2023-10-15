#include "stspch.h"
#include "D3D11Framebuffer.h"

#include "Steins/Core/Application.h"

namespace Steins
{
	static const u32 s_MaxFrameBufferSize = 8192;

	D3D11Framebuffer::D3D11Framebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
		Invalidate();
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
		m_Context->GetD3DContext()->Flush();
	}
	void D3D11Framebuffer::Invalidate()
	{
		D3D11_TEXTURE2D_DESC txtDesc = {};
		txtDesc.Width = m_Specification.Width;
		txtDesc.Height = m_Specification.Height;
		txtDesc.MipLevels = 1;
		txtDesc.ArraySize = 1;
		txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		txtDesc.SampleDesc.Count = 1;
		txtDesc.SampleDesc.Quality = 0;
		txtDesc.Usage = D3D11_USAGE_DEFAULT;
		txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, nullptr, m_Texture.GetAddressOf());
		m_Context->GetD3DDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr, m_ColorSRV.GetAddressOf());

	}
	void D3D11Framebuffer::Bind()
	{


	}
	void D3D11Framebuffer::Unbind()
	{
		m_Context->GetD3DContext()->CopyResource(m_Texture.Get(), m_Context->GetBackbuffer());
	}

	void D3D11Framebuffer::Resize(u32 width, u32 height)
	{
		if(width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			STS_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		ComPtr<ID3D11Texture2D> backBuffer;
		m_Context->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		D3D11_TEXTURE2D_DESC bbDesc;
		backBuffer->GetDesc(&bbDesc);
		m_Specification.Width = bbDesc.Width;
		m_Specification.Height = bbDesc.Height;
		Invalidate();
	}

}