#pragma once

#include "Steins/Core/PlatformDetection.h"

#include <memory>

// 이 파일이 위치한 엔진 DLL에서는 BUILD_DLL이 정의되어 있기 때문에 export를 통해 내보내고
// 이 파일을 include하는 어플리케이션은 BUILD_DLL이 정의되어 있지 않기 때문에 import 하게 된다.


#ifdef STS_DEBUG
#if defined(STS_PLATFORM_WINDOWS)
#define STS_DEBUGBREAK() __debugbreak()
#elif defined(STS_PLATFORM_LINUX)
#include <signal.h>
#define STS_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define STS_ENABLE_ASSERTS
#else
#define STS_DEBUGBREAK()
#endif

#define STS_EXPAND_MACRO(x) x
#define STS_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)
	
#define STS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Steins
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Steins/Core/Log.h"
#include "Steins/Core/Assert.h"