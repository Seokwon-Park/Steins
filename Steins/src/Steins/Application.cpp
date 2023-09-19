#include "stspch.h"
#include "Application.h"

#include "Steins/Events/ApplicationEvent.h"
#include "Steins/Log.h"

namespace Steins {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			STS_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			STS_TRACE(e);
		}

		while (true);
	}
}