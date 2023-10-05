#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Steins
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		u32 m_RendererID;

	};
}
