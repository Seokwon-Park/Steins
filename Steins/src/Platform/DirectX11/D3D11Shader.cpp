#include "stspch.h"
#include "D3D11Shader.h"

#include "Steins/Core/Application.h"

#include <fstream>

namespace Steins
{
	
	D3D11Shader::D3D11Shader(const std::string& filepath)
		:m_CbufferIndex(0)
	{
		ID3DBlob* vsBlob;

		ID3DBlob* shaderCompileErrorsBlob;
		HRESULT hResult = D3DCompileFromFile(GetFilepath(filepath).c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob, &shaderCompileErrorsBlob);
		if (FAILED(hResult))
		{
			const char* errorString = NULL;
			if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
				errorString = "Could not compile shader; file not found";
			else if (shaderCompileErrorsBlob) {
				errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
				shaderCompileErrorsBlob->Release();
			}
			MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
			return;
		}
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
		hResult = m_Context->GetD3DDevice()->
			CreateVertexShader(
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				nullptr,
				&m_VertexShader);
		assert(SUCCEEDED(hResult));

		m_Context->GetD3DDevice()->CreateInputLayout(m_Context->GetInputElements().data(), UINT(m_Context->GetInputElements().size()),
			vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
			&m_InputLayout);

		// Create Pixel Shader
		ID3DBlob* psBlob;
		hResult = D3DCompileFromFile(GetFilepath(filepath).c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, &shaderCompileErrorsBlob);
		if (FAILED(hResult))
		{
			const char* errorString = NULL;
			if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
				errorString = "Could not compile shader; file not found";
			else if (shaderCompileErrorsBlob) {
				errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
				shaderCompileErrorsBlob->Release();
			}
			MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
			return;
		}

		hResult = m_Context->GetD3DDevice()->
			CreatePixelShader(
				psBlob->GetBufferPointer(),
				psBlob->GetBufferSize(),
				nullptr,
				&m_PixelShader);
		assert(SUCCEEDED(hResult));
		psBlob->Release();

	}
	D3D11Shader::D3D11Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc)
		:m_CbufferIndex(0)
	{
		m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());

		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
		HRESULT hr = D3DCompile(
			vertexSrc.c_str(),
			vertexSrc.length(),
			nullptr,
			nullptr,
			nullptr,
			"vs_main", "vs_5_0",
			0, 0,
			&vsBlob,
			nullptr);

		hr = m_Context->GetD3DDevice()->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			&m_VertexShader);

		m_Context->GetD3DDevice()->CreateInputLayout(m_Context->GetInputElements().data(), UINT(m_Context->GetInputElements().size()),
			vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
			&m_InputLayout);

		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		hr = D3DCompile(
			pixelSrc.c_str(),
			pixelSrc.length(),
			nullptr,
			nullptr,
			nullptr,
			"ps_main", "ps_5_0",
			0, 0,
			&blob,
			nullptr);

		hr = m_Context->GetD3DDevice()->CreatePixelShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&m_PixelShader);
	}
	D3D11Shader::~D3D11Shader()
	{
	}
	void D3D11Shader::Bind() const
	{
		m_Context->GetD3DContext()->IASetInputLayout(m_InputLayout.Get());
		m_Context->GetD3DContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		m_Context->GetD3DContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}
	void D3D11Shader::Unbind() const
	{
		m_Context->GetD3DContext()->VSSetShader(nullptr, nullptr, 0);
		m_Context->GetD3DContext()->PSSetShader(nullptr, nullptr, 0);
	}
	void D3D11Shader::SetInt(const std::string& name, int value)
	{
	}
	void D3D11Shader::SetIntArray(const std::string& name, int* values, u32 count)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void D3D11Shader::SetFloat(const std::string& name, float value)
	{
	}
	void D3D11Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
	}
	void D3D11Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
	}
	void D3D11Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		//TODO:update constant buffer ex)..viewproj,
	}

	std::wstring D3D11Shader::GetFilepath(std::string filepath)
	{
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find(L"Steins");
		std::wstring wFilepath;
		wFilepath.assign(filepath.begin(), filepath.end());

		std::wstring newFilepath = std::wstring(buffer).substr(0, pos);
		std::wstring::size_type found = newFilepath.find(L"\\");

		while (found != std::wstring::npos) {
			newFilepath.replace(newFilepath.find(L"\\"), 1, L"/");
			found = newFilepath.find(L"\\");
		}

		newFilepath += std::wstring(L"Steins/Sandbox/") + wFilepath;
		return newFilepath;
	}
}