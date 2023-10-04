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

namespace Steins
{

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;	\
	}

	D3D11Context::D3D11Context(GLFWwindow* windowHandle, WindowProps windowProps)
		:m_WindowHandle(glfwGetWin32Window(windowHandle)), m_WindowProps(windowProps)
	{
		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilBuffer = nullptr;

		STS_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	// 1. m_d3dDevice, m_d3dContext, 
	// 2. m_swapChain,
	// 3. m_renderTargetView,
	// 4. m_screenViewport,
	// 5. m_rasterizerState
	// 6. m_depthStencilState
	void D3D11Context::Init()
	{
		m_MSAAEnabled = true;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};

		u32 numDriverTypes = ARRAYSIZE(driverTypes);
		D3D_DRIVER_TYPE driverType;

		for (u32 driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
		{
			driverType = driverTypes[driverTypeIndex];

			HRESULT hr = D3D11CreateDevice(NULL, driverType, NULL,
				m_DebugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED,
				NULL, NULL, D3D11_SDK_VERSION, &D3DDevice, &m_D3DFeatureLevel, &D3DContext);
			if (FAILED(hr)) continue;

			D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);

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

			D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
			dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
			dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
			dxgiFactory->CreateSwapChain(D3DDevice, &scd, &SwapChain);

			dxgiFactory->Release();
			dxgiAdapter->Release();
			dxgiDevice->Release();

			if (m_DebugLayerEnabled)
			{
				D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_DebugLayer));
				m_DebugLayer->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);

				ID3D11InfoQueue* infoQueue;
				D3DDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));
				D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
				D3D11_INFO_QUEUE_FILTER filter;
				memset(&filter, 0, sizeof(filter));
				filter.DenyList.NumIDs = 1;
				filter.DenyList.pIDList = hide;
				infoQueue->AddStorageFilterEntries(&filter);
			}

			Resize();

			//https://github.com/CybernetHacker14/Sentinel/tree/main/Engine/Source/Platform/ImGui 참고하라고 ㅋㅋ
			ImGui_ImplDX11_Init(D3DDevice, D3DContext);

			if (SUCCEEDED(hr))return;
		}
	}

	void D3D11Context::Resize()
	{
		u32 width = m_WindowProps.Width;
		u32 height = m_WindowProps.Height;

		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);

		SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		ID3D11Texture2D* backBuffer;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		D3DDevice->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		backBuffer->Release();

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

		D3DDevice->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer);
		D3DDevice->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView);
		SetRenderTargets(m_RenderTargetView, m_DepthStencilView);

		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		D3DContext->RSSetViewports(1, &m_ScreenViewport);

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
		D3DDevice->CreateRasterizerState(&rasterDesc, &rs);
		D3DContext->RSSetState(rs);
		ReleaseCOM(rs);
	}

	void D3D11Context::SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view)
	{
		D3DContext->OMSetRenderTargets(1, &target, view);
	}

	void D3D11Context::SwapBuffers()
	{
		SwapChain->Present(1, 0);
	}
}