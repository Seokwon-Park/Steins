#pragma once

#include "entt.hpp"

#include "Steins/Core/Timestep.h"

namespace Steins
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep dt);
		void OnViewportResize(u32 width, u32 height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		u32 m_ViewportWidth = 0, m_ViewportHeight = 0;
		
		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}