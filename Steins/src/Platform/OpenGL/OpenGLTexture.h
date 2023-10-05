#pragma once

#include "Steins/Renderer/Texture.h"

namespace Steins
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual u32 GetWidth() const override { return m_Width; }
		virtual u32 GetHeight() const override { return m_Height; }

		virtual void Bind(u32 slot = 0) const override;
	private:
		std::string m_Path;
		u32 m_Width, m_Height;
		u32 m_RendererID;
	};
}