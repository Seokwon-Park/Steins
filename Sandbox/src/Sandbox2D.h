#pragma once

#include "Steins.h"

#include "ParticleSystem.h"

class Sandbox2D : public Steins::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Steins::Timestep dt) override; 
	virtual void OnImGuiRender() override; 
	virtual void OnEvent(Steins::Event& event) override;
private:
	Steins::OrthographicCameraController m_CameraController;

	// Temp
	Steins::Ref<Steins::VertexArray> m_SquareVA;
	Steins::Ref<Steins::Shader> m_FlatColorShader;

	Steins::Ref<Steins::Texture2D> m_CheckerboardTexture;
	Steins::Ref<Steins::Texture2D> m_LogoTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

};