#include "stspch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Steins
{
	/////////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(u32 size)
	{
		STS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, u32 size)
	{
		STS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		STS_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		STS_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		STS_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, u32 size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(u32* indices, u32 count)
		:m_Count(count)
	{
		STS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(u32), indices, GL_STATIC_DRAW);

	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		STS_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		STS_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		STS_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
