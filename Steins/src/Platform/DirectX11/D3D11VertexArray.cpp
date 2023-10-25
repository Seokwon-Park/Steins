#include "stspch.h"
#include "D3D11VertexArray.h"

#include "Steins/Core/Application.h"

#include <map>

namespace Steins
{
	static DXGI_FORMAT GetDXGIFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Mat3:	return DXGI_FORMAT_UNKNOWN;
		case ShaderDataType::Mat4:	return DXGI_FORMAT_UNKNOWN;
		case ShaderDataType::Int:	return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:	return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:	return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:	return DXGI_FORMAT_R32G32B32A32_SINT;
		case ShaderDataType::Bool:	return DXGI_FORMAT_UNKNOWN;
		}

		STS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}

	D3D11VertexArray::D3D11VertexArray()
	{
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());

		//glCreateVertexArrays(1, &m_RendererID);
	}
	D3D11VertexArray::~D3D11VertexArray()
	{
		m_VertexBuffers = std::vector<Ref<VertexBuffer>>();
		m_Context->GetD3DContext()->Flush();
		
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

		//for (key, value) -> (TEXCOORD, 0), (TEXCOORD, 1), (TEXCOORD, 2) ...
		std::map<std::string, UINT> m;
		//In DX11
		//std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		//{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};

		u32 index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
		for (const auto& element : layout)
		{
			//For Semantic Index
			if (m.find(element.Name.c_str()) != m.end()) {
				// key가 이미 존재하는 경우, 값을 1 증가시킴
				m[element.Name.c_str()]++;
			}
			else {
				// key가 없는 경우, 값을 1로 초기화
				m[element.Name.c_str()] = 0;
			}
			D3D11_INPUT_ELEMENT_DESC DXLayout = {element.Name.c_str(), m[element.Name.c_str()], GetDXGIFormat(element.Type), 0, element.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0};
			inputElements.push_back(DXLayout);
		}
		m_Context->SetInputElements(inputElements);
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void D3D11VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}