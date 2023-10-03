#pragma once

#include "Core.h"
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Steins
{
	class STEINS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define STS_CORE_TRACE(...)		::Steins::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define STS_CORE_INFO(...)		::Steins::Log::GetCoreLogger()->info(__VA_ARGS__)
#define STS_CORE_WARN(...)		::Steins::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define STS_CORE_ERROR(...)		::Steins::Log::GetCoreLogger()->error(__VA_ARGS__)
#define STS_CORE_FATAL(...)		::Steins::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define STS_TRACE(...)			::Steins::Log::GetClientLogger()->trace(__VA_ARGS__)
#define STS_INFO(...)			::Steins::Log::GetClientLogger()->info(__VA_ARGS__)
#define STS_WARN(...)			::Steins::Log::GetClientLogger()->warn(__VA_ARGS__)
#define STS_ERROR(...)			::Steins::Log::GetClientLogger()->error(__VA_ARGS__)
#define STS_FATAL(...)			::Steins::Log::GetClientLogger()->fatal(__VA_ARGS__)