#include "stspch.h"
#include "D3D11Framebuffer.h"

#include "Steins/Core/Application.h"

namespace Steins
{
	D3D11Framebuffer::D3D11Framebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
		Invalidate();
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{

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
		txtDesc.Usage = D3D11_USAGE_DEFAULT;
		txtDesc.BindFlags = D3D11_BIND_RENDER_TARGET| D3D11_BIND_SHADER_RESOURCE;

		m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, nullptr, m_RT.GetAddressOf());
		m_Context->GetD3DDevice()->CreateRenderTargetView(m_RT.Get(), nullptr, m_Context->GetRTV().GetAddressOf());

		m_Context->GetD3DDevice()->CreateShaderResourceView(m_RT.Get(), nullptr, m_ColorSRV.GetAddressOf());
	}
	void D3D11Framebuffer::Bind()
	{
	}
	void D3D11Framebuffer::Unbind()
	{
	}

	void D3D11Framebuffer::Resize(u32 width, u32 height)
	{
	}

}