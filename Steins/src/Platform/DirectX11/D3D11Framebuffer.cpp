#include "stspch.h"
#include "D3D11Framebuffer.h"

namespace Steins
{
	D3D11Framebuffer::D3D11Framebuffer(const FramebufferSpecification& spec)
	{
		Invalidate();
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
	}
	void D3D11Framebuffer::Invalidate()
	{
	}
	void D3D11Framebuffer::Bind()
	{
	}
	void D3D11Framebuffer::Unbind()
	{
	}

	void D3D11Framebuffer::Resize(u32 width, u32 height)
	{
	}

}