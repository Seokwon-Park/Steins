#pragma once

#include "Steins/Renderer/Framebuffer.h"

namespace Steins
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(u32 width, u32 height) override;

		virtual u32 GetColorAttachmentRendererID() const override {	return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }


	private:
		u32 m_RendererID = 0;
		u32 m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;


	};
}