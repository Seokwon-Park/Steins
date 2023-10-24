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
			case FramebufferTextureFormat::RED_INTEGER:
				return DXGI_FORMAT_R32_SINT;
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				return DXGI_FORMAT_D24_UNORM_S8_UINT;
				// Handle other formats as needed
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		//// Function to create textures
		static void CreateTextures(bool multisampled, std::vector<ComPtr<ID3D11Texture2D>>& outID, u32 count, 
			std::vector<FramebufferTextureSpecification> format, FramebufferSpecification spec) {
			// Create a texture
			//for (auto id : outID)
			//{
			//	if(id)
			//	id->Release();
			//}

			D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			for (u32 i = 0; i < count; i++)
			{
				ComPtr<ID3D11Texture2D> backBuffer;
				m_Context->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
				D3D11_TEXTURE2D_DESC bbDesc;
				backBuffer->GetDesc(&bbDesc);

				D3D11_TEXTURE2D_DESC textureDesc = {};
				textureDesc.Width = spec.Width;
				textureDesc.Height = spec.Height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = ConvertToDXGIFormat(format[i].TextureFormat);
				textureDesc.SampleDesc.Count = multisampled ? 4 : 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // You can add more flags as needed
				backBuffer.Reset();

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
			D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			m_Context->GetD3DDevice()->CreateShaderResourceView(m_Context->GetRTTs()[index].Get(), nullptr, srv.GetAddressOf());
		}

		// Function to attach a color texture
		static void AttachColorTexture(int samples, FramebufferTextureFormat format, u32 width, u32 height, int index) {
			D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			// Create a render target view for the texture
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = ConvertToDXGIFormat(format);
			rtvDesc.ViewDimension = false ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
			HRESULT hr = m_Context->GetD3DDevice()->CreateRenderTargetView(m_Context->GetRTTs()[index].Get(), &rtvDesc, m_Context->GetRTVs()[index].GetAddressOf());
			// Bind the render target view as needed
			// Example: g_pd3dDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		}

		// Function to attach a depth texture
		static void AttachDepthTexture(u32 id, int samples, FramebufferTextureFormat format, u32 width, u32 height) {
			D3D11Context* m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
			// Create a depth-stencil view for the texture
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = ConvertToDXGIFormat(format);
			dsvDesc.ViewDimension = false ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
			HRESULT hr = m_Context->GetD3DDevice()->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, m_Context->GetDSV().GetAddressOf());

			// Bind the depth-stencil view as needed
			// Example: g_pd3dDeviceContext->OMSetRenderTargets(1, nullptr, dsv);
		}

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

			Utils::CreateTextures(multisample, m_Context->GetRTTs(), m_ColorSRVs.size(), m_ColorAttachmentSpecifications, m_Specification);
			//std::vector<ID3D11Texture2D*> tmp = m_Context->GetRTTs();

			for (u64 i = 0; i < m_ColorSRVs.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorSRVs[i], i);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_Specification.Samples, FramebufferTextureFormat::RGBA8, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_Specification.Samples, FramebufferTextureFormat::RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample,{m_Context->GetDSB()}, 1, { m_DepthAttachmentSpecification }, m_Specification);
			//std::vector<ID3D11Texture2D*> tmp = m_Context->GetRTTs();

			for (u64 i = 0; i < m_ColorSRVs.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorSRVs[i], i);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_Specification.Samples, FramebufferTextureFormat::RGBA8, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_Specification.Samples, FramebufferTextureFormat::RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		//ComPtr<ID3D11Texture2D> backBuffer;
		//m_Context->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		//D3D11_TEXTURE2D_DESC bbDesc;
		//backBuffer->GetDesc(&bbDesc);
		//D3D11_TEXTURE2D_DESC txtDesc = {};
		//txtDesc.Width = bbDesc.Width;
		//txtDesc.Height = bbDesc.Height;
		//txtDesc.MipLevels = 1;
		//txtDesc.ArraySize = 1;
		//txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//txtDesc.SampleDesc.Count = 1;
		//txtDesc.SampleDesc.Quality = 0;
		//txtDesc.Usage = D3D11_USAGE_DEFAULT;
		//txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		//m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, nullptr, m_Texture.GetAddressOf());
		//m_Context->GetD3DDevice()->CreateShaderResourceView(m_Context->GetRTTs()[0].Get(), nullptr, m_ColorSRVs[0].GetAddressOf());
		//m_Context->GetD3DDevice()->CreateShaderResourceView(m_Context->GetRTTs()[1].Get(), nullptr, m_ColorSRVs[1].GetAddressOf());

	}
	void D3D11Framebuffer::Bind()
	{
		//m_Context->GetD3DContext()->CopyResource(m_Context->GetRTTs()[0].Get(), m_Context->GetBackbuffer().Get());
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

	int D3D11Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{		
		D3D11_TEXTURE2D_DESC textureDesc = {};
		m_Context->GetRTTs()[attachmentIndex]->GetDesc(&textureDesc);
		textureDesc.Width = 1;
		textureDesc.Height = 1;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		textureDesc.Usage = D3D11_USAGE_STAGING;
		textureDesc.BindFlags = 0; // You can add more flags as needed
		textureDesc.MiscFlags = 0; // You can add more flags as needed

		m_Context->GetD3DDevice()->CreateTexture2D(&textureDesc, nullptr, m_indexTempTexture.GetAddressOf());

		//1x1 pixel box
		D3D11_BOX box;

		box.left = std::clamp(x, 0, (int)m_Specification.Width);
		box.right = std::clamp(x + 1, 0, (int)m_Specification.Width);
		box.top = std::clamp(y, 0, (int)m_Specification.Height);
		box.bottom = std::clamp(y + 1, 0, (int)m_Specification.Height);

		// if 3D Texture
		box.front = 0;
		box.back = 1;

		//D3D11_TEXTURE2D_DESC desc = {};
		//m_Context->GetRTTs()[0].Get()->GetDesc(&desc);
		////STS_CORE_TRACE("{0}, {1}", desc.Width, desc.Height);

		m_Context->GetD3DContext()->CopySubresourceRegion(m_indexTempTexture.Get(), 0, 0, 0, 0,
			m_Context->GetRTTs()[attachmentIndex].Get(), 0, &box);
		//m_Context->GetD3DContext()->CopySubresourceRegion(tempTexture.Get(), 0, 0, 0, 0,
		//	tempTextureforCopy.Get(), 0, &box);

		//uint8_t test[1];

		//D3D11_MAPPED_SUBRESOURCE ms;
		//m_Context->GetD3DContext()->Map(m_indexTempTexture.Get(), NULL, D3D11_MAP_READ, NULL,
		//	&ms); // D3D11_MAP_READ 주의
		//memcpy(test, ms.pData, sizeof(uint8_t));
		//m_Context->GetD3DContext()->Unmap(m_indexTempTexture.Get(), NULL);
		//return (int)test[0];

		uint8_t test[4];

		D3D11_MAPPED_SUBRESOURCE ms;
		m_Context->GetD3DContext()->Map(m_indexTempTexture.Get(), NULL, D3D11_MAP_READ, NULL,
			&ms); // D3D11_MAP_READ 주의
		memcpy(test, ms.pData, sizeof(uint8_t)*4);
		m_Context->GetD3DContext()->Unmap(m_indexTempTexture.Get(), NULL);

		m_indexTempTexture.Reset();
		return (int)test[0];
	}

}