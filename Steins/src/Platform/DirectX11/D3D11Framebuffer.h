#pragma once

#include "D3D11Context.h"

#include "Steins/Renderer/Framebuffer.h"


namespace Steins
{
	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(const FramebufferSpecification& spec);
		virtual ~D3D11Framebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(u32 width, u32 height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual u32 GetColorAttachmentRendererID(u32 index = 0) const override { return 0; }
		virtual ID3D11ShaderResourceView* GetSRV(u32 index = 0) const override { return m_ColorSRVs[index].Get(); }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }


	private:
		D3D11Context* m_Context;
		ComPtr<ID3D11Texture2D> m_Texture;
		ComPtr<ID3D11Texture2D> m_Texture2;
		ComPtr<ID3D11ShaderResourceView> m_ColorSRV;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<ComPtr<ID3D11ShaderResourceView>> m_ColorSRVs;
		u32 m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;


	};
}