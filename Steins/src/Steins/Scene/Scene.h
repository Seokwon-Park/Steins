#pragma once

#include "entt.hpp"

#include "Steins/Core/Timestep.h"

namespace Steins
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		//temp
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep dt);
	private:
		entt::registry m_Registry;
	};
}