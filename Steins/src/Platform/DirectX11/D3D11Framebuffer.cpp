#include "stspch.h"
#include "D3D11Framebuffer.h"

#include "Steins/Core/Application.h"

namespace Steins
{
	static const u32 s_MaxFrameBufferSize = 8192;

	namespace Utils
	{
		//static GLenum TextureTarget(bool multisampled)
		//{
		//	return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		//}
		

		static DXGI_FORMAT ConvertToDXGIFormat(FramebufferTextureFormat format) {
			switch (format) {
			case FramebufferTextureFormat::RGBA8:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				return DXGI_FORMAT_D24_UNORM_S8_UINT;
				// Handle other formats as needed
			}
		}

		//// Function to create textures
		static void CreateTextures(bool multisampled, std::vector<ComPtr<ID3D11Texture2D>>& outID, u32 count, FramebufferTextureFormat format) {
			// Create a texture
			//for (auto id : outID)
			//{
			//	if(id)
			//	id->Release();
			//}

			D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			for (u32 i = 0; i < count; i++)
			{
				D3D11_TEXTURE2D_DESC textureDesc = {};
				textureDesc.Width = Application::Get().GetWindow().GetWidth();
				textureDesc.Height = Application::Get().GetWindow().GetHeight();
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = ConvertToDXGIFormat(format);
				textureDesc.SampleDesc.Count = multisampled ? 4 : 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // You can add more flags as needed

				HRESULT hr = m_Context->GetD3DDevice()->CreateTexture2D(&textureDesc, nullptr, outID[i].GetAddressOf());
			} 

			// Handle HRESULT error checking
		}

		// Function to bind a texture
		static void BindTexture(bool multisampled, ComPtr<ID3D11ShaderResourceView>& srv, int index) {
			if (srv) srv->Release();
			// You generally bind textures as shader resource views in DirectX 11
			// Bind 'id' as a shader resource view here using g_pd3dDeviceContext
			// Example: g_pd3dDeviceContext->PSSetShaderResources(slot, 1, &id);
			//D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			//m_Context->GetD3DDevice()->CreateShaderResourceView(m_Context->GetRTTs()[index].Get(), nullptr, &srv);
		}

		// Function to attach a color texture
		static void AttachColorTexture(int samples, FramebufferTextureFormat format, u32 width, u32 height, int index) {
			D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			// Create a render target view for the texture
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = ConvertToDXGIFormat(format);
			rtvDesc.ViewDimension = false ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
			HRESULT hr = m_Context->GetD3DDevice()->CreateRenderTargetView(m_Context->GetRTTs()[index].Get(), &rtvDesc, &m_Context->GetRTVs()[index]);
			// Bind the render target view as needed
			// Example: g_pd3dDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		}

		//// Function to attach a depth texture
		////static void AttachDepthTexture(u32 id, int samples, FramebufferTextureFormat format, FramebufferTextureFormat attachmentType, u32 width, u32 height) {
		////	// Create a depth-stencil view for the texture
		////	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		////	dsvDesc.Format = ConvertToDXGIFormat(format);
		////	dsvDesc.ViewDimension = false ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		////	HRESULT hr = g_pd3dDevice->CreateDepthStencilView(id, &dsvDesc, nullptr);

		////	// Bind the depth-stencil view as needed
		////	// Example: g_pd3dDeviceContext->OMSetRenderTargets(1, nullptr, dsv);
		////}

		//// Function to check if the format is a depth format
		static bool IsDepthFormat(FramebufferTextureFormat format) {
			switch (format) {
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
			default:
				return false;
			}
		}
	}
	D3D11Framebuffer::D3D11Framebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
			{
				m_ColorAttachmentSpecifications.emplace_back(spec.TextureFormat);
			}
			else
			{
				m_DepthAttachmentSpecification = spec.TextureFormat;
			}
		}
		Invalidate();
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
		m_Texture.Reset();
		m_ColorSRV.Reset();
		m_ColorSRVs.clear();
	}
	void D3D11Framebuffer::Invalidate()
	{
		m_ColorSRVs.clear();
		m_ColorSRV.Reset();
		m_Texture.Reset();

		bool multisample = m_Specification.Samples > 1;

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorSRVs.resize(m_ColorAttachmentSpecifications.size());
			m_Context->GetRTTs().resize(m_ColorAttachmentSpecifications.size());
			m_Context->GetRTVs().resize(m_ColorAttachmentSpecifications.size());
			
			Utils::CreateTextures(multisample, m_Context->GetRTTs(), m_ColorSRVs.size(), FramebufferTextureFormat::RGBA8);
			//std::vector<ID3D11Texture2D*> tmp = m_Context->GetRTTs();

			for (u64 i = 1; i < m_ColorSRVs.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorSRVs[i], i);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_Specification.Samples, FramebufferTextureFormat::RGBA8, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		ComPtr<ID3D11Texture2D> backBuffer;
		m_Context->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		D3D11_TEXTURE2D_DESC bbDesc;
		backBuffer->GetDesc(&bbDesc);
		D3D11_TEXTURE2D_DESC txtDesc = {};
		txtDesc.Width = bbDesc.Width;
		txtDesc.Height = bbDesc.Height;
		txtDesc.MipLevels = 1;
		txtDesc.ArraySize = 1;
		txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		txtDesc.SampleDesc.Count = 1;
		txtDesc.SampleDesc.Quality = 0;
		txtDesc.Usage = D3D11_USAGE_DEFAULT;
		txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, nullptr, m_Texture.GetAddressOf());
		m_Context->GetD3DDevice()->CreateShaderResourceView(m_Context->GetRTTs()[0].Get(), nullptr, m_ColorSRVs[0].GetAddressOf());
		m_Context->GetD3DDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr, m_ColorSRVs[1].GetAddressOf());

	}
	void D3D11Framebuffer::Bind()
	{
		m_Context->GetD3DContext()->CopyResource(m_Context->GetRTTs()[0].Get(), m_Context->GetBackbuffer().Get());
		m_Context->GetD3DContext()->CopyResource(m_Texture.Get(), m_Context->GetTest().Get());

	}
	void D3D11Framebuffer::Unbind()
	{
		
	}

	void D3D11Framebuffer::Resize(u32 width, u32 height)
	{
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			STS_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		ComPtr<ID3D11Texture2D> backBuffer;
		m_Context->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		D3D11_TEXTURE2D_DESC bbDesc;
		backBuffer->GetDesc(&bbDesc);
		//m_Specification.Width = bbDesc.Width;
		//m_Specification.Height = bbDesc.Height;
		m_Specification.Width = width;
		m_Specification.Height = height;
		backBuffer.Reset();
		Invalidate();
	}

}