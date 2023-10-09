#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	:Layer("Snadbox2D"), m_CameraController(16.f/9.f)
{
}

void Sandbox2D::OnAttach()
{	
	m_SquareVA = Steins::VertexArray::Create();

	float squareVertices[3 * 4] =
	{
		-.5f, -.5f, 0.f, 
		 .5f, -.5f, 0.f, 
		 0.5f,  .5f, 0.f,
		 -0.5f, .5f, 0.f
	};

	Steins::Ref<Steins::VertexBuffer> squareVB;
	squareVB.reset(Steins::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout(
	{
		{ Steins::ShaderDataType::Float3, "a_Position", true},
	});

	m_SquareVA->AddVertexBuffer(squareVB);

	unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
	Steins::Ref<Steins::IndexBuffer> squareIB;
	squareIB.reset(Steins::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(u32)));
	m_SquareVA->SetIndexBuffer(squareIB);
	
	m_FlatColorShader = Steins::Shader::Create("assets/GLshaders/FlatColor.glsl");
}
void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Steins::Timestep dt)
{
	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
	Steins::RenderCommand::Clear();

	Steins::Renderer::BeginScene(m_CameraController.GetCamera());
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	std::dynamic_pointer_cast<Steins::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Steins::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Steins::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Steins::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Steins::Event& e)
{
	m_CameraController.OnEvent(e);
}
