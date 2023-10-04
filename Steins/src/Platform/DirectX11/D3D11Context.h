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
		D3D11Context(GLFWwindow* windowHandle, WindowProps windowProps);

		virtual void Init() override;
		virtual void SwapBuffers() override;

		void Resize();
	private:
		void SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view);
	public:
		IDXGISwapChain* SwapChain;
		ID3D11Device* D3DDevice;
		ID3D11DeviceContext* D3DContext;
		ID3D11RenderTargetView* BackBuffer;

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
		WindowProps m_WindowProps;
	};
}