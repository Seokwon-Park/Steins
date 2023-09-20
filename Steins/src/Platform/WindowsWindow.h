#pragma once

#include "Steins/Window.h"

#include <GLFW/glfw3.h>

#include <d3dcompiler.h>
#include <d3d11.h>

#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Steins
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline u32 GetWidth() const override { return m_Data.Width; }
		inline u32 GetHeight() const override { return m_Data.Height; }

		//Window attr
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		bool InitWindow(const WindowProps& props);
		bool InitDirect3D11();

		bool CreateDeviceAndContext();
		DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc();
		bool CreateRenderTargetView();
		void SetViewport();
		bool CreateDepthBuffer();

		// for GLFW
		GLFWwindow* m_glfwWindow;

		//for Win32
		HWND m_Window;
		UINT m_numQualityLevels = 0;

		struct WindowData
		{
			std::string Title;
			u32 Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		
		// 1. ����̽�, ���ؽ�Ʈ
		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
		// 2. Swapchain
		ComPtr<IDXGISwapChain> m_swapChain;

		// 3. ����Ÿ�� ��(RTV)
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11RenderTargetView> m_indexRenderTargetView;
		ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

		// RenderTargetView���� ������ ���⿡ �׸�
		ComPtr<ID3D11Texture2D> m_tempTexture;
		// indexRenderTargetView�� �׸��� indexColor�� ǥ�õ� �̹����� ���⿡ �׸�.
		ComPtr<ID3D11Texture2D> m_indexTexture;

		// indexTexture�� �����ؼ� ������
		ComPtr<ID3D11Texture2D> m_indexTempTexture;
		// indexTempTexture���� 1x1ũ���� �ȼ��� �����Ѵ�.
		ComPtr<ID3D11Texture2D> m_indexStagingTexture;

		// 4. RasterizerState
		ComPtr<ID3D11RasterizerState> m_solidRasterizerState;
		ComPtr<ID3D11RasterizerState> m_wiredRasterizerState;
		bool m_drawAsWire = false;
		bool m_usePostProcessing = true;

		// Depth buffer ����
		ComPtr<ID3D11Texture2D> m_depthStencilBuffer;

		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;

		// 5. Viewport
		D3D11_VIEWPORT m_screenViewport;

		// ���� ���۾���.
		D3D11_VIEWPORT m_screenViewport2;


	
	};
}