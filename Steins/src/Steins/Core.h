#pragma once

// 이 파일이 위치한 엔진 DLL에서는 BUILD_DLL이 정의되어 있기 때문에 export를 통해 내보내고
// 이 파일을 include하는 어플리케이션은 BUILD_DLL이 정의되어 있지 않기 때문에 import 하게 된다.
#ifdef STS_PLATFORM_WINDOWS
#if STS_DYNAMIC_LINK
	#ifdef STS_BUILD_DLL
		#define STEINS_API __declspec(dllexport)
	#else
		#define STEINS_API __declspec(dllimport)
	#endif
#else
	#define STEINS_API
#endif
#else
	#error Steins only support Windows!
#endif

#ifdef STS_DEBUG
	#define STS_ENABLE_ASSERTS
#endif

#ifdef STS_ENABLE_ASSERTS
	#define STS_ASSERT(x, ...) {if(!(x)) {STS_ERROR("Assertion Failed: {0}", __VA_ARGS); __debugbreak(); } }
	#define STS_CORE_ASSERT(x, ...) { if(!(x)) {STS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define STS_ASSERT(x, ...)
	#define STS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
	
#define STS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
