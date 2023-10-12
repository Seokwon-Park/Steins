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

		virtual u32 GetColorAttachmentRendererID() const override {	return m_ColorAttachment; }


		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		u32 m_RendererID;
		u32 m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;


	};
}