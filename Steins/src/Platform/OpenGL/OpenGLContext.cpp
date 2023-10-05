#include "stspch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace Steins
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		STS_CORE_ASSERT(windowHandle,"Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		STS_CORE_ASSERT(status, "Failed to initialize Glad!");

		STS_CORE_INFO("OpenGL Info:" );
		STS_CORE_INFO("  Vendeor: {0}", (const char*)glGetString(GL_VENDOR));
		STS_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		STS_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	ID3D11Device* OpenGLContext::GetDevice()
	{
		return nullptr;
	}
	ID3D11DeviceContext* OpenGLContext::GetContext()
	{
		return nullptr;
	}
}