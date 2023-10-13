#pragma once

namespace Steins
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}



// From glfw3.h
#define STS_KEY_SPACE              ::Steins::Key::Space 
#define STS_KEY_APOSTROPHE         ::Steins::Key::Apostrophe /* ' */
#define STS_KEY_COMMA              ::Steins::Key::Comma /* , */
#define STS_KEY_MINUS              ::Steins::Key::Minus /* - */
#define STS_KEY_PERIOD             ::Steins::Key::Period /* . */
#define STS_KEY_SLASH              ::Steins::Key::Slash /* / */
#define STS_KEY_0                  ::Steins::Key::D0 
#define STS_KEY_1                  ::Steins::Key::D1 
#define STS_KEY_2                  ::Steins::Key::D2 
#define STS_KEY_3                  ::Steins::Key::D3 
#define STS_KEY_4                  ::Steins::Key::D4 
#define STS_KEY_5                  ::Steins::Key::D5 
#define STS_KEY_6                  ::Steins::Key::D6 
#define STS_KEY_7                  ::Steins::Key::D7 
#define STS_KEY_8                  ::Steins::Key::D8 
#define STS_KEY_9                  ::Steins::Key::D9 
#define STS_KEY_SEMICOLON          ::Steins::Key::Semicolon /* ; */
#define STS_KEY_EQUAL              ::Steins::Key::Equal /* = */
#define STS_KEY_A                  ::Steins::Key::A 
#define STS_KEY_B                  ::Steins::Key::B 
#define STS_KEY_C                  ::Steins::Key::C 
#define STS_KEY_D                  ::Steins::Key::D 
#define STS_KEY_E                  ::Steins::Key::E 
#define STS_KEY_F                  ::Steins::Key::F 
#define STS_KEY_G                  ::Steins::Key::G 
#define STS_KEY_H                  ::Steins::Key::H 
#define STS_KEY_I                  ::Steins::Key::I 
#define STS_KEY_J                  ::Steins::Key::J 
#define STS_KEY_K                  ::Steins::Key::K 
#define STS_KEY_L                  ::Steins::Key::L 
#define STS_KEY_M                  ::Steins::Key::M 
#define STS_KEY_N                  ::Steins::Key::N 
#define STS_KEY_O                  ::Steins::Key::O 
#define STS_KEY_P                  ::Steins::Key::P 
#define STS_KEY_Q                  ::Steins::Key::Q 
#define STS_KEY_R                  ::Steins::Key::R 
#define STS_KEY_S                  ::Steins::Key::S 
#define STS_KEY_T                  ::Steins::Key::T 
#define STS_KEY_U                  ::Steins::Key::U 
#define STS_KEY_V                  ::Steins::Key::V 
#define STS_KEY_W                  ::Steins::Key::W 
#define STS_KEY_X                  ::Steins::Key::X 
#define STS_KEY_Y                  ::Steins::Key::Y 
#define STS_KEY_Z                  ::Steins::Key::Z 
#define STS_KEY_LEFT_BRACKET       ::Steins::Key::LeftBracket /* [ */
#define STS_KEY_BACKSLASH          ::Steins::Key::Backslash /* \ */
#define STS_KEY_RIGHT_BRACKET      ::Steins::Key::RightBracket /* ] */
#define STS_KEY_GRAVE_ACCENT       ::Steins::Key::GraveAccent /* ` */
#define STS_KEY_WORLD_1            ::Steins::Key::World1 /* non-US #1 */
#define STS_KEY_WORLD_2            ::Steins::Key::World2 /* non-US #2 */

