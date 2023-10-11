#include "stspch.h"
#include "D3D11Texture.h"

#include "stb_image.h"
#include "Steins/Core/Application.h"

Steins::D3D11Texture2D::D3D11Texture2D(u32 width, u32 height)
{
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

	int internalFormat = 0, dataFormat = 0;
	if (channels == 4)
	{
		internalFormat = 4;
		dataFormat = 4;
	}
	else if (channels == 3)
	{
		internalFormat = 3;
		dataFormat = 3;
	}


	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = m_Width;
	txtDesc.Height = m_Height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * internalFormat;
	// InitData.SysMemSlicePitch = 0;

	m_Context->GetD3DDevice()->CreateTexture2D(&txtDesc, &InitData, m_Texture.GetAddressOf());
	m_Context->GetD3DDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr,
		m_TextureResourceView.GetAddressOf());

	STS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
	
	stbi_image_free(data);
}

Steins::D3D11Texture2D::~D3D11Texture2D()
{
}

void Steins::D3D11Texture2D::SetData(void* data, u32 size)
{
}

void Steins::D3D11Texture2D::Bind(u32 slot) const
{
}
