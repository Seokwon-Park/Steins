#include "stspch.h"
#include "D3D11Context.h"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

namespace Steins
{
	namespace{
		// Try to Create driverTypes
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};

		UINT numDriverTypes = ARRAYSIZE(driverTypes);

	}// anonymous namespace

	D3D11Context::D3D11Context(GLFWwindow* windowHandle)
		:m_WindowHandle(glfwGetWin32Window(windowHandle))
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

		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;

		for (u32 driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
		{
			driverType = driverTypes[driverTypeIndex];
		}

		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		const D3D_FEATURE_LEVEL featureLevels[3] = {
			D3D_FEATURE_LEVEL_11_1, // 더 높은 버전이 먼저 오도록 설정
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_9_3 };

		D3D_FEATURE_LEVEL featureLevel;

		//if (FAILED(D3D11CreateDevice(
		//	nullptr,					// Specify nullptr to use the default adapter.
		//	driverType,					// Create a device using the hardware graphics driver.
		//	0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		//	createDeviceFlags,			// Set debug and Direct2D compatibility flags.
		//	featureLevels,				// List of feature levels this app can support.
		//	ARRAYSIZE(featureLevels),	// Size of the list above.
		//	D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
		//	device.GetAddressOf(),      // Returns the Direct3D device created.
		//	&featureLevel,				// Returns feature level of device created.
		//	context.GetAddressOf()      // Returns the device immediate context.
		//))) {
		//	std::cout << "D3D11CreateDevice() failed.\n";
		//	return;
		//}

	}

	void D3D11Context::SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view)
	{
		d3dContext->OMSetRenderTargets(1, &target, view);
	}

	void D3D11Context::SwapBuffers()
	{
		swapChain->Present(1, 0);
	}
}