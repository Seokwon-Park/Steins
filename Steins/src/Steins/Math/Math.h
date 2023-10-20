#pragma once

#include <glm/glm.hpp>

namespace Steins::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& ouatRotation, glm::vec3& outScale);
}