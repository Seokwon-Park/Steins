#pragma once

#include "D3D11Context.h"

#include "Steins/Renderer/Buffer.h"

namespace Steins
{
	using namespace Microsoft::WRL;

	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(u32 size, u32 vertexSize);
		D3D11VertexBuffer(float* vertices, u32 size, u32 count);
		virtual ~D3D11VertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetData(const void* data, u32 size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		D3D11Context* m_Context;
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		u32 m_Stride;
		u32 m_Offset = 0;
		BufferLayout m_Layout;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(u32* indices, u32 count);
		virtual ~D3D11IndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual u32 GetCount() const { return m_Count; };
	private:
		D3D11Context* m_Context;
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		u32 m_RendererID;
		u32 m_Count;
	};
}