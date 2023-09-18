#pragma once

#ifdef STS_PLATFORM_WINDOWS

extern Steins::Application* Steins::CreateApplication();

int main(int argc, char** argv)
{
	printf("Steins Engine\n");
	auto app = Steins::CreateApplication();
	app->Run();
	delete app;
}

#endif