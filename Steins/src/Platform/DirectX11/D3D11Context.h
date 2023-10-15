#pragma once

#include "Steins/Core/Window.h"

#include "Steins/Renderer/GraphicsContext.h"

using namespace DirectX;
using namespace Microsoft::WRL;

struct GLFWwindow;
struct WindowData;

namespace Steins
{
	class D3D11Context : public GraphicsContext
	{
	public:
		D3D11Context(GLFWwindow* windowHandle, WindowProps windowProps);

		virtual void Init() override;
		virtual void SwapBuffers(bool VSync) override;

		IDXGISwapChain* GetSwapChain() { return this->m_SwapChain; }
		ID3D11Device* GetD3DDevice() { return this->m_D3DDevice; }
		ID3D11DeviceContext* GetD3DContext() { return this->m_D3DContext; }
		ID3D11RenderTargetView* GetRTV() { return this->m_RenderTargetView; }
		ID3D11RenderTargetView** GetRTVAddr() { return &this->m_RenderTargetView; }
		
		ID3D11DepthStencilView* GetDSV() { return this->m_DepthStencilView.Get(); }
		ID3D11DepthStencilState* GetDSS() { return this->m_DepthStencilState.Get(); }		
		ID3D11BlendState* GetBS() { return this->m_BlendState.Get(); }		
		ID3D11Texture2D* GetBackbuffer() { return this->m_Backbuffer; }

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElements() { return m_InputElements; }
		void SetInputElements(std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements) { m_InputElements = inputElements; }

		void Resize();
		void ResizeSwapChain(u32 width, u32 height)
		{
			m_RenderTargetView = nullptr;
			m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			this->SetViewport(width, height);
			CreateRenderTargetView();
		}

	private:
		void SetViewport(int width, int height);
		void CreateRenderTargetView();
	public:

		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		u32 m_MSAAQuality = 0;
		bool m_MSAAEnabled;
		bool m_DebugLayerEnabled;

		IDXGISwapChain* m_SwapChain;
		ID3D11Device* m_D3DDevice;
		ID3D11DeviceContext* m_D3DContext;
		ID3D11Texture2D* m_Backbuffer;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputElements;

		ID3D11RenderTargetView* m_RenderTargetView;
		

		ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		D3D11_VIEWPORT m_ScreenViewport;

		ComPtr<ID3D11BlendState> m_BlendState;

		ID3D11Debug* m_DebugLayer;

		GLFWwindow* m_GLFWHandle;
		HWND m_WindowHandle;
		WindowProps m_WindowProps;
	};
}