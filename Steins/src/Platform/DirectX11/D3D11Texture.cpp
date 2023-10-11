#include "stspch.h"
#include "D3D11Texture.h"

#include "stb_image.h"

Steins::D3D11Texture2D::D3D11Texture2D(u32 width, u32 height)
{
}

Steins::D3D11Texture2D::D3D11Texture2D(const std::string& path)
{
	STS_PROFILE_FUNCTION();

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
	InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
	// InitData.SysMemSlicePitch = 0;

	/*device->CreateTexture2D(&txtDesc, &InitData, m_Texture.GetAddressOf());
	device->CreateShaderResourceView(m_Texture.Get(), nullptr,
		textureResourceView.GetAddressOf());




	STS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);*/

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
