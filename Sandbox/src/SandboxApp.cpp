#include <Steins.h>

#include "imgui/imgui.h"

class ExampleLayer : public Steins::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		/*m_VertexArray.reset(Steins::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-.5f, -.5f, 0.f, 1.0f, 0.0f, 1.0f, 1.0f,
			 .5f, -.5f, 0.f, 0.0f, 1.0f, 1.0f, 1.0f,
			 0.f,  .5f, 0.f, 1.0f, 0.0f, 1.0f, 1.0f,
		};

		std::shared_ptr<Steins::VertexBuffer>vertexBuffer;
		vertexBuffer.reset(Steins::VertexBuffer::Create(vertices, sizeof(vertices)));

		Steins::BufferLayout layout = {
			{ Steins::ShaderDataType::Float3, "a_Position", true},
			{ Steins::ShaderDataType::Float4, "a_Color", true},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<Steins::IndexBuffer> indexBuffer;
		indexBuffer.reset(Steins::IndexBuffer::Create(indices, sizeof(indices) / sizeof(u32)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Steins::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-.5f, -.5f, 0.f,
			 .5f, -.5f, 0.f,
			 0.5f,  .5f, 0.f,
			 -0.5f,  .5f, 0.f
		};

		std::shared_ptr<Steins::VertexBuffer> squareVB;
		squareVB.reset(Steins::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Steins::BufferLayout squareVBLayout = {
			{ Steins::ShaderDataType::Float3, "a_Position", true},
		};
		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
		std::shared_ptr<Steins::IndexBuffer> squareIB;
		squareIB.reset(Steins::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(u32)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;		
			out vec4 v_Color;	

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Steins::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;		

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderfragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Steins::Shader(blueShaderVertexSrc, blueShaderfragmentSrc));*/
	}

	void OnUpdate() override
	{
		if (Steins::Input::IsKeyPressed(STS_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (Steins::Input::IsKeyPressed(STS_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}

		 if (Steins::Input::IsKeyPressed(STS_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (Steins::Input::IsKeyPressed(STS_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}

		 if (Steins::Input::IsKeyPressed(STS_KEY_A))
		 {
			 m_CameraRotation += m_CameraRotationSpeed;
		 }
		 else if (Steins::Input::IsKeyPressed(STS_KEY_D))
		 {
			 m_CameraRotation -= m_CameraRotationSpeed;
		 }


		Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Steins::RenderCommand::Clear();

		//m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetRotation(m_CameraRotation);

		//Steins::Renderer::BeginScene(m_Camera);

		//Steins::Renderer::Submit(m_BlueShader, m_SquareVA);
		//Steins::Renderer::Submit(m_Shader, m_VertexArray);

		//Steins::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(Steins::Event& event) override
	{
	}

private:
	std::shared_ptr<Steins::Shader> m_Shader;
	std::shared_ptr<Steins::VertexArray> m_VertexArray;

	std::shared_ptr<Steins::Shader> m_BlueShader;
	std::shared_ptr<Steins::VertexArray> m_SquareVA;

	Steins::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.1f;
};

class Sandbox : public Steins::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Steins::Application* Steins::CreateApplication()
{
	return new Sandbox();
}