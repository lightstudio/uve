#pragma once


#include "BasicTimer.h"
#include <SDL.h>

#include <DrawingSurfaceNative.h>
#include <XDLRenderer.h>
#include "ONScripter.h"
#include <SharedData.h>
#include "INativeCalls.h"

using namespace SDL_Render;
//struct SDL_VertexShaderConstants
//{
//    DirectX::XMFLOAT4X4 model;
//    DirectX::XMFLOAT4X4 view;
//    DirectX::XMFLOAT4X4 projection;
//};

//typedef struct
//{
//    Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice;
//    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext;
//    //Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
//    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mainRenderTargetView;
//    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> currentOffscreenRenderTargetView;
//    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
//    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> texturePixelShader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> colorPixelShader;
//    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeBlend;
//    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeAdd;
//    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeMod;
//    Microsoft::WRL::ComPtr<ID3D11SamplerState> mainSampler;
//    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mainRasterizer;
//    D3D_FEATURE_LEVEL featureLevel;
//
//    // Vertex buffer constants:
//    SDL_VertexShaderConstants vertexShaderConstantsData;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexShaderConstants;
//
//    // Cached renderer properties.
//    DirectX::XMFLOAT2 windowSizeInDIPs;
//    DirectX::XMFLOAT2 renderTargetSize;
//    Windows::Graphics::Display::DisplayOrientations orientation;
//
//    // Transform used for display orientation.
//    DirectX::XMFLOAT4X4 orientationTransform3D;
//} D3D11_RenderData;

namespace UVEngineNative
{
	public delegate void RequestAdditionalFrameHandler();
	public delegate void RecreateSynchronizedTextureHandler();

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Direct3DInterop sealed : public Windows::Phone::Input::Interop::IDrawingSurfaceManipulationHandler
	{
	public:
		Direct3DInterop(Platform::String^ gamepath);
		
		Windows::Phone::Graphics::Interop::IDrawingSurfaceContentProvider^ CreateContentProvider();

		// IDrawingSurfaceManipulationHandler
		virtual void SetManipulationHost(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ manipulationHost);

		event RequestAdditionalFrameHandler^ RequestAdditionalFrame;
		event RecreateSynchronizedTextureHandler^ RecreateSynchronizedTexture;

		property Windows::Foundation::Size WindowBounds;
		property Windows::Foundation::Size NativeResolution;
		property Windows::Foundation::Size RenderResolution
		{
			Windows::Foundation::Size get(){ return m_renderResolution; }
			void set(Windows::Foundation::Size renderResolution);
		}
		void InitGlobalCallback(INativeCalls^ inc);
		void BackKeyPressed();
		void Exited();
		void DeActivated();
		void Activated();
		void CtrlPressed();
		void CtrlReleased();
		void Quit();
		void Save();
		void Load();
		void SetONSResolutionAndDisplayMode(int devicew,int deviceh,int xamlw,int xamlh,int flags);
	protected:
		// 事件处理程序
		void OnPointerPressed(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerMoved(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerReleased(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);

	internal:
		HRESULT STDMETHODCALLTYPE Connect(_In_ IDrawingSurfaceRuntimeHostNative* host);
		void STDMETHODCALLTYPE Disconnect();
		HRESULT STDMETHODCALLTYPE PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty);
		HRESULT STDMETHODCALLTYPE GetTexture(_In_ const DrawingSurfaceSizeF* size, _Inout_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Inout_ DrawingSurfaceRectF* textureSubRectangle);
		ID3D11Texture2D* GetTexture();



	private:
		//CubeRenderer^ m_renderer;
		BasicTimer^ m_timer;
		Windows::Foundation::Size m_renderResolution;
		ONScripter* ons;
		D3D11_RenderData *data;
		SDLRenderer^ m_renderer;
		bool isLoaded;
		/*bool m_windowClosed;
		bool m_windowVisible;
		const SDL_WindowData* m_sdlWindowData;
		const SDL_VideoDevice* m_sdlVideoDevice;
		bool m_useRelativeMouseMode;*/

	};

}