#include "stspch.h"
#include "Entity.h"

namespace Steins
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
		
	}
}