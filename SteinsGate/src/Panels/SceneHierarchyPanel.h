#pragma once

#include "Steins/Core/Base.h"
#include "Steins/Core/Log.h"
#include "Steins/Scene/Scene.h"
#include "Steins/Scene/Entity.h"

namespace Steins
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}