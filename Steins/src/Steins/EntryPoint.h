#pragma once

#ifdef STS_PLATFORM_WINDOWS

extern Steins::Application* Steins::CreateApplication();

int main(int argc, char** argv)
{
	Steins::Log::Init();
	STS_CORE_WARN("Initialized Log!");
	int a = 5;
	STS_INFO("Hello! Var={0}",a);

	auto app = Steins::CreateApplication();
	app->Run();
	delete app;
}

#endif