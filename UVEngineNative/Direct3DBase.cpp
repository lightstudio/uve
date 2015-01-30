
#include <pch.h>
#include "Direct3DBase.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// 构造函数。
Direct3DBase::Direct3DBase()
{
}

// 初始化运行所需的 Direct3D 资源。
void Direct3DBase::Initialize()
{
	CreateDeviceResources();
}

// 以下是依赖设备的资源。
void Direct3DBase::CreateDeviceResources()
{
	// 此标志为与 API 默认设置具有不同颜色渠道顺序的图面
	// 添加支持。要与 Direct2D 兼容，必须满足此要求。
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// 如果项目处于调试生成过程中，请通过带有此标志的 SDK 层启用调试。
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 此数组定义此应用程序将支持的 DirectX 硬件功能级别组。
	// 请注意，应保留顺序。
	// 请不要忘记在应用程序的说明中声明其所需的
	// 最低功能级别。除非另行说明，否则假定所有应用程序均支持 9.3。
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_9_3
	};

	// 创建 Direct3D 11 API 设备对象和对应的上下文。
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	DX::ThrowIfFailed(
		D3D11CreateDevice(
			nullptr, // 指定 nullptr 以使用默认适配器。
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags, // 设置调试和 Direct2D 兼容性标志。
			featureLevels, // 此应用程序可以支持的功能级别的列表。
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, // 始终将这设置为 D3D11_SDK_VERSION。
			&device, // 返回创建的 Direct3D 设备。
			&m_featureLevel, // 返回所创建设备的功能级别。
			&context // 返回设备的即时上下文。
			)
		);

	// 获取 Direct3D 11.1 API 设备和上下文接口。
	DX::ThrowIfFailed(
		device.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		context.As(&m_d3dContext)
		);
}

// 分配在依赖于窗口大小的所有内存资源。
void Direct3DBase::CreateWindowSizeDependentResources()
{
	// 为呈现目标缓冲区创建描述符。
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
			&m_renderTarget
			)
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
			m_renderTarget.Get(),
			nullptr,
			&m_renderTargetView
			)
		);

	// 创建深度模具视图。
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&depthStencil
			)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilView(
			depthStencil.Get(),
			&depthStencilViewDesc,
			&m_depthStencilView
			)
		);

	// 设置用于确定整个窗口的呈现视区。
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_renderTargetSize.Width,
		m_renderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &viewport);
}

void Direct3DBase::UpdateForRenderResolutionChange(float width, float height)
{
	m_renderTargetSize.Width = width;
	m_renderTargetSize.Height = height;

	ID3D11RenderTargetView* nullViews[] = {nullptr};
	m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	m_renderTarget = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilView = nullptr;
	m_d3dContext->Flush();
	CreateWindowSizeDependentResources();
}

void Direct3DBase::UpdateForWindowSizeChange(float width, float height)
{
	m_windowBounds.Width  = width;
	m_windowBounds.Height = height;
}
#include <pch.h>