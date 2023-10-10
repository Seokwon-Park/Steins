#pragma once

namespace Steins
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers(bool VSync) = 0;		
	protected:
	};
}