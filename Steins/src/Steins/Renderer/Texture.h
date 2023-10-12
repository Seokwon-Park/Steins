#pragma once

#include <string>

#include "Steins/Core/Base.h"

namespace Steins
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;
		virtual u32 GetRendererID() const = 0;
		virtual ID3D11ShaderResourceView* GetSRV() const = 0;

		virtual void SetData(void* data, u32 size) = 0;

		virtual void Bind(u32 slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(u32 width, u32 height);
		static Ref<Texture2D> Create(const std::string& path);
	};
}