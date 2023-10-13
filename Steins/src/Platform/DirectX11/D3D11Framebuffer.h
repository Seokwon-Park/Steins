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

		virtual u32 GetColorAttachmentRendererID() const override { return 0; }
		virtual ID3D11ShaderResourceView* GetSRV() const override { return m_ColorSRV.Get(); }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }


	private:
		D3D11Context* m_Context;
		ComPtr<ID3D11Texture2D> m_RT;
		ComPtr<ID3D11ShaderResourceView> m_ColorSRV;
		u32 m_RendererID;
		u32 m_RenderTexture, m_DepthAttachment;
		FramebufferSpecification m_Specification;


	};
}