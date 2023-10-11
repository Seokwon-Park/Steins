#pragma once

#include "D3D11Context.h"
#include "Steins/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Steins
{
	struct BasicVertexConstantBuffer {
		XMMATRIX world;			//XMMATRIX float(4byte)*4*4 = 64byte 아무튼 16배수
		XMMATRIX invTranspose;
		XMMATRIX view;
		XMMATRIX projection;
	};

	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(const std::string& filepath);
		D3D11Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~D3D11Shader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, u32 count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return m_Name; }



		/*void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);*/
	private:
		//std::string ReadFile(const std::string& filepath);
		std::wstring GetFilepath(std::string filepath);
	private:
		D3D11Context* m_Context;
		ComPtr<ID3D11VertexShader> m_VertexShader;
		ComPtr<ID3D11PixelShader> m_PixelShader;
		ComPtr<ID3D11InputLayout> m_InputLayout;
		ComPtr<ID3D11Buffer> m_VertexConstant;
		u32 m_CbufferIndex;
		u32 m_RendererID;
		std::string m_Name;
	};
}
