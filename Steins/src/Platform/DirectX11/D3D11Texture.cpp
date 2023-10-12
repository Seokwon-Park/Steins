#include "stspch.h"
#include "D3D11Texture.h"

#include "stb_image.h"
#include "Steins/Core/Application.h"

Steins::D3D11Texture2D::D3D11Texture2D(u32 width, u32 height)
{
	m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());

	m_Width = width;
	m_Height = height;

	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = m_Width;
	txtDesc.Height = m_Height;
	txtDesc.MipLevels = 1;
	txtDesc.ArraySize = 1;
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_DYNAMIC;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	txtDesc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE;

	m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, nullptr, m_Texture.GetAddressOf());
	m_Context->GetD3DDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr, m_TextureResourceView.GetAddressOf());
}

Steins::D3D11Texture2D::D3D11Texture2D(const std::string& path)
{
	STS_PROFILE_FUNCTION();

	m_Context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = nullptr;
	{
		STS_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	}
	STS_CORE_ASSERT(data, "Failed to load image!");
	m_Width = width;
	m_Height = height;

	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = m_Width;
	txtDesc.Height = m_Height;
	txtDesc.MipLevels = 1;
	txtDesc.ArraySize = 1;
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	//InitData.pSysMem = data;
	InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
	int internalFormat = 0, dataFormat = 0;
	std::vector<uint8_t> newData;
	if (channels == 4)
	{
		InitData.pSysMem = data;
	}
	else if (channels == 3)
	{


		newData.resize(width * height * 4);
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 3; c++) {
				newData[4 * i + c] = data[i * channels + c];
			}
			newData[4 * i + 3] = 255;
		}

		InitData.pSysMem = newData.data();
	}

	m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, &InitData, m_Texture.GetAddressOf());
	m_Context->GetD3DDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr, m_TextureResourceView.GetAddressOf());
	//m_Context->GetD3DContext()->PSSetShaderResources(0, 1, m_TextureResourceView.GetAddressOf());

	stbi_image_free(data);
}

Steins::D3D11Texture2D::~D3D11Texture2D()
{
}

void Steins::D3D11Texture2D::SetData(void* data, u32 size)
{
	// 데이터 업데이트
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_Context->GetD3DContext()->Map(m_Texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// 데이터 복사
	memcpy(mappedResource.pData, data, size);
	// 언맵 (ComPtr이라 필요 X)
	m_Context->GetD3DContext()->Unmap(m_Texture.Get(), 0);

}

void Steins::D3D11Texture2D::Bind(u32 slot) const
{
	m_Context->GetD3DContext()->PSSetShaderResources(slot, 1, m_TextureResourceView.GetAddressOf());
}


