#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdint.h>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <DirectXMath.h>

#include "Steins/Log.h"
#include "Steins/MathTypes.h"

//platforms

#ifdef STS_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif // !NOMINMAX
	#include <Windows.h>
#endif
