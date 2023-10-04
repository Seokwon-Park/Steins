#pragma once

#include <string>

namespace Steins
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		u32 m_RendererID;

	};
}
