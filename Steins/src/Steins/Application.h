#pragma once

#include "Core.h"

namespace Steins {
	class STEINS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();
}

