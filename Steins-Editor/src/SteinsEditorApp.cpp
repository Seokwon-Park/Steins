#include <Steins.h>
#include <Steins/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Steins
{
	class SteinsEditor : public Application
	{
	public:
		SteinsEditor()
			:Application("Steins Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SteinsEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new SteinsEditor();
	}
}