/* Function keys */
#define STS_KEY_ESCAPE             ::Steins::Key::Escape 
#define STS_KEY_ENTER              ::Steins::Key::Enter 
#define STS_KEY_TAB                ::Steins::Key::Tab 
#define STS_KEY_BACKSPACE          ::Steins::Key::Backspace 
#define STS_KEY_INSERT             ::Steins::Key::Insert 
#define STS_KEY_DELETE             ::Steins::Key::Delete 
#define STS_KEY_RIGHT              ::Steins::Key::Right 
#define STS_KEY_LEFT               ::Steins::Key::Left 
#define STS_KEY_DOWN               ::Steins::Key::Down 
#define STS_KEY_UP                 ::Steins::Key::Up 
#define STS_KEY_PAGE_UP            ::Steins::Key::PageUp 
#define STS_KEY_PAGE_DOWN          ::Steins::Key::PageDown 
#define STS_KEY_HOME               ::Steins::Key::Home 
#define STS_KEY_END                ::Steins::Key::End 
#define STS_KEY_CAPS_LOCK          ::Steins::Key::CapsLock 
#define STS_KEY_SCROLL_LOCK        ::Steins::Key::ScrollLock 
#define STS_KEY_NUM_LOCK           ::Steins::Key::NumLock 
#define STS_KEY_PRINT_SCREEN       ::Steins::Key::PrintScreen 
#define STS_KEY_PAUSE              ::Steins::Key::Pause 
#define STS_KEY_F1                 ::Steins::Key::F1 
#define STS_KEY_F2                 ::Steins::Key::F2 
#define STS_KEY_F3                 ::Steins::Key::F3 
#define STS_KEY_F4                 ::Steins::Key::F4 
#define STS_KEY_F5                 ::Steins::Key::F5 
#define STS_KEY_F6                 ::Steins::Key::F6 
#define STS_KEY_F7                 ::Steins::Key::F7 
#define STS_KEY_F8                 ::Steins::Key::F8 
#define STS_KEY_F9                 ::Steins::Key::F9 
#define STS_KEY_F10                ::Steins::Key::F10 
#define STS_KEY_F11                ::Steins::Key::F11 
#define STS_KEY_F12                ::Steins::Key::F12 
#define STS_KEY_F13                ::Steins::Key::F13 
#define STS_KEY_F14                ::Steins::Key::F14 
#define STS_KEY_F15                ::Steins::Key::F15 
#define STS_KEY_F16                ::Steins::Key::F16 
#define STS_KEY_F17                ::Steins::Key::F17 
#define STS_KEY_F18                ::Steins::Key::F18 
#define STS_KEY_F19                ::Steins::Key::F19 
#define STS_KEY_F20                ::Steins::Key::F20 
#define STS_KEY_F21                ::Steins::Key::F21 
#define STS_KEY_F22                ::Steins::Key::F22 
#define STS_KEY_F23                ::Steins::Key::F23 
#define STS_KEY_F24                ::Steins::Key::F24 
#define STS_KEY_F25                ::Steins::Key::F25 
#define STS_KEY_KP_0               ::Steins::Key::KP0
#define STS_KEY_KP_1               ::Steins::Key::KP1
#define STS_KEY_KP_2               ::Steins::Key::KP2
#define STS_KEY_KP_3               ::Steins::Key::KP3
#define STS_KEY_KP_4               ::Steins::Key::KP4
#define STS_KEY_KP_5               ::Steins::Key::KP5
#define STS_KEY_KP_6               ::Steins::Key::KP6
#define STS_KEY_KP_7               ::Steins::Key::KP7
#define STS_KEY_KP_8               ::Steins::Key::KP8
#define STS_KEY_KP_9               ::Steins::Key::KP9
#define STS_KEY_KP_DECIMAL         ::Steins::Key::KPDecimal
#define STS_KEY_KP_DIVIDE          ::Steins::Key::KPDivide
#define STS_KEY_KP_MULTIPLY        ::Steins::Key::KPMultiply
#define STS_KEY_KP_SUBTRACT        ::Steins::Key::KPSubtract
#define STS_KEY_KP_ADD             ::Steins::Key::KPAdd
#define STS_KEY_KP_ENTER           ::Steins::Key::KPEnter
#define STS_KEY_KP_EQUAL           ::Steins::Key::KPEqual
#define STS_KEY_LEFT_SHIFT         ::Steins::Key::LeftShift
#define STS_KEY_LEFT_CONTROL       ::Steins::Key::LeftControl
#define STS_KEY_LEFT_ALT           ::Steins::Key::LeftAlt
#define STS_KEY_LEFT_SUPER         ::Steins::Key::LeftSuper
#define STS_KEY_RIGHT_SHIFT        ::Steins::Key::RightShift
#define STS_KEY_RIGHT_CONTROL      ::Steins::Key::RightControl
#define STS_KEY_RIGHT_ALT          ::Steins::Key::RightAlt
#define STS_KEY_RIGHT_SUPER        ::Steins::Key::RightSuper
#define STS_KEY_MENU               ::Steins::Key::Menu
#define STS_KEY_LAST               STS_KEY_LAST