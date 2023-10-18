#pragma once

#include "Steins/Core/Base.h"
#include "Steins/Core/Log.h"
#include <filesystem>

#ifdef STS_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define STS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { STS##type##ERROR(msg, __VA_ARGS__); STS_DEBUGBREAK(); } }
#define STS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) STS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define STS_INTERNAL_ASSERT_NO_MSG(type, check) STS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", STS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define STS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define STS_INTERNAL_ASSERT_GET_MACRO(...) STS_EXPAND_MACRO( STS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, STS_INTERNAL_ASSERT_WITH_MSG, STS_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define STS_ASSERT(...) STS_EXPAND_MACRO( STS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define STS_CORE_ASSERT(...) STS_EXPAND_MACRO( STS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define STS_ASSERT(...)
#define STS_CORE_ASSERT(...)
#endif