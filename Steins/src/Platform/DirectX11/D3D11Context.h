#pragma once

#include "Steins/Renderer/GraphicsContext.h"

using namespace DirectX;
using namespace Microsoft::WRL;

struct GLFWwindow;

namespace Steins
{
	class D3D11Context : public GraphicsContext
	{


	public:
		D3D11Context(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		void SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view);
	public:
		IDXGISwapChain* swapChain;
		ID3D11Device* d3dDevice;
		ID3D11DeviceContext* d3dContext;
		ID3D11RenderTargetView* backBuffer;

	private:
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		u32 m_MSAAQuality = 0;
		bool m_MSAAEnabled;
		bool m_DebugLayerEnabled;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		D3D11_VIEWPORT m_ScreenViewport;
		ID3D11Texture2D* m_DepthStencilBuffer;

		ID3D11Debug* m_DebugLayer;

		HWND m_WindowHandle;
	};
}