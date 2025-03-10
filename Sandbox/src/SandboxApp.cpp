#include <Steins.h>
#include <Steins/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Steins::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.f / 720.f, true)
	{
		m_VertexArray = Steins::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-.5f, -.5f, 0.f, 0.0f, 0.0f, 1.0f, 1.0f,
			 .5f, -.5f, 0.f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.f,  .5f, 0.f, 1.0f, 0.0f, 0.0f, 1.0f,
		};

		Steins::Ref<Steins::VertexBuffer>vertexBuffer;
		vertexBuffer = Steins::VertexBuffer::Create(vertices, sizeof(vertices), 3);

		Steins::BufferLayout layout;
		//temp
		if (Steins::Renderer::GetAPI() == Steins::RendererAPI::API::OpenGL)
			//OpenGLStyle
		{
			layout = {
				{ Steins::ShaderDataType::Float3, "a_Position", true},
				{ Steins::ShaderDataType::Float4, "a_Color", true},
			};
		}
		else
			//DX11Style
		{
			layout = {
				{ Steins::ShaderDataType::Float3, "POSITION", true},
				{ Steins::ShaderDataType::Float4, "COLOR", true},
			};
		}

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		Steins::Ref<Steins::IndexBuffer> indexBuffer;
		indexBuffer = Steins::IndexBuffer::Create(indices, sizeof(indices) / sizeof(u32));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Steins::VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-.5f, -.5f, 0.f, 0.0f,	0.0f,
			 .5f, -.5f, 0.f, 1.0f,	0.0f,
			 0.5f,  .5f, 0.f, 1.0f, 1.0f,
			 -0.5f, .5f, 0.f, 0.0f,1.0f
		};

		//Steins::Ref<Steins::VertexBuffer> squareVB;
		//squareVB = Steins::VertexBuffer::Create(squareVertices, sizeof(squareVertices), 4);
		//Steins::BufferLayout squareVBLayout;
		//if (Steins::Renderer::GetAPI() == Steins::RendererAPI::API::OpenGL)
		//	//OpenGLStyle
		//{
		//	squareVBLayout = {
		//		{ Steins::ShaderDataType::Float3, "a_Position", true},
		//		{ Steins::ShaderDataType::Float2, "a_TexCoord", true},
		//	};
		//}
		//else
		//	//DX11Style
		//{
		//	squareVBLayout = {
		//		{ Steins::ShaderDataType::Float3, "POSITION", true},
		//		{ Steins::ShaderDataType::Float2, "TEXCOORD", true},
		//	};
		//}
		//squareVB->SetLayout(squareVBLayout);
		//m_SquareVA->AddVertexBuffer(squareVB);

		//unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
		//Steins::Ref<Steins::IndexBuffer> squareIB;
		//squareIB = Steins::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(u32));
		//m_SquareVA->SetIndexBuffer(squareIB);


		if (Steins::Renderer::GetAPI() == Steins::RendererAPI::API::OpenGL)
			//OpenGLStyle
		{
			std::string vertexSrc = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;
				
				out vec3 v_Position;		
				out vec4 v_Color;	
				
				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

			m_Shader = Steins::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);
		}
		else
			//DX11Style
		{
			std::string vertexShader = R"(
struct VS_Input
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    output.position = float4(input.pos, 1.0f);
    output.color = input.color;

    return output;
}
)";



			std::string pixelShader = R"(
struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 ps_main(VS_Output input) : SV_TARGET
{
	return input.color;
}
)";
			m_Shader = Steins::Shader::Create("VertexPosColor", vertexShader, pixelShader);
		}

		if (Steins::Renderer::GetAPI() == Steins::RendererAPI::API::OpenGL)
			//OpenGLStyle
		{
			std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

			std::string blueShaderfragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";

			m_BlueShader = Steins::Shader::Create("blueColor", blueShaderVertexSrc, blueShaderfragmentSrc);
			auto textureShader = m_ShaderLibrary.Load("assets/GLshaders/Texture.glsl");

			std::dynamic_pointer_cast<Steins::OpenGLShader>(textureShader)->Bind();
			std::dynamic_pointer_cast<Steins::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		}
		else
		{

		}

		//m_Texture = Steins::Texture2D::Create("assets/textures/Checkerboard.png");
		//m_LogoTexture = Steins::Texture2D::Create("assets/textures/Logo.png");

		
	}



	void OnUpdate(Steins::Timestep dt) override
	{
		// Update
		m_CameraController.OnUpdate(dt);

		// Render
		Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Steins::RenderCommand::Clear();

		Steins::Renderer::BeginScene(m_CameraController.GetCamera());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		/*std::dynamic_pointer_cast<Steins::OpenGLShader>(m_BlueShader)->Bind();
		std::dynamic_pointer_cast<Steins::OpenGLShader>(m_BlueShader)->UploadUniformFloat4("u_Color", m_SquareColor);*/

		//for (int y = 0; y < 20; y++)
		//{
		//	for (int x = 0; x < 20; x++)
		//	{
		//		glm::vec3 pos(x * 0.11f, y * 0.11f, 0.f);
		//		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
		//		Steins::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
		//	}
		//}

		//auto textureShader = m_ShaderLibrary.Get("Texture");

		/*m_Texture->Bind();
		Steins::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Steins::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.25f,-0.25f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));*/

		// Triangle
		m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
		Steins::Renderer::Submit(m_Shader, m_VertexArray);

		Steins::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Steins::Event& e) override
	{
		//m_CameraController.OnEvent(e);

		if (e.GetEventType() == Steins::EventType::WindowResize)
		{
			auto& re = (Steins::WindowResizeEvent&)e;
		}
	}

private:
	Steins::ShaderLibrary m_ShaderLibrary;

	Steins::Ref<Steins::Shader> m_Shader;
	Steins::Ref<Steins::VertexArray> m_VertexArray;

	Steins::Ref<Steins::Shader> m_BlueShader;
	Steins::Ref<Steins::VertexArray> m_SquareVA;

	Steins::Ref<Steins::Texture2D> m_Texture, m_LogoTexture;

	Steins::OrthographicCameraController m_CameraController;
	glm::vec4 m_SquareColor = { .2f, .3f, .8f, 1.f };
};

using namespace Steins;



class Sandbox : public Steins::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Steins::Application* Steins::CreateApplication()
{
	return new Sandbox();
}