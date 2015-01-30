#include <pch.h>
#include <SDL.h>
#include <XDLRenderer.h>
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;
using namespace SDL_Render;


void SDLRenderer::Render() 
{
	//if(!SharedData.operations.empty())
	//{
	//	char method=SharedData.operations.front();
	//	SharedData.operations.pop();
	//	switch (method)
	//	{
	//	case 0x00:

	//	default:
	//		break;
	//	}
	//}
		const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	//m_d3dContext->ClearRenderTargetView(
	//	m_renderTargetView.Get(),
	//	midnightBlue
	//	);
	//m_d3dContext->OMSetRenderTargets(1,SharedData.mainRenderTargetView.GetAddressOf(),nullptr);
	//m_d3dContext->ClearRenderTargetView(SharedData.mainRenderTargetView.Get(),midnightBlue);
	switch(SharedData.MethodsCount)
	{
	case 1:
		{
			D3D11_RenderClear(SharedData.renderer);
			SharedData.MethodsCount=0;
			break;
		}
	case 2:
		{
			D3D11_RenderDrawPoints(SharedData.renderer,(const SDL_FPoint *)SharedData.magics[0],*(int *)SharedData.magics[1]);
			SharedData.MethodsCount=0;
			break;
		}
	case 3:
		{
			D3D11_RenderDrawLines(SharedData.renderer,(const SDL_FPoint *)SharedData.magics[0],*(int *)SharedData.magics[1]);
			SharedData.MethodsCount=0;
			break;
		}
	case 4:
		{
			D3D11_RenderFillRects(SharedData.renderer,(const SDL_FRect *)SharedData.magics[0],*(int *)SharedData.magics[1]);
			SharedData.MethodsCount=0;
			break;
		}
	case 5:
		{
			//m_d3dContext->ClearRenderTargetView(SharedData.mainRenderTargetView.Get(),midnightBlue);
			D3D11_RenderCopy(SharedData.renderer,(SDL_Texture *)SharedData.magics[0],(const SDL_Rect *)SharedData.magics[1],(const SDL_FRect *)SharedData.magics[2]);
			SharedData.MethodsCount=0;
			break;
		}
	case 6:
		{
			D3D11_RenderCopyEx(SharedData.renderer,(SDL_Texture *)SharedData.magics[0],(const SDL_Rect * )SharedData.magics[1],(const SDL_FRect *)SharedData.magics[2],*(const double *)SharedData.magics[3],(const SDL_FPoint *)SharedData.magics[4],*(SDL_RendererFlip *)SharedData.magics[5]);
			SharedData.MethodsCount=0;
			break;
		}
	default:
		{
			break;
		}

	}
 
}
 SDLRenderer::SDLRenderer() :
	m_loadingComplete(false),
	doOnce(true),IsInitialized(false)
 {

 }

void SDLRenderer::CreateWindowSizeDependentResources()
{
	//Direct3DBase::CreateWindowSizeDependentResources();
	//SharedData.m_SyncTexture=m_renderTarget;
		CD3D11_TEXTURE2D_DESC renderTargetDesc(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);
	renderTargetDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;

	// 将 2-D 图面作为呈现目标缓冲区分配。
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
			&renderTargetDesc,
			nullptr,
			&SharedData.m_SyncTexture
			)
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
		SharedData.m_SyncTexture.Get(),
			nullptr,
			&SharedData.mainRenderTargetView
			)
		);

	//// 设置用于确定整个窗口的呈现视区。
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_renderTargetSize.Width,
		m_renderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &viewport);
	if (IsInitialized)
	D3D11_CreateWindowSizeDependentResources(SharedData.renderer);
	else IsInitialized = true;

}
void SDLRenderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();
	m_d3dContext.As(&SharedData.m_context);
	m_d3dDevice.As(&SharedData.m_device);
}
void SDLRenderer::Update(float timeTotal,float timeDelta)
{
	
}
D3DSharedData SDLRenderer::GetSharedData()
{
	return SharedData;
}
