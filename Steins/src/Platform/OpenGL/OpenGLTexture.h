#pragma once

#include "Steins/Renderer/Texture.h"

#include <glad/glad.h>

namespace Steins
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(u32 width, u32 height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual u32 GetWidth() const override { return m_Width; }
		virtual u32 GetHeight() const override { return m_Height; }
		virtual u32 GetRendererID() const override { return m_RendererID; }
		virtual ID3D11ShaderResourceView* GetSRV() const override { return nullptr; };


		virtual void SetData(void* data, u32 size) override;

		virtual void Bind(u32 slot = 0) const override;

		virtual bool operator==(const Texture& other) const override 
		{ 
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		};

	private:
		std::string m_Path;
		u32 m_Width, m_Height;
		u32 m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}