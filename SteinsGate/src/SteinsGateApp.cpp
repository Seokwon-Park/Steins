#include <Steins.h>
#include <Steins/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Steins
{
	class SteinsGate : public Application
	{
	public:
		SteinsGate()
			:Application("SteinsGate")
		{
			PushLayer(new EditorLayer());
		}

		~SteinsGate()
		{

		}
	};

	Application* CreateApplication()
	{
		return new SteinsGate();
	}
}