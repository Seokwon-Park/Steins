#pragma once

// �� ������ ��ġ�� ���� DLL������ BUILD_DLL�� ���ǵǾ� �ֱ� ������ export�� ���� ��������
// �� ������ include�ϴ� ���ø����̼��� BUILD_DLL�� ���ǵǾ� ���� �ʱ� ������ import �ϰ� �ȴ�.
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
	
