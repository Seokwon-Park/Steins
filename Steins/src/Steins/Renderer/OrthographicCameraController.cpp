#include "stspch.h"
#include "OrthographicCameraController.h"

#include "Steins/Core/Input.h"
#include "Steins/Core/KeyCodes.h"

Steins::OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
	: m_AspectRatio(aspectRatio), 
	m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), 
	m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top),
	m_Rotation(rotation)
{
}

void Steins::OrthographicCameraController::OnUpdate(Timestep dt)
{
	STS_PROFILE_FUNCTION();

	if (Input::IsKeyPressed(Key::W))
	{
		m_CameraPosition.y += m_CameraTranslationSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
	}
	if (Input::IsKeyPressed(Key::A))
	{
		m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_CameraPosition.x += m_CameraTranslationSpeed * dt;
	}

	if (m_Rotation)
	{
		if (Steins::Input::IsKeyPressed(Key::Q))
		{
			m_CameraRotation += m_CameraRotationSpeed * dt;
		}
		else if (Steins::Input::IsKeyPressed(Key::E))
		{
			m_CameraRotation -= m_CameraRotationSpeed * dt;
		}
		m_Camera.SetRotation(m_CameraRotation);
	}
	m_Camera.SetPosition(m_CameraPosition);

	m_CameraTranslationSpeed = m_ZoomLevel;
}

void Steins::OrthographicCameraController::OnEvent(Event& e)
{
	STS_PROFILE_FUNCTION();

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(STS_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(STS_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void Steins::OrthographicCameraController::CalculateView()
{
	m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void Steins::OrthographicCameraController::OnResize(float width, float height)
{
	m_AspectRatio = width / height;
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

bool Steins::OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	STS_PROFILE_FUNCTION();

	m_ZoomLevel -= e.GetYOffset()*0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	CalculateView();
	return false;
}

bool Steins::OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
	STS_PROFILE_FUNCTION();

	OnResize((float)e.GetWidth(), (float)e.GetHeight());
	CalculateView();
	return false;
}
