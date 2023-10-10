#include "stspch.h"
#include "D3D11Buffer.h"
#include "D3D11Context.h"
#include "Steins/Core/Application.h"

namespace Steins
{
	D3D11VertexBuffer::D3D11VertexBuffer(float* vertices, u32 size, u32 count)
		:m_Stride(size/(count*sizeof(float)))
    {
		std::vector<float> vec_vertices{ vertices, vertices + (size / sizeof(float)) };

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
		vertexBufferDesc.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // MS 예제에서 초기화하는 방식
		vertexBufferData.pSysMem = vec_vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());

		m_Context->GetD3DDevice()->
			CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			m_VertexBuffer.GetAddressOf());
		

    }
	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		m_VertexBuffer->Release();
	}
	void D3D11VertexBuffer::Bind() const
	{
		m_Context->GetD3DContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
	}
	void D3D11VertexBuffer::UnBind() const
	{

	}
	D3D11IndexBuffer::D3D11IndexBuffer(u32* indices, u32 count)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
		bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * count);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
		bufferDesc.StructureByteStride = sizeof(uint32_t);

		std::vector<float> vec_indices{ indices, indices + count };

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = vec_indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());

		m_Context->GetD3DDevice()->
			CreateBuffer(
				&bufferDesc,
				&indexBufferData,
				m_IndexBuffer.GetAddressOf());
	}
	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
		m_IndexBuffer->Release();
	}
	void D3D11IndexBuffer::Bind() const
	{
	}
	void D3D11IndexBuffer::UnBind() const
	{
	}
}