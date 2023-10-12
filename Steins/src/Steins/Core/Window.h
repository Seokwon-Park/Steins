#pragma once

#include "stspch.h"

#include "Base.h"
#include "Steins/Events/Event.h"
#include "Steins/Renderer/GraphicsContext.h"


namespace Steins
{
	struct WindowProps
	{
		std::string Title;
		u32 Width;
		u32 Height;

		WindowProps(const std::string& title = "Steins; Engine",
			u32 width = 1280,
			u32 height = 720)
			:Title(title), Width(width), Height(height)
		{

		}
	};

	// Interface represending a desktop system based Window
	class STEINS_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual inline GraphicsContext* GetContext() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}