#pragma once

namespace Steins
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		
		virtual ID3D11Device* GetDevice() = 0;
		virtual ID3D11DeviceContext* GetContext() = 0;
	};
}