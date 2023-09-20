#include "stspch.h"
#include "WindowsWindow.h"

#include "Steins/Events/KeyEvent.h"
#include "Steins/Events/MouseEvent.h"
#include "Steins/Events/ApplicationEvent.h"

#include <glad/glad.h>

namespace Steins
{

	namespace
	{
		static bool s_GLFWInitialized = false;

		static void GLFWErrorCallback(int error , const char* description)
		{
			STS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		}

		WindowsWindow* g_WindowsBase = nullptr;

		// RegisterClassEX()에 실제로 등록될 콜백함수
		LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			// g_appBase를 이용해서 간접적으로 멤버 함수 호출
			return g_WindowsBase->MsgProc(hwnd, msg, wParam, lParam);
		}

	}//anonymous namespace

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		std::wstring title(m_Data.Title.begin(), m_Data.Title.end());

		STS_CORE_INFO("Create Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

#ifndef STS_PLATFORM_WINDOWS
		if (!InitWindow(props))
		{			
			STS_CORE_ASSERT(InitWindow(props), "Failed to Initialize Window");
			return;
		}

		if (!InitDirect3D11())
		{
			STS_CORE_ASSERT(InitWindow(props), "Failed to Initialize Direct3D11");
			return;
		}



#else

		//glfw
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			STS_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_glfwWindow = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_glfwWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		STS_CORE_ASSERT(status, "Failed to initialize Glad!");
		glfwSetWindowUserPointer(m_glfwWindow, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_glfwWindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
#endif
	}

	void WindowsWindow::Shutdown()
	{
		//DestroyWindow(m_Window);
		glfwDestroyWindow(m_glfwWindow);
	}

	void WindowsWindow::OnUpdate()
	{
#ifndef STS_PLATFORM_WINDOWS
		m_d3dContext->RSSetViewports(1, &m_screenViewport);

		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 검은색으로 클리어
		m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
		m_d3dContext->ClearRenderTargetView(m_indexRenderTargetView.Get(), clearColor);

		m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* rtvs[] = {
			m_renderTargetView.Get(),
			m_indexRenderTargetView.Get()
		};

		m_d3dContext->OMSetRenderTargets(2, rtvs, m_depthStencilView.Get());
		m_d3dContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

		if (m_drawAsWire)
			m_d3dContext->RSSetState(m_wiredRasterizerState.Get());
		else
			m_d3dContext->RSSetState(m_solidRasterizerState.Get());



		//if (m_drawNormals) {
		//	m_d3dContext->VSSetShader(m_normalVertexShader.Get(), 0, 0);

		//	m_d3dContext->VSSetConstantBuffers(1, 1, m_normalLines->vertexConstantBuffer.GetAddressOf());

		//	m_d3dContext->PSSetShader(m_normalPixelShader.Get(), 0, 0);
		//	// m_d3dContext->IASetInputLayout(m_basicInputLayout.Get());
		//	m_d3dContext->IASetVertexBuffers(
		//		0, 1, m_normalLines->vertexBuffer.GetAddressOf(), &stride,
		//		&offset);
		//	m_d3dContext->IASetIndexBuffer(m_normalLines->indexBuffer.Get(),
		//		DXGI_FORMAT_R32_UINT, 0);

		//	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		//	m_d3dContext->DrawIndexed(m_normalLines->m_indexCount, 0, 0);
		//}

		//m_billboardPoints.Render(m_d3dContext);


		ComPtr<ID3D11Texture2D> backBuffer;
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		m_d3dContext->ResolveSubresource(m_tempTexture.Get(), 0, backBuffer.Get(), 0,
			DXGI_FORMAT_R8G8B8A8_UNORM);
		
		m_swapChain->Present(1, 0);
#else
		glfwPollEvents();
		glfwSwapBuffers(m_glfwWindow);
#endif
	}
	
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	bool WindowsWindow::InitWindow(const WindowProps& props)
	{
		std::wstring wTitle(props.Title.begin(), props.Title.end());

		WNDCLASSEX wc = { sizeof(WNDCLASSEX),
						CS_CLASSDC,
						WndProc,
						0L,
						0L,
						GetModuleHandle(NULL),
						NULL,
						NULL,
						NULL,
						NULL,
						L"SteinsEngine", // lpszClassName, L-string
						NULL };

		// The RegisterClass function has been superseded by the RegisterClassEx function.
		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
		if (!RegisterClassEx(&wc)) {
			std::cout << "RegisterClassEx() failed.\n";
			return false;
		}

		// 원하는 크기의 해상도(툴바 제외)
		RECT windowRect = { 0, 0, props.Width, props.Height };

		// 필요한 윈도우 크기(해상도) 계산 (툴바를 포함)
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

		// 윈도우를 만들때 위에서 계산한 windowRect 사용
		m_Window = CreateWindow(
			wc.lpszClassName,
			wTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			//윈도우의 생성 위치
			100,                // 윈도우 좌측 상단의 x 좌표
			100,                // 윈도우 좌측 상단의 y 좌표

			// 툴바를 포함한 실제 윈도우의 크기 계산
			windowRect.right - windowRect.left, // 윈도우 가로 방향 해상도
			windowRect.bottom - windowRect.top, // 윈도우 세로 방향 해상도
			NULL, NULL, wc.hInstance, NULL);

		if (!m_Window) {
			std::cout << "CreateWindow() failed.\n";
			return false;
		}

		// 윈도우 표시
		ShowWindow(m_Window, SW_SHOWDEFAULT);
		UpdateWindow(m_Window);

		return true;
	}

	bool WindowsWindow::InitDirect3D11()
	{
		// 1. m_d3dDevice, m_d3dContext, 
		// 2. m_swapChain,
		// 3. m_renderTargetView,
		// 4. m_screenViewport,
		// 5. m_rasterizerState
		// 6. m_depthStencilState

		// 1. m_d3dDevice와 m_d3dContext 생성
		// 2. 내부에서 m_SwapChain도 같이 생성
		if (!CreateDeviceAndContext())
			return false;

		// 3. CreateRenderTargetView m_renderTargetView 생성
		if (!CreateRenderTargetView())
			return false;

		// 4. CreateViewport m_screenViewport 생성
		SetViewport();

		// 5. Create a solid rasterizer state
		D3D11_RASTERIZER_DESC rastDesc;
		ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
		rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		// rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rastDesc.FrontCounterClockwise = false;
		rastDesc.DepthClipEnable = true;

		m_d3dDevice->CreateRasterizerState(&rastDesc, m_solidRasterizerState.GetAddressOf());

		// Create a wired rasterizer state
		rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		m_d3dDevice->CreateRasterizerState(&rastDesc, m_wiredRasterizerState.GetAddressOf());

		// Create depth buffer
		CreateDepthBuffer();

		// Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = true; // false
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		if (FAILED(m_d3dDevice->CreateDepthStencilState(&depthStencilDesc,
			m_depthStencilState.GetAddressOf()))) {
			std::cout << "CreateDepthStencilState() failed.\n";
			return false;
		}

		return true;
	}

	LRESULT WindowsWindow::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		//if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		//	return true;


		switch (msg) {
		case WM_SIZE:

			break;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_MOUSEMOVE:
			//std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
			break;
		case WM_LBUTTONDOWN:
			std::cout << "WM_LBUTTONUP Left mouse button down\n";
			
			break;
		case WM_LBUTTONUP:
			std::cout << "WM_LBUTTONUP Left mouse button up\n";
			std::cout << MK_LBUTTON << std::endl;
			break;
		case WM_RBUTTONDOWN:
			std::cout << "WM_RBUTTONUP Right mouse button\n";
			std::cout << MK_RBUTTON << std::endl;
			break;
		case WM_RBUTTONUP:
			std::cout << "WM_RBUTTONUP Right mouse button\n";
			
			break;
		case WM_MBUTTONDOWN:
			std::cout << VK_MBUTTON << std::endl;
			break;
		case WM_KEYDOWN:
			std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;		
		
			break;
		case WM_KEYUP:
		
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}

		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool WindowsWindow::CreateDeviceAndContext()
	{
		// 그래픽카드 사용하도록 설정
		const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

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

		if (FAILED(D3D11CreateDevice(
			nullptr,					// Specify nullptr to use the default adapter.
			driverType,					// Create a device using the hardware graphics driver.
			0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
			createDeviceFlags,			// Set debug and Direct2D compatibility flags.
			featureLevels,				// List of feature levels this app can support.
			ARRAYSIZE(featureLevels),	// Size of the list above.
			D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
			device.GetAddressOf(),      // Returns the Direct3D device created.
			&featureLevel,				// Returns feature level of device created.
			context.GetAddressOf()      // Returns the device immediate context.
		))) {
			std::cout << "D3D11CreateDevice() failed.\n";
			return false;
		}

		//DX11을 지원하는지 확인
		if (!(featureLevel == D3D_FEATURE_LEVEL_11_1 || featureLevel == D3D_FEATURE_LEVEL_11_0)) {
			std::cout << "D3D Feature Level 11 unsupported.\n";
			return false;
		}

		// 참고: Immediate vs deferred context
		// A deferred context is primarily used for multithreading and is not necessary for a
		// single-threaded application.
		// https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context

		// MultiSample Anti-Aliasing
		// 4X MSAA 지원하는지 확인
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_numQualityLevels);
		if (m_numQualityLevels <= 0) {
			std::cout << "MSAA not supported.\n";
		}
		// m_numQualityLevels = 0 // MSAA 끄기

		DXGI_SWAP_CHAIN_DESC sd = CreateSwapChainDesc();

		if (FAILED(D3D11CreateDeviceAndSwapChain(
			0, // Default adapter
			driverType,
			0, // No software device
			createDeviceFlags,
			featureLevels,
			1,
			D3D11_SDK_VERSION,
			&sd,
			m_swapChain.GetAddressOf(),
			m_d3dDevice.GetAddressOf()
			, &featureLevel,
			m_d3dContext.GetAddressOf())))
		{
			return false;
		}
			

		return true;
	}


	DXGI_SWAP_CHAIN_DESC WindowsWindow::CreateSwapChainDesc()
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = m_Data.Width;               // set the back buffer width
		sd.BufferDesc.Height = m_Data.Height;             // set the back buffer height
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
		sd.BufferCount = 2;                                // Double-buffering
		sd.BufferDesc.RefreshRate.Numerator = 60;          // FPS
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
		sd.OutputWindow = m_Window;                    // the window to be used
		sd.Windowed = TRUE;                                // windowed/full-screen mode
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//MSAA 사용하는지 안하는지 확인
		if (m_numQualityLevels > 0) {
			sd.SampleDesc.Count = 4; // how many multisamples
			sd.SampleDesc.Quality = m_numQualityLevels - 1;
		}
		else {
			sd.SampleDesc.Count = 1; // how many multisamples
			sd.SampleDesc.Quality = 0;
		}

		return sd;
	}


	bool WindowsWindow::CreateRenderTargetView()
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		if (backBuffer) {
			m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr,
				m_renderTargetView.GetAddressOf());

			D3D11_TEXTURE2D_DESC desc;
			backBuffer->GetDesc(&desc);
			// 디버깅용
			// cout << desc.Width << " " << desc.Height << " " << desc.Format <<
			// endl;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = 0;

			if (FAILED(m_d3dDevice->CreateTexture2D(
				&desc, nullptr, m_tempTexture.GetAddressOf()))) {
				std::cout << "Failed()\n";
			}

			if (FAILED(m_d3dDevice->CreateTexture2D(
				&desc, nullptr, m_indexTempTexture.GetAddressOf()))) {
				std::cout << "Failed()\n";
			}

			m_d3dDevice->CreateShaderResourceView(m_tempTexture.Get(), nullptr,
				m_shaderResourceView.GetAddressOf());

			// ShaderResource를 (backBuffer가 아니라) tempTexture로부터 생성 
			//m_d3dDevice->CreateShaderResourceView(m_indexTempTexture.Get(), nullptr,
			//	m_shaderResourceView.GetAddressOf());

			// 1x1 작은 스테이징 텍스춰 만들기
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.Width = 1;
			desc.Height = 1;

			if (FAILED(m_d3dDevice->CreateTexture2D(
				&desc, nullptr, m_indexStagingTexture.GetAddressOf()))) {
				std::cout << "Failed()\n";
			}

			// 마우스 피킹에 사용할 인덱스 색을 렌더링할 텍스쳐와 렌더타겟 생성
			backBuffer->GetDesc(&desc); // BackBuffer와 동일한 설정
			if (FAILED(m_d3dDevice->CreateTexture2D(&desc, nullptr,
				m_indexTexture.GetAddressOf()))) {
				std::cout << "Failed()\n";
			}
			m_d3dDevice->CreateRenderTargetView(
				m_indexTexture.Get(), nullptr,
				m_indexRenderTargetView.GetAddressOf());

		}
		else {
			std::cout << "CreateRenderTargetView() failed." << std::endl;
			return false;
		}
		return true;
	}

	void WindowsWindow::SetViewport()
	{
		//초기값을 0보다 작게 설정
		static float previousGuiWidth = -1.f;
		if (previousGuiWidth != 0)
		{
			previousGuiWidth = 0;

			// Set the viewport
			ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
			m_screenViewport.TopLeftX = 0;
			m_screenViewport.TopLeftY = 0;
			m_screenViewport.Width = float(m_Data.Width);
			m_screenViewport.Height = float(m_Data.Height);
			//m_screenViewport.Width = static_cast<float>(m_screenHeight);
			m_screenViewport.MinDepth = 0.0f;
			m_screenViewport.MaxDepth = 1.0f;
		}
	}

	bool WindowsWindow::CreateDepthBuffer()
	{
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = m_Data.Width;
		depthStencilBufferDesc.Height = m_Data.Height;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (m_numQualityLevels > 0) {
			depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
			depthStencilBufferDesc.SampleDesc.Quality = m_numQualityLevels - 1;
		}
		else {
			depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
			depthStencilBufferDesc.SampleDesc.Quality = 0;
		}
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		if (FAILED(m_d3dDevice->CreateTexture2D(
			&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf()))) {
			std::cout << "CreateTexture2D() failed.\n";
		}
		if (FAILED(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0,
			&m_depthStencilView))) {
			std::cout << "CreateDepthStencilView() failed.\n";
		}
		return true;
	}
}