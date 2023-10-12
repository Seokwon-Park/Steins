#pragma once

#include "Steins/Core/Base.h"

namespace Steins
{
	struct FramebufferSpecification
	{
		u32 Width, Height;
		// FramebufferFormat Format = 
		u32 Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual u32 GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}