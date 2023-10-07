#include "stspch.h"
#include "D3D11Buffer.h"

namespace Steins
{
    D3D11VertexBuffer::D3D11VertexBuffer(float* vertices, u32 size)
    {
		std::vector<float> vec_vertices{ vertices, vertices + (size / sizeof(float)) };

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
		bufferDesc.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // MS �������� �ʱ�ȭ�ϴ� ���
		vertexBufferData.pSysMem = vec_vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
    }
}