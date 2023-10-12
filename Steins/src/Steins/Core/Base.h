#pragma once

#include <memory>

// �� ������ ��ġ�� ���� DLL������ BUILD_DLL�� ���ǵǾ� �ֱ� ������ export�� ���� ��������
// �� ������ include�ϴ� ���ø����̼��� BUILD_DLL�� ���ǵǾ� ���� �ʱ� ������ import �ϰ� �ȴ�.
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