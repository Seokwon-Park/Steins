#pragma once

// 이 파일이 위치한 엔진 DLL에서는 BUILD_DLL이 정의되어 있기 때문에 export를 통해 내보내고
// 이 파일을 include하는 어플리케이션은 BUILD_DLL이 정의되어 있지 않기 때문에 import 하게 된다.
#ifdef STS_PLATFORM_WINDOWS
	#ifdef STS_BUILD_DLL
		#define STEINS_API __declspec(dllexport)
	#else
		#define STEINS_API __declspec(dllimport)
	#endif
#else
	#error Steins only support Windows!
#endif

#define BIT(x) (1 << x)
	
