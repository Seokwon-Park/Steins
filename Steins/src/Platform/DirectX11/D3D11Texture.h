#pragma once

#include "D3D11Context.h"
#include "Steins/Renderer/Texture.h"

namespace Steins
{
	class D3D11Texture2D : public Texture2D
	{
	public:
		D3D11Texture2D(u32 width, u32 height);
		D3D11Texture2D(const std::string& path);
		virtual ~D3D11Texture2D();

		virtual u32 GetWidth() const override { return m_Width; }
		virtual u32 GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, u32 size) override;

		virtual void Bind(u32 slot = 0) const override;
	private:
		ComPtr<ID3D11Texture2D> m_Texture;
		std::string m_Path;
		u32 m_Width, m_Height;
		u32 m_RendererID;
	};
}