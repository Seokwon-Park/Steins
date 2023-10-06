#pragma once

#include "Steins/Window.h"

#include "Steins/Renderer/GraphicsContext.h"

using namespace DirectX;
using namespace Microsoft::WRL;

struct GLFWwindow;

namespace Steins
{
	class D3D11Context : public GraphicsContext
	{
	public:
		static D3D11Context* instance();
		D3D11Context(GLFWwindow* windowHandle, WindowProps windowProps);

		virtual void Init() override;
		virtual void SwapBuffers() override;

		ComPtr<ID3D11Device> GetD3DDevice() { return this->m_D3DDevice; }
		ComPtr<ID3D11DeviceContext> GetD3DContext() { return this->m_D3DContext; }
		ID3D11RenderTargetView* GetRTV() { return this->m_RenderTargetView; }
		ID3D11DepthStencilView* GetDSV() { return this->m_DepthStencilView; }

		void Resize();
	private:
		void SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view);
	public:

	private:
		static D3D11Context* m_Instance;
		
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		u32 m_MSAAQuality = 0;
		bool m_MSAAEnabled;
		bool m_DebugLayerEnabled;

		IDXGISwapChain* m_SwapChain;
		ComPtr<ID3D11Device> m_D3DDevice;
		ComPtr<ID3D11DeviceContext> m_D3DContext;
		ID3D11RenderTargetView* m_BackBuffer;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		D3D11_VIEWPORT m_ScreenViewport;
		ID3D11Texture2D* m_DepthStencilBuffer;

		ID3D11Debug* m_DebugLayer;

		GLFWwindow* m_GLFWHandle;
		HWND m_WindowHandle;
		WindowProps m_WindowProps;
	};
}