#include "stspch.h"
#include "Window.h"

#ifdef STS_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Steins
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef STS_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		STS_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}