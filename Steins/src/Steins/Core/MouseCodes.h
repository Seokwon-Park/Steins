#pragma once

namespace Steins
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}
// From STS3.h
#define STS_MOUSE_BUTTON_1         ::Steins::Mouse::Button0 
#define STS_MOUSE_BUTTON_2         ::Steins::Mouse::Button1 
#define STS_MOUSE_BUTTON_3         ::Steins::Mouse::Button2 
#define STS_MOUSE_BUTTON_4         ::Steins::Mouse::Button3 
#define STS_MOUSE_BUTTON_5         ::Steins::Mouse::Button4 
#define STS_MOUSE_BUTTON_6         ::Steins::Mouse::Button5 
#define STS_MOUSE_BUTTON_7         ::Steins::Mouse::Button6 
#define STS_MOUSE_BUTTON_8         ::Steins::Mouse::Button7 
#define STS_MOUSE_BUTTON_LAST      ::Steins::Mouse::ButtonLast 
#define STS_MOUSE_BUTTON_LEFT      ::Steins::Mouse::ButtonLeft 
#define STS_MOUSE_BUTTON_RIGHT     ::Steins::Mouse::ButtonRight 
#define STS_MOUSE_BUTTON_MIDDLE    ::Steins::Mouse::ButtonMiddle 