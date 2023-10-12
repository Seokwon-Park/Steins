#pragma once

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

		virtual u32 GetColorAttachmentRendererID() const override { return 0; }


		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		u32 m_RendererID;
		u32 m_RenderTexture, m_DepthAttachment;
		FramebufferSpecification m_Specification;


	};
}