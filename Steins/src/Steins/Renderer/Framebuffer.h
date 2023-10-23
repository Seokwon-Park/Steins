#pragma once

#include "Steins/Core/Base.h"

namespace Steins
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		
		//Color
		RGBA8,
		RED_INTEGER,

		//Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		u32 Width, Height;
		FramebufferAttachmentSpecification Attachments;
		u32 Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(u32 width, u32 height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual u32 GetColorAttachmentRendererID(u32 index = 0) const = 0;
		virtual ID3D11ShaderResourceView* GetSRV(u32 index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}