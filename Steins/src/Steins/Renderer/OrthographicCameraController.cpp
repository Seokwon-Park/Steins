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

	if (Input::IsKeyPressed(STS_KEY_W))
	{
		m_CameraPosition.y += m_CameraTranslationSpeed * dt;
	}
	else if (Input::IsKeyPressed(STS_KEY_S))
	{
		m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
	}
	if (Input::IsKeyPressed(STS_KEY_A))
	{
		m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
	}
	else if (Input::IsKeyPressed(STS_KEY_D))
	{
		m_CameraPosition.x += m_CameraTranslationSpeed * dt;
	}

	if (m_Rotation)
	{
		if (Steins::Input::IsKeyPressed(STS_KEY_Q))
		{
			m_CameraRotation += m_CameraRotationSpeed * dt;
		}
		else if (Steins::Input::IsKeyPressed(STS_KEY_E))
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

bool Steins::OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	STS_PROFILE_FUNCTION();

	m_ZoomLevel -= e.GetYOffset()*0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

bool Steins::OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
	STS_PROFILE_FUNCTION();

	m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
