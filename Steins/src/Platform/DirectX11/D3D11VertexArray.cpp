#include "stspch.h"
#include "D3D11VertexArray.h"

namespace Steins
{
	D3D11VertexArray::D3D11VertexArray()
	{
		//glCreateVertexArrays(1, &m_RendererID);
	}
	D3D11VertexArray::~D3D11VertexArray()
	{
		//glDeleteBuffers(1, &m_RendererID);
	}
	void D3D11VertexArray::Bind() const
	{
		//glBindVertexArray(m_RendererID);
	}
	void D3D11VertexArray::UnBind() const
	{
		//glBindVertexArray(0);
	}
	void D3D11VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		STS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		//glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		//In DX11
		//std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		//{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};


		u32 index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			/*glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;*/
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void D3D11VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}