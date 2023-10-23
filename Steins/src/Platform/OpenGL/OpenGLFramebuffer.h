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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual u32 GetColorAttachmentRendererID(u32 index = 0) const override { 
			STS_CORE_ASSERT(index < m_ColorAttachments.size());
			return m_ColorAttachments[index]; }
		virtual ID3D11ShaderResourceView* GetSRV(u32 index = 0) const override { return nullptr; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }


	private:
		u32 m_RendererID = 0;
		//u32 m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<u32> m_ColorAttachments;
		u32 m_DepthAttachment = 0;
	};
}