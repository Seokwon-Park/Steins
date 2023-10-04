#include "stspch.h"
#include "Application.h"

#include "Steins/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Steins {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

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

	Application::Application()
	{
		STS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		/*glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] =
		{
			-.5f, -.5f, 0.f, 1.0f, 0.0f, 1.0f, 1.0f,
			 .5f, -.5f, 0.f, 0.0f, 1.0f, 1.0f, 1.0f,
		 	 0.f,  .5f, 0.f, 1.0f, 0.0f, 1.0f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position", true},
				{ ShaderDataType::Float4, "a_Color", true},
			};
			m_VertexBuffer->SetLayout(layout);
		}

		u32 index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
		glEnableVertexAttribArray(index);		
		glVertexAttribPointer(index, 
			element.GetComponentCount(), 
			ShaderDataTypeToOpenGLBaseType( element.Type), 
			element.Normalized? GL_TRUE:GL_FALSE, 
			layout.GetStride(),
			(const void*)element.Offset);
		index++;
		}

		//In DX11
		//std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		//{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};

		unsigned int indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(u32)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;		
			out vec4 v_Color;	

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position*0.5+0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));*/
	}
	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			//glClearColor(.1f, .1f, .1f, 1);
			//glClear(GL_COLOR_BUFFER_BIT);

			//m_Shader->Bind();
			//glBindVertexArray(m_VertexArray);
			//glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}