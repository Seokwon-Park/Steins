#include "stspch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Steins
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Steins::ShaderDataType::None:	return GL_FLOAT;
		case Steins::ShaderDataType::Float:	return GL_FLOAT;
		case Steins::ShaderDataType::Float2:return GL_FLOAT;
		case Steins::ShaderDataType::Float3:return GL_FLOAT;
		case Steins::ShaderDataType::Float4:return GL_FLOAT;
		case Steins::ShaderDataType::Mat3:	return GL_FLOAT;
		case Steins::ShaderDataType::Mat4:	return GL_FLOAT;
		case Steins::ShaderDataType::Int:	return GL_INT;
		case Steins::ShaderDataType::Int2:	return GL_INT;
		case Steins::ShaderDataType::Int3:	return GL_INT;
		case Steins::ShaderDataType::Int4:	return GL_INT;
		case Steins::ShaderDataType::Bool:	return GL_BOOL;
		}
		STS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		STS_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		STS_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		STS_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		STS_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		STS_PROFILE_FUNCTION();

		STS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");
		
		glBindVertexArray(m_RendererID);
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
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		STS_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}