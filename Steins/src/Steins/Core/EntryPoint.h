#pragma once

#ifdef STS_PLATFORM_WINDOWS

extern Steins::Application* Steins::CreateApplication();

int main(int argc, char** argv)
{
	Steins::Log::Init();

	STS_PROFILE_BEGIN_SESSION("Startup", "SteinsProfile-Startup.json");
	auto app = Steins::CreateApplication();
	STS_PROFILE_END_SESSION();
	STS_PROFILE_BEGIN_SESSION("Runtime", "SteinsProfile-Runtime.json");
	app->Run();
	STS_PROFILE_END_SESSION();
	STS_PROFILE_BEGIN_SESSION("Shutdown", "SteinsProfile-Shutdown.json");
	delete app;
	STS_PROFILE_END_SESSION();
}

#endif