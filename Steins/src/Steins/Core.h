#pragma once

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
	
#define STS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
