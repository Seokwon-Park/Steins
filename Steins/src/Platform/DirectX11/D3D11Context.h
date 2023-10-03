#pragma once

#include "Steins/Renderer/GraphicsContext.h"

namespace Steins
{
	class D3D11Context : public GraphicsContext
	{
	public:
		D3D11Context(HWND windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		HWND m_WindowHandle;
	};
}