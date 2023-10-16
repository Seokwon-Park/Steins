#include "stspch.h"
#include "D3D11Context.h"

//temp
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

#include <map>

namespace Steins
{

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;	\
	}
	namespace
	{
		std::string GetVendor(int vendorCode)
		{
			switch (vendorCode)
			{
			case 0x10DE: return "NVIDIA Corporation";
			case 0x1002: return "AMD Inc.";
			case 0x8086: return "Intel";
			case 0x1414: return "Microsoft";
			}
			STS_CORE_ERROR("Not a valid VendorID");
			return "";
		}
	} //anonymous namespace


	D3D11Context::D3D11Context(GLFWwindow* windowHandle, WindowProps windowProps)
		:m_GLFWHandle(windowHandle), m_WindowHandle(glfwGetWin32Window(windowHandle)), m_WindowProps(windowProps)
	{
		glfwMakeContextCurrent(windowHandle);

		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilBuffer = nullptr;

		STS_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	// 1. m_m_D3DDevice, m_m_D3DContext, 
	// 2. m_m_SwapChain,
	// 3. m_renderTargetView,
	// 4. m_screenViewport,
	// 5. m_rasterizerState
	// 6. m_depthStencilState
	void D3D11Context::Init()
	{
		// MSAA->CopyResource 문제로 일단 사용안하기
		m_MSAAEnabled = false;

		// DRIVER_TYPE ARRAY
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};

		u32 numDriverTypes = ARRAYSIZE(driverTypes);
		D3D_DRIVER_TYPE driverType;
		// DRIVERTYPE Test
		// If Succeed -> Init End.
		for (u32 driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
		{
			driverType = driverTypes[driverTypeIndex];
			// 1. m_m_D3DDevice, m_m_D3DContext, 
			HRESULT hr = D3D11CreateDevice(
				NULL,
				driverType,
				NULL,
				m_DebugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED,
				NULL,
				NULL,
				D3D11_SDK_VERSION,
				m_D3DDevice.GetAddressOf(),
				&m_D3DFeatureLevel,
				m_D3DContext.GetAddressOf());

			if (FAILED(hr))
			{
				STS_CORE_ERROR("Failed to Create m_D3DDevice!");
			}

			m_D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);

			DXGI_SWAP_CHAIN_DESC scd;
			ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

			scd.BufferDesc.Width = m_WindowProps.Width;
			scd.BufferDesc.Height = m_WindowProps.Height;
			scd.BufferDesc.RefreshRate.Numerator = 60;
			scd.BufferDesc.RefreshRate.Denominator = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
			scd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = 2;
			scd.OutputWindow = m_WindowHandle;
			//scd.Windowed = !m_WindowProps.fullscreen;
			scd.Windowed = true;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			IDXGIDevice* dxgiDevice = 0;
			IDXGIAdapter* dxgiAdapter = 0;
			IDXGIFactory* dxgiFactory = 0;

			m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
			dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
			dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

			dxgiFactory->CreateSwapChain(m_D3DDevice.Get(), &scd, m_SwapChain.GetAddressOf());

			//Here is For Get DX11 Version
			DXGI_ADAPTER_DESC adapterDescription; // Vendor
			SecureZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));
			char videoCardDescription[128]; // Renderer
			LARGE_INTEGER driverVersion; // Version

			// Vendor
			dxgiAdapter->GetDesc(&adapterDescription);
			dxgiAdapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

			// Renderer
			wcstombs_s(NULL, videoCardDescription, 128, adapterDescription.Description, 128);

			// Version
			std::string major, minor, release, build;
			major = std::to_string(HIWORD(driverVersion.HighPart));
			minor = std::to_string(LOWORD(driverVersion.HighPart));
			release = std::to_string(HIWORD(driverVersion.LowPart));
			build = std::to_string(LOWORD(driverVersion.LowPart));

			std::string version;
			version = major + "." + minor + "." + release + "." + build;

			dxgiFactory->Release();
			dxgiAdapter->Release();
			dxgiDevice->Release();


			//Set DX11 DebugMode
			if (m_DebugLayerEnabled)
			{
				m_D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_DebugLayer));
				m_DebugLayer->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
				ID3D11InfoQueue* infoQueue;
				m_D3DDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));
				D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
				D3D11_INFO_QUEUE_FILTER filter;
				memset(&filter, 0, sizeof(filter));
				filter.DenyList.NumIDs = 1;
				filter.DenyList.pIDList = hide;
				infoQueue->AddStorageFilterEntries(&filter);
			}

			Resize();

			STS_CORE_INFO("DirectX11 Info:");
			STS_CORE_INFO("  Vendor: {0}", GetVendor(adapterDescription.VendorId));
			STS_CORE_INFO("  Renderer: {0}", videoCardDescription);
			STS_CORE_INFO("  Version: {0}", version);			

			if (SUCCEEDED(hr))return;
		}
	}

	void D3D11Context::Resize()
	{
		u32 width = m_WindowProps.Width;
		u32 height = m_WindowProps.Height;

		m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		ID3D11Texture2D* backBuffer;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		assert(backBuffer);
		m_D3DDevice->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);

		m_Backbuffer = backBuffer;

		D3D11_TEXTURE2D_DESC textureDesc;
		backBuffer->GetDesc(&textureDesc);

		// 텍스처의 너비와 높이 변경
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage= D3D11_USAGE_DEFAULT;   // 새로운 너비

		m_D3DDevice->CreateTexture2D(&textureDesc, nullptr, &m_Backbuffer);

		backBuffer->Release();

		D3D11_DEPTH_STENCIL_DESC dsd = {};
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;

		m_D3DDevice->CreateDepthStencilState(&dsd, m_DepthStencilState.GetAddressOf());

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		m_D3DDevice->CreateTexture2D(&depthStencilDesc, 0, m_DepthStencilBuffer.GetAddressOf());
		m_D3DDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), 0, m_DepthStencilView.GetAddressOf());

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
		m_D3DDevice->CreateBlendState(&blendDesc, m_BlendState.GetAddressOf());

		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		m_D3DContext->RSSetViewports(1, &m_ScreenViewport);

		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		ID3D11RasterizerState* rs;
		m_D3DDevice->CreateRasterizerState(&rasterDesc, &rs);
		m_D3DContext->RSSetState(rs);
		ReleaseCOM(rs);
	}

	void D3D11Context::ReleaseCall()
	{
		m_SwapChain.Reset();
		m_Backbuffer.Reset();
		m_RenderTargetView.Reset();
		m_DepthStencilBuffer.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilState.Reset();
	}

	void D3D11Context::SetViewport(int width, int height)
	{
		// Set the viewport
		ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;
		m_ScreenViewport.MinDepth = -1.0f;
		m_ScreenViewport.MaxDepth = 1.0f;

		m_D3DContext->RSSetViewports(1, &m_ScreenViewport);
	}

	void D3D11Context::CreateRenderTargetView()
	{
		m_Backbuffer.Reset();
		m_RenderTargetView.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilBuffer.Reset();

		ComPtr<ID3D11Texture2D> backBuffer;
		m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (backBuffer) {
			m_D3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr,
				&m_RenderTargetView);
		}

		D3D11_TEXTURE2D_DESC bbDesc;
		backBuffer->GetDesc(&bbDesc);
		backBuffer.Reset();

		// 텍스처의 너비와 높이 변경
		bbDesc.SampleDesc.Count = 1;
		bbDesc.SampleDesc.Quality = 0;
		bbDesc.Usage = D3D11_USAGE_DEFAULT;
		m_D3DDevice->CreateTexture2D(&bbDesc, nullptr, &m_Backbuffer);
				
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = bbDesc.Width;
		depthStencilDesc.Height = bbDesc.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		m_D3DDevice->CreateTexture2D(&depthStencilDesc, 0, m_DepthStencilBuffer.GetAddressOf());
		m_D3DDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), 0, m_DepthStencilView.GetAddressOf());

	}

	void D3D11Context::SwapBuffers(bool VSync)
	{
		m_SwapChain->Present(VSync, 0);
	}
}