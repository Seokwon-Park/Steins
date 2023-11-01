#pragma once

#include "entt.hpp"

#include "Steins/Core/Timestep.h"
#include "Steins/Core/UUID.h"
#include "Steins/Renderer/EditorCamera.h"

class b2World;

namespace Steins
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateEditor(Timestep dt, EditorCamera& camera);
		void OnUpdateRuntime(Timestep dt);
		void OnViewportResize(u32 width, u32 height);

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		u32 m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;
		
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}