#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdint.h>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Steins/Core/Log.h"

#include "Steins/Debug/Instrumentor.h"

//platforms
#define OPENGL 0
#define DIRECTX11 1

//#define RENDER_API OPENGL
#define RENDER_API OPENGL

#ifdef STS_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif // !NOMINMAX
	#include <Windows.h>

	#include <wrl.h> 

	#include <d3d11.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	#include <DirectXCollision.h>
#endif

#include "Steins/Core/MathTypes.h"