/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "SDL_config.h"

#if SDL_VIDEO_RENDER_D3D11 && !SDL_RENDER_DISABLED

#ifdef __WINRT__
#include <windows.ui.core.h>
#include <windows.foundation.h>
#include <winbase.h>
#endif

extern "C" {
#include "../../core/windows/SDL_windows.h"
//#include "SDL_hints.h"
//#include "SDL_loadso.h"
#include "SDL_system.h"
#include "SDL_syswm.h"
#include "../SDL_sysrender.h"
#include "SDL_log.h"
#include "../../video/SDL_sysvideo.h"
#include "SDL.h"
//#include "stdio.h"
}

#include <fstream>
#include <string>
#include <vector>


#include "../../../include/SDL_render_d3d11_cpp.h"
#include "SharedData.h"
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;
using namespace SDL_Render;
#ifdef __WINRT__
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
#endif

/* Direct3D 11.1 renderer implementation */

static D3DSharedData* LocalRenderer;

extern "C" SDL_RenderDriver D3D11_RenderDriver = {
    D3D11_CreateRenderer,
    {
        "direct3d 11.1",
        (
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC |
            SDL_RENDERER_TARGETTEXTURE
        ),                          // flags.  see SDL_RendererFlags
        2,                          // num_texture_formats
        {                           // texture_formats
            SDL_PIXELFORMAT_RGB888,
            SDL_PIXELFORMAT_ARGB8888
        },
        0,                          // max_texture_width: will be filled in later
        0                           // max_texture_height: will be filled in later
    }
};


static Uint32
DXGIFormatToSDLPixelFormat(DXGI_FORMAT dxgiFormat) {
    switch (dxgiFormat) {
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            return SDL_PIXELFORMAT_ARGB8888;
        case DXGI_FORMAT_B8G8R8X8_UNORM:
            return SDL_PIXELFORMAT_RGB888;
        default:
            return SDL_PIXELFORMAT_UNKNOWN;
    }
}

static DXGI_FORMAT
SDLPixelFormatToDXGIFormat(Uint32 sdlFormat)
{
    switch (sdlFormat) {
        case SDL_PIXELFORMAT_ARGB8888:
            return DXGI_FORMAT_B8G8R8A8_UNORM;
        case SDL_PIXELFORMAT_RGB888:
            return DXGI_FORMAT_B8G8R8X8_UNORM;
        default:
            return DXGI_FORMAT_UNKNOWN;
    }
}







SDL_Renderer *
D3D11_CreateRenderer(SDL_Window * window, Uint32 flags)
{
    SDL_Renderer *renderer;
    D3DSharedData *data;

    renderer = (SDL_Renderer *) SDL_calloc(1, sizeof(*renderer));
    if (!renderer) {
        SDL_OutOfMemory();
        return NULL;
    }
    SDL_zerop(renderer);

	data =(D3DSharedData *) window->driverdata;    // Use the C++ 'new' operator to make sure the struct's members initialize using C++ rules
    if (!data) {
        SDL_OutOfMemory();
        return NULL;
    }

    renderer->WindowEvent = D3D11_WindowEvent;
    renderer->CreateTexture = D3D11_CreateTexture;
    renderer->UpdateTexture = D3D11_UpdateTexture;
    renderer->LockTexture = D3D11_LockTexture;
    renderer->UnlockTexture = D3D11_UnlockTexture;
    renderer->SetRenderTarget = D3D11_SetRenderTarget;
    renderer->UpdateViewport = D3D11_UpdateViewport;
    renderer->RenderClear = D3D11_RenderClear;
    renderer->RenderDrawPoints = D3D11_RenderDrawPoints;
    renderer->RenderDrawLines = D3D11_RenderDrawLines;
    renderer->RenderFillRects = D3D11_RenderFillRects;
    renderer->RenderCopy = D3D11_RenderCopy;
    renderer->RenderCopyEx = D3D11_RenderCopyEx;
    renderer->RenderReadPixels = D3D11_RenderReadPixels;
    renderer->RenderPresent = D3D11_RenderPresent;
    renderer->DestroyTexture = D3D11_DestroyTexture;
    renderer->DestroyRenderer = D3D11_DestroyRenderer;
    renderer->info = D3D11_RenderDriver.info;
    renderer->driverdata = data;

    // HACK: make sure the SDL_Renderer references the SDL_Window data now, in
    // order to give init functions access to the underlying window handle:
    renderer->window = window;

    /* Initialize Direct3D resources */
    if (FAILED(D3D11_CreateDeviceResources(renderer))) {
        D3D11_DestroyRenderer(renderer);
        return NULL;
    }
    if (FAILED(D3D11_CreateWindowSizeDependentResources(renderer))) {
        D3D11_DestroyRenderer(renderer);
        return NULL;
    }
	data->renderer=renderer;
    // TODO, WinRT: fill in renderer->info.texture_formats where appropriate
	LocalRenderer=(D3DSharedData *)renderer->driverdata;
    return renderer;
}

static void
D3D11_DestroyRenderer(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    if (data) {
        delete data;
        data = NULL;
    }
}

static bool
D3D11_ReadFileContents(const wstring & fileName, vector<char> & out)
{
    ifstream in(fileName, ios::in | ios::binary);
    if (!in) {
        return false;
    }

    in.seekg(0, ios::end);
    out.resize((size_t) in.tellg());
    in.seekg(0, ios::beg);
    in.read(&out[0], out.size());
    return in.good();
}

static bool
D3D11_ReadShaderContents(const wstring & shaderName, vector<char> & out)
{
    wstring fileName;


    fileName = SDL_WinRTGetFSPathUNICODE(SDL_WINRT_PATH_INSTALLED_LOCATION);
    fileName += L"\\";

    // WinRT, TODO: test Direct3D 11.1 shader loading on Win32
    fileName += shaderName;
    return D3D11_ReadFileContents(fileName, out);
}

static HRESULT
D3D11_LoadPixelShader(SDL_Renderer * renderer,
                      const wstring & shaderName,
                      ID3D11PixelShader ** shaderOutput)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    HRESULT result = S_OK;
    vector<char> fileData;
    
    if (!D3D11_ReadShaderContents(shaderName, fileData)) {
        SDL_SetError("Unable to open SDL's pixel shader file.");
        return E_FAIL;
    }

    result = data->m_device->CreatePixelShader(
        &fileData[0],
        fileData.size(),
        nullptr,
        shaderOutput
        );
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    return S_OK;
}

static HRESULT
D3D11_CreateBlendMode(SDL_Renderer * renderer,
                      BOOL enableBlending,
                      D3D11_BLEND srcBlend,
                      D3D11_BLEND destBlend,
                      ID3D11BlendState ** blendStateOutput)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    HRESULT result = S_OK;

    D3D11_BLEND_DESC blendDesc;
    memset(&blendDesc, 0, sizeof(blendDesc));
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = enableBlending;
    blendDesc.RenderTarget[0].SrcBlend = srcBlend;
    blendDesc.RenderTarget[0].DestBlend = destBlend;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    result = data->m_device->CreateBlendState(&blendDesc, blendStateOutput);
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    return S_OK;
}
// Create resources that depend on the device.
HRESULT
D3D11_CreateDeviceResources(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Don't forget to declare your application's minimum required feature level in its
    // description.  All applications are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the Direct3D 11 API device object and a corresponding context.
    //ComPtr<ID3D11Device> device;
    //ComPtr<ID3D11DeviceContext> context;
    HRESULT result = S_OK;
    //result = D3D11CreateDevice(
    //    nullptr, // Specify nullptr to use the default adapter.
    //    D3D_DRIVER_TYPE_HARDWARE,
    //    nullptr,
    //    creationFlags, // Set set debug and Direct2D compatibility flags.
    //    featureLevels, // List of feature levels this app can support.
    //    ARRAYSIZE(featureLevels),
    //    D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Windows Store apps.
    //    &device, // Returns the Direct3D device created.
    //    &data->featureLevel, // Returns feature level of device created.
    //    &context // Returns the device immediate context.
    //    );
    //if (FAILED(result)) {
    //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
    //    return result;
    //}

    // Get the Direct3D 11.1 API device and context interfaces.
//    Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice1;


    //
    // Make note of the maximum texture size
    // Max texture sizes are documented on MSDN, at:
    // http://msdn.microsoft.com/en-us/library/windows/apps/ff476876.aspx
    //
    switch (data->m_device->GetFeatureLevel()) {
        case D3D_FEATURE_LEVEL_11_1:
        case D3D_FEATURE_LEVEL_11_0:
            renderer->info.max_texture_width = renderer->info.max_texture_height = 16384;
            break;

        case D3D_FEATURE_LEVEL_10_1:
        case D3D_FEATURE_LEVEL_10_0:
            renderer->info.max_texture_width = renderer->info.max_texture_height = 8192;
            break;

        case D3D_FEATURE_LEVEL_9_3:
            renderer->info.max_texture_width = renderer->info.max_texture_height = 4096;
            break;

        case D3D_FEATURE_LEVEL_9_2:
        case D3D_FEATURE_LEVEL_9_1:
            renderer->info.max_texture_width = renderer->info.max_texture_height = 2048;
            break;
    }

    //
    // Load in SDL's one and only vertex shader:
    //
    vector<char> fileData;
    if (!D3D11_ReadShaderContents(L"SDL_D3D11_VertexShader_Default.cso", fileData)) {
        SDL_SetError("Unable to open SDL's vertex shader file.");
        return E_FAIL;
    }
//	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
//	auto device=data->m_device;
    result = data->m_device->CreateVertexShader(
        &fileData[0],
        fileData.size(),
        nullptr,
		data->vertexShader.GetAddressOf()
        );
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    //
    // Create an input layout for SDL's vertex shader:
    //
    const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
//	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    result = data->m_device->CreateInputLayout(
        vertexDesc,
        ARRAYSIZE(vertexDesc),
        &fileData[0],
        fileData.size(),
        data->inputLayout.GetAddressOf()
        );
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    //
    // Load in SDL's pixel shaders
    //
//	ComPtr<ID3D11PixelShader> texturePixelShader;
	result = D3D11_LoadPixelShader(renderer, L"SDL_D3D11_PixelShader_TextureColored.cso", data->texturePixelShader.GetAddressOf());
    if (FAILED(result)) {
        // D3D11_LoadPixelShader will have aleady set the SDL error
        return result;
    }
//	ComPtr<ID3D11PixelShader> colorPixelShader;
	result = D3D11_LoadPixelShader(renderer, L"SDL_D3D11_PixelShader_FixedColor.cso", data->colorPixelShader.GetAddressOf());
    if (FAILED(result)) {
        // D3D11_LoadPixelShader will have aleady set the SDL error
        return result;
    }

    //
    // Setup space to hold vertex shader constants:
    //
//	ComPtr<ID3D11Buffer> vertexShaderConstants;
    CD3D11_BUFFER_DESC constantBufferDesc(sizeof(SDL_VertexShaderConstants), D3D11_BIND_CONSTANT_BUFFER);
    result = data->m_device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		data->vertexShaderConstants.GetAddressOf()
		);
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    //
    // Make sure that the vertex buffer, if already created, gets freed.
    // It will be recreated later.
    //
    //data->vertexBuffer = nullptr;

    //
    // Create a sampler to use when drawing textures:
    //
    D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	ComPtr<ID3D11SamplerState> mainSampler;
    result = data->m_device->CreateSamplerState(
        &samplerDesc,
		data->mainSampler.GetAddressOf()
        );
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    //
    // Setup the Direct3D rasterizer
    //
    D3D11_RASTERIZER_DESC rasterDesc;
    memset(&rasterDesc, 0, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
//	ComPtr<ID3D11RasterizerState> mainRasterizer;
	result = data->m_device->CreateRasterizerState(&rasterDesc, data->mainRasterizer.GetAddressOf());
	if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    //
    // Create blending states:
    //
//	ComPtr<ID3D11BlendState> blendModeBlend;
    result = D3D11_CreateBlendMode(
        renderer,
        TRUE,
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA,
		data->blendModeBlend.GetAddressOf());
    if (FAILED(result)) {
        // D3D11_CreateBlendMode will set the SDL error, if it fails
        return result;
    }
//	ComPtr<ID3D11BlendState> blendModeAdd;
    result = D3D11_CreateBlendMode(
        renderer,
        TRUE,
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_ONE,
		data->blendModeAdd.GetAddressOf());
    if (FAILED(result)) {
        // D3D11_CreateBlendMode will set the SDL error, if it fails
        return result;
    }
//	ComPtr<ID3D11BlendState> blendModeMod;
    result = D3D11_CreateBlendMode(
        renderer,
        TRUE,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_SRC_COLOR,
		data->blendModeMod.GetAddressOf());
    if (FAILED(result)) {
        // D3D11_CreateBlendMode will set the SDL error, if it fails
        return result;
    }
    //
    // All done!
    //
    return S_OK;
}

#ifdef __WINRT__
// Method to convert a length in device-independent pixels (DIPs) to a length in physical pixels.
static float
D3D11_ConvertDipsToPixels(float dips)
{
    static const float dipsPerInch = 96.0f;
    return floor(dips * DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
}
#endif

// Initialize all resources that change when the window's size changes.
// WinRT, TODO: get D3D11_CreateWindowSizeDependentResources working on Win32
HRESULT
D3D11_CreateWindowSizeDependentResources(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    HRESULT result = S_OK;
    //ABI::Windows::UI::Core::ICoreWindow * coreWindow = D3D11_GetCoreWindowFromSDLRenderer(renderer);

    // Store the window bounds so the next time we get a SizeChanged event we can
    // avoid rebuilding everything if the size is identical.
    //ABI::Windows::Foundation::Rect coreWindowBounds;
    //result = coreWindow->get_Bounds(&coreWindowBounds);
    //if (FAILED(result)) {
    //    WIN_SetErrorFromHRESULT(__FUNCTION__", Get Window Bounds", result);
    //    return result;
    //}
	//	CD3D11_TEXTURE2D_DESC renderTargetDesc(
	//	DXGI_FORMAT_B8G8R8A8_UNORM,
	//	static_cast<UINT>(renderer->window->w),
	//	static_cast<UINT>(renderer->window->h),
	//	1,
	//	1,
	//	D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	//	);
	//renderTargetDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;

	//// 将 2-D 图面作为呈现目标缓冲区分配。
	//	result=data->m_device->CreateTexture2D(
	//		&renderTargetDesc,
	//		nullptr,
	//		&data->m_SyncTexture
	//		);
	//	if(FAILED(result))
	//	{
	//		throw;
	//	}


	//// 创建深度模具视图。
	//CD3D11_TEXTURE2D_DESC depthStencilDesc(
	//	DXGI_FORMAT_D24_UNORM_S8_UINT,
	//	static_cast<UINT>(renderer->window->w),
	//	static_cast<UINT>(renderer->window->h),
	//	1,
	//	1,
	//	D3D11_BIND_DEPTH_STENCIL
	//	);

	//ComPtr<ID3D11Texture2D> depthStencil;

	//	result=data->m_device->CreateTexture2D(
	//		&depthStencilDesc,
	//		nullptr,
	//		&depthStencil
	//		);
	//	if(FAILED(result))
	//	{
	//		throw;
	//	}


	//CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	//	result=data->m_device->CreateDepthStencilView(
	//		depthStencil.Get(),
	//		&depthStencilViewDesc,
	//		&data->m_depthStencilView
	//	);
	//	if(FAILED(result))
	//	{
	//		throw;
	//	}
	// 设置用于确定整个窗口的呈现视区。

    //data->windowSizeInDIPs.x = coreWindowBounds.Width;
    //data->windowSizeInDIPs.y = coreWindowBounds.Height;

    // Calculate the necessary swap chain and render target size in pixels.
    //float windowWidth = D3D11_ConvertDipsToPixels(data->windowSizeInDIPs.x);
    //float windowHeight = D3D11_ConvertDipsToPixels(data->windowSizeInDIPs.y);

    // The width and height of the swap chain must be based on the window's
    // landscape-oriented width and height. If the window is in a portrait
    // orientation, the dimensions must be reversed.
    //data->orientation = DisplayOrientations::Landscape;  //debug purpose 
    //const bool swapDimensions =
    //    data->orientation == DisplayOrientations::Portrait ||
    //    data->orientation == DisplayOrientations::PortraitFlipped;
    //data->renderTargetSize.x = swapDimensions ? windowHeight : windowWidth;
    //data->renderTargetSize.y = swapDimensions ? windowWidth : windowHeight;

//    if(data->swapChain != nullptr)
//    {
//        // If the swap chain already exists, resize it.
//        result = data->swapChain->ResizeBuffers(
//            2, // Double-buffered swap chain.
//            static_cast<UINT>(data->renderTargetSize.x),
//            static_cast<UINT>(data->renderTargetSize.y),
//            DXGI_FORMAT_B8G8R8A8_UNORM,
//            0
//            );
//        if (FAILED(result)) {
//            WIN_SetErrorFromHRESULT(__FUNCTION__, result);
//            return result;
//        }
//    }
//    else
//    {
//        // Otherwise, create a new one using the same adapter as the existing Direct3D device.
//        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
//        swapChainDesc.Width = static_cast<UINT>(data->renderTargetSize.x); // Match the size of the window.
//        swapChainDesc.Height = static_cast<UINT>(data->renderTargetSize.y);
//        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
//        swapChainDesc.Stereo = false;
//        swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
//        swapChainDesc.SampleDesc.Quality = 0;
//        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//        swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
//        swapChainDesc.Scaling = DXGI_SCALING_STRETCH; // On phone, only stretch and aspect-ratio stretch scaling are allowed.
//        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // On phone, no swap effects are supported.
#else
//        swapChainDesc.Scaling = DXGI_SCALING_NONE;
//        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
#endif
//        swapChainDesc.Flags = 0;

        //ComPtr<IDXGIDevice1>  dxgiDevice;
        //result = data->d3dDevice.As(&dxgiDevice);
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        //    return result;
        //}

        //ComPtr<IDXGIAdapter> dxgiAdapter;
        //result = dxgiDevice->GetAdapter(&dxgiAdapter);
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        //    return result;
        //}

        //ComPtr<IDXGIFactory2> dxgiFactory;
        //result = dxgiAdapter->GetParent(
        //    __uuidof(IDXGIFactory2), 
        //    &dxgiFactory
        //    );
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        //    return result;
        //}

        //IUnknown * coreWindowAsIUnknown = nullptr;
        //result = coreWindow->QueryInterface(&coreWindowAsIUnknown);
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__ ", CoreWindow to IUnknown", result);
        //    return result;
        //}
        //result = dxgiFactory->CreateSwapChainForCoreWindow(
        //    data->d3dDevice.Get(),
        //    coreWindowAsIUnknown,
        //    &swapChainDesc,
        //    nullptr, // Allow on all displays.
        //    &data->swapChain
        //    );
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        //    return result;
        //}
            
        // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
        // ensures that the application will only render after each VSync, minimizing power consumption.
        //result = dxgiDevice->SetMaximumFrameLatency(1);
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        //    return result;
        //}
//    }
    
    // Set the proper orientation for the swap chain, and generate the
    // 3D matrix transformation for rendering to the rotated swap chain.
    DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
    //switch (data->orientation)
    //{
    //    case DisplayOrientations::Landscape:
    //        rotation = DXGI_MODE_ROTATION_IDENTITY;
    //        break;

    //    case DisplayOrientations::Portrait:
    //        rotation = DXGI_MODE_ROTATION_ROTATE270;
    //        break;

    //    case DisplayOrientations::LandscapeFlipped:
    //        rotation = DXGI_MODE_ROTATION_ROTATE180;
    //        break;

    //    case DisplayOrientations::PortraitFlipped:
    //        rotation = DXGI_MODE_ROTATION_ROTATE90;
    //        break;

    //    default:
    //        throw ref new Platform::FailureException();
    //}
	rotation=DXGI_MODE_ROTATION_IDENTITY;
	data->orientation=DisplayOrientations::Landscape;

#if WINAPI_FAMILY != WINAPI_FAMILY_PHONE_APP
    // TODO, WinRT: Windows Phone does not have the IDXGISwapChain1::SetRotation method.  Check if an alternative is available, or needed.
    result = data->swapChain->SetRotation(rotation);
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }
#endif

    // Create a render target view of the swap chain back buffer.
 //   ComPtr<ID3D11Texture2D> backBuffer;
 //   //result = data->swapChain->GetBuffer(
 //   //    0,
 //   //    __uuidof(ID3D11Texture2D),
 //   //    &backBuffer
 //   //    );
 //   //if (FAILED(result)) {
 //   //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
 //   //    return result;
 //   //}
	//backBuffer=data->m_backbuffer;
  //  result = data->m_device->CreateRenderTargetView(
		//data->m_SyncTexture.Get(),
  //      nullptr,
		//data->mainRenderTargetView.GetAddressOf()
  //      );
  //  if (FAILED(result)) {
  //      WIN_SetErrorFromHRESULT(__FUNCTION__, result);
  //      return result;
  //  }

    if (D3D11_UpdateViewport(renderer) != 0) {
        // D3D11_UpdateViewport will set the SDL error if it fails.
        return E_FAIL;
    }

    return S_OK;
}

// This method is called when the window's size changes.
HRESULT
D3D11_UpdateForWindowSizeChange(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    HRESULT result = S_OK;
    //ABI::Windows::UI::Core::ICoreWindow * coreWindow = D3D11_GetCoreWindowFromSDLRenderer(renderer);
    //ABI::Windows::Foundation::Rect coreWindowBounds;

    //result = coreWindow->get_Bounds(&coreWindowBounds);
    //if (FAILED(result)) {
    //    WIN_SetErrorFromHRESULT(__FUNCTION__ ", Get Window Bounds", result);
    //    return result;
    //}

    //if (coreWindowBounds.Width  != data->windowSizeInDIPs.x ||
    //    coreWindowBounds.Height != data->windowSizeInDIPs.y ||
    //    data->orientation != DisplayProperties::CurrentOrientation)
    //{
		//data->operations.push_back(0x00);

        ID3D11RenderTargetView* nullViews[] = {nullptr};
		//int size=ARRAYSIZE(nullViews);
		//void **params;
		//params[0]=&size;
		//params[1]=&nullViews;
		//params[2]=nullptr;
		//data->params.push_back(params);
		//data->paramCount.push_back(3);
		//data->callerID.push_back(0x00);
        //data->m_context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
        ////data->mainRenderTargetView = nullptr;
        //data->m_context->Flush();
        //result = D3D11_CreateWindowSizeDependentResources(renderer);
        //if (FAILED(result)) {
        //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        //    return result;
        //}
	//while(data->callerID->top()==0x00);

    //}

    return S_OK;
}

HRESULT
D3D11_HandleDeviceLost(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
    HRESULT result = S_OK;

    // Reset these member variables to ensure that D3D11_UpdateForWindowSizeChange recreates all resources.
    //data->windowSizeInDIPs.x = 0;
    //data->windowSizeInDIPs.y = 0;
    //data->swapChain = nullptr;

    result = D3D11_CreateDeviceResources(renderer);
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return result;
    }

    //result = D3D11_UpdateForWindowSizeChange(renderer);
    //if (FAILED(result)) {
    //    WIN_SetErrorFromHRESULT(__FUNCTION__, result);
    //    return result;
    //}

    return S_OK;
}

static void
D3D11_WindowEvent(SDL_Renderer * renderer, const SDL_WindowEvent *event)
{
    //D3D11_RenderData *data = (D3D11_RenderData *) renderer->driverdata;

    //if (event->event == SDL_WINDOWEVENT_RESIZED) {
    //    D3D11_UpdateForWindowSizeChange(renderer);
    //}
}

static int
D3D11_CreateTexture(SDL_Renderer * renderer, SDL_Texture * texture)
{
	D3DSharedData *rendererData = (D3DSharedData *) renderer->window->driverdata;
    D3D11_TextureData *textureData;
    HRESULT result;
    DXGI_FORMAT textureFormat = SDLPixelFormatToDXGIFormat(texture->format);
    if (textureFormat == SDL_PIXELFORMAT_UNKNOWN) {
        SDL_SetError("%s, An unsupported SDL pixel format (0x%x) was specified",
            __FUNCTION__, texture->format);
        return -1;
    }

    textureData = new D3D11_TextureData;
    if (!textureData) {
        SDL_OutOfMemory();
        return -1;
    }
    textureData->pixelFormat = SDL_AllocFormat(texture->format);
    textureData->lockedTexturePosition = XMINT2(0, 0);

    texture->driverdata = textureData;

    D3D11_TEXTURE2D_DESC textureDesc = {0};
    textureDesc.Width = texture->w;
    textureDesc.Height = texture->h;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = textureFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.MiscFlags = 0;

    if (texture->access == SDL_TEXTUREACCESS_STREAMING) {
        textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    } else {
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.CPUAccessFlags = 0;
    }

    if (texture->access == SDL_TEXTUREACCESS_TARGET) {
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    } else {
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    }

#if 0
    // Fill the texture with a non-black color, for debugging purposes:
    const int numPixels = textureDesc.Width * textureDesc.Height;
    const int pixelSizeInBytes = textureData->pixelFormat->BytesPerPixel;
    std::vector<uint8> initialTexturePixels(numPixels * pixelSizeInBytes, 0x00);
    for (int i = 0; i < (numPixels * pixelSizeInBytes); i += pixelSizeInBytes) {
        initialTexturePixels[i+0] = 0xff;
        initialTexturePixels[i+1] = 0xff;
        initialTexturePixels[i+2] = 0x00;
        initialTexturePixels[i+3] = 0xff;
    }
    D3D11_SUBRESOURCE_DATA initialTextureData = {0};
    initialTextureData.pSysMem = (void *)&(initialTexturePixels[0]);
    initialTextureData.SysMemPitch = textureDesc.Width * pixelSizeInBytes;
    initialTextureData.SysMemSlicePitch = numPixels * pixelSizeInBytes;
#endif
    result = rendererData->m_device->CreateTexture2D(
        &textureDesc,
        NULL,   // &initialTextureData,
        &textureData->mainTexture
        );
		//rendererData->m_SyncTexture=textureData->mainTexture ;
    if (FAILED(result)) {
        D3D11_DestroyTexture(renderer, texture);
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return -1;
    }

    if (texture->access & SDL_TEXTUREACCESS_TARGET) {
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        result = rendererData->m_device->CreateRenderTargetView(
            textureData->mainTexture.Get(),
            &renderTargetViewDesc,
            &textureData->mainTextureRenderTargetView);
        if (FAILED(result)) {
            D3D11_DestroyTexture(renderer, texture);
            WIN_SetErrorFromHRESULT(__FUNCTION__, result);
            return -1;
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
    resourceViewDesc.Format = textureDesc.Format;
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDesc.Texture2D.MostDetailedMip = 0;
    resourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
    result = rendererData->m_device->CreateShaderResourceView(
        textureData->mainTexture.Get(),
        &resourceViewDesc,
		textureData->mainTextureResourceView.GetAddressOf()
        );
    if (FAILED(result)) {
        D3D11_DestroyTexture(renderer, texture);
        WIN_SetErrorFromHRESULT(__FUNCTION__, result);
        return -1;
    }

    return 0;
}

static void
D3D11_DestroyTexture(SDL_Renderer * renderer,
                     SDL_Texture * texture)
{
    D3D11_TextureData *textureData = (D3D11_TextureData *) texture->driverdata;

    if (textureData) {
        if (textureData->pixelFormat) {
            SDL_FreeFormat(textureData->pixelFormat);
            textureData->pixelFormat = NULL;
        }

        delete textureData;
        texture->driverdata = NULL;
    }
}

static int
D3D11_UpdateTexture(SDL_Renderer * renderer, SDL_Texture * texture,
                    const SDL_Rect * rect, const void * srcPixels,
                    int srcPitch)
{
    // Lock the texture, retrieving a buffer to write pixel data to:
    void * destPixels = NULL;
    int destPitch = 0;
    if (D3D11_LockTexture(renderer, texture, rect, &destPixels, &destPitch) != 0) {
        // An error is already set.  Attach some info to it, then return to
        // the caller.
        std::string errorMessage = string(__FUNCTION__ ", Lock Texture Failed: ") + SDL_GetError();
        SDL_SetError(errorMessage.c_str());
        return -1;
    }

    // Copy pixel data to the locked texture's memory:
    for (int y = 0; y < rect->h; ++y) {
        memcpy(
            ((Uint8 *)destPixels) + (destPitch * y),
            ((Uint8 *)srcPixels) + (srcPitch * y),
            srcPitch
            );
    }

    // Commit the texture's memory back to Direct3D:
    D3D11_UnlockTexture(renderer, texture);

    // Return to the caller:
    return 0;
}

static int
D3D11_LockTexture(SDL_Renderer * renderer, SDL_Texture * texture,
                  const SDL_Rect * rect, void **pixels, int *pitch)
{
	D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
    D3D11_TextureData *textureData = (D3D11_TextureData *) texture->driverdata;
    HRESULT result = S_OK;
	//const char* temp=rendererData->name;
    if (textureData->stagingTexture) {
        SDL_SetError("texture is already locked");
        return -1;
    }
    // Create a 'staging' texture, which will be used to write to a portion
    // of the main texture.  This is necessary, as Direct3D 11.1 does not
    // have the ability to write a CPU-bound pixel buffer to a rectangular
    // subrect of a texture.  Direct3D 11.1 can, however, write a pixel
    // buffer to an entire texture, hence the use of a staging texture.
	D3D11_TEXTURE2D_DESC stagingTextureDesc;
    textureData->mainTexture->GetDesc(&stagingTextureDesc);
    stagingTextureDesc.Width = rect->w;
    stagingTextureDesc.Height = rect->h;
    stagingTextureDesc.BindFlags = 0;
    stagingTextureDesc.MiscFlags = 0;
    stagingTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    stagingTextureDesc.Usage = D3D11_USAGE_STAGING;
	if(rendererData->m_device==nullptr)
	{
		return SDL_SetError("Device NULL");
	}
	result = rendererData->m_device->CreateTexture2D(
        &stagingTextureDesc,
		NULL,
		textureData->stagingTexture.GetAddressOf());
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__ ", Create Staging Texture", result);
        return -1;
    }

    // Get a write-only pointer to data in the staging texture:
    D3D11_MAPPED_SUBRESOURCE textureMemory = {0};
    result = rendererData->m_context->Map(
        textureData->stagingTexture.Get(),
        D3D11CalcSubresource(0, 0, 0),
        D3D11_MAP_WRITE,
        0,
        &textureMemory
        );
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__ ", Map Staging Texture", result);
        textureData->stagingTexture = nullptr;
        return -1;
    }

    // Make note of where the staging texture will be written to (on a
    // call to SDL_UnlockTexture):
    textureData->lockedTexturePosition = XMINT2(rect->x, rect->y);

    // Make sure the caller has information on the texture's pixel buffer,
    // then return:
    *pixels = textureMemory.pData;
    *pitch = textureMemory.RowPitch;
    return 0;
}

static void
D3D11_UnlockTexture(SDL_Renderer * renderer, SDL_Texture * texture)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
    D3D11_TextureData *textureData = (D3D11_TextureData *) texture->driverdata;

    // Commit the pixel buffer's changes back to the staging texture:
    rendererData->m_context->Unmap(
        textureData->stagingTexture.Get(),
        0);

    // Copy the staging texture's contents back to the main texture:
    rendererData->m_context->CopySubresourceRegion(
        textureData->mainTexture.Get(),
        D3D11CalcSubresource(0, 0, 0),
        textureData->lockedTexturePosition.x,
        textureData->lockedTexturePosition.y,
        0,
        textureData->stagingTexture.Get(),
        D3D11CalcSubresource(0, 0, 0),
        NULL);

    // Clean up and return:
    textureData->stagingTexture = nullptr;
    textureData->lockedTexturePosition = XMINT2(0, 0);
}

static int
D3D11_SetRenderTarget(SDL_Renderer * renderer, SDL_Texture * texture)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;

    if (texture == NULL) {
        //rendererData->currentOffscreenRenderTargetView = nullptr;
        return 0;
    }

    D3D11_TextureData *textureData = (D3D11_TextureData *) texture->driverdata;

    if (!textureData->mainTextureRenderTargetView) {
        std::string errorMessage = string(__FUNCTION__) + ": specified texture is not a render target";
        SDL_SetError(errorMessage.c_str());
        return -1;
    }

	rendererData->mainRenderTargetView = textureData->mainTextureRenderTargetView;

    return 0;
}

static int
D3D11_UpdateViewport(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;

    if (renderer->viewport.w == 0 || renderer->viewport.h == 0) {
        // If the viewport is empty, assume that it is because
        // SDL_CreateRenderer is calling it, and will call it again later
        // with a non-empty viewport.
        return 0;
    }

    switch (data->orientation)
    {
        case DisplayOrientations::Landscape:
            data->vertexShaderConstantsData.projection = XMFLOAT4X4( // 0-degree Z-rotation
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
                );
            break;

        case DisplayOrientations::Portrait:
            data->vertexShaderConstantsData.projection = XMFLOAT4X4( // 90-degree Z-rotation
                0.0f, 1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
                );
            break;

        case DisplayOrientations::LandscapeFlipped:
            data->vertexShaderConstantsData.projection = XMFLOAT4X4( // 180-degree Z-rotation
                -1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, -1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
                );
            break;

        case DisplayOrientations::PortraitFlipped:
            data->vertexShaderConstantsData.projection = XMFLOAT4X4( // 270-degree Z-rotation
                0.0f, -1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
                );
            break;

        default:
            SDL_SetError("An unknown DisplayOrientation is being used");
            return -1;
    }

    //
    // Update the view matrix
    //
    float viewportWidth = (float) renderer->viewport.w;
    float viewportHeight = (float) renderer->viewport.h;
    XMStoreFloat4x4(&data->vertexShaderConstantsData.view,
        XMMatrixMultiply(
            XMMatrixScaling(2.0f / viewportWidth, 2.0f / viewportHeight, 1.0f),
            XMMatrixMultiply(
                XMMatrixTranslation(-1, -1, 0),
                XMMatrixRotationX(XM_PI)
                )));
#if 0
    data->vertexShaderConstantsData.view = XMMatrixIdentity();
#endif

    //
    // Reset the model matrix
    //
    XMStoreFloat4x4(&data->vertexShaderConstantsData.model, XMMatrixIdentity());

    //
    // Update the Direct3D viewport, which seems to be aligned to the
    // swap buffer's coordinate space, which is always in landscape:
    //
    SDL_FRect orientationAlignedViewport;
    const bool swapDimensions =
        data->orientation == DisplayOrientations::Portrait ||
        data->orientation == DisplayOrientations::PortraitFlipped;
    if (swapDimensions) {
        orientationAlignedViewport.x = (float) renderer->viewport.y;
        orientationAlignedViewport.y = (float) renderer->viewport.x;
        orientationAlignedViewport.w = (float) renderer->viewport.h;
        orientationAlignedViewport.h = (float) renderer->viewport.w;
    } else {
        orientationAlignedViewport.x = (float) renderer->viewport.x;
        orientationAlignedViewport.y = (float) renderer->viewport.y;
        orientationAlignedViewport.w = (float) renderer->viewport.w;
        orientationAlignedViewport.h = (float) renderer->viewport.h;
    }
    // WinRT, TODO: get custom viewports working with non-Landscape modes (Portrait, PortraitFlipped, and LandscapeFlipped)

    D3D11_VIEWPORT viewport;
    memset(&viewport, 0, sizeof(viewport));
    viewport.TopLeftX = orientationAlignedViewport.x;
    viewport.TopLeftY = orientationAlignedViewport.y;
    viewport.Width = orientationAlignedViewport.w;
    viewport.Height = orientationAlignedViewport.h;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    data->m_context->RSSetViewports(1, &viewport);

#if 0
    SDL_Log("%s, oav={%.0f,%.0f,%.0f,%.0f}, rend={%.0f,%.0f}\n",
        __FUNCTION__,
        orientationAlignedViewport.x,
        orientationAlignedViewport.y,
        orientationAlignedViewport.w,
        orientationAlignedViewport.h,
        data->renderTargetSize.x,
        data->renderTargetSize.y);
#endif

    return 0;
}

static ComPtr<ID3D11RenderTargetView> &
D3D11_GetCurrentRenderTargetView(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
	return data->mainRenderTargetView;
}

static int
D3D11_RenderClear(SDL_Renderer * renderer)
{
	D3DSharedData *data = (D3DSharedData *) renderer->driverdata;
	if(data->MethodsCount==1)
	{

		//const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
		//data->m_context->ClearRenderTargetView(
		//	data->mainRenderTargetView.Get(),
		//	midnightBlue
		//	);
		return 0;
	}
	else 
	{
		data->renderer=renderer;
		data->MethodsCount=1;
		//while(data->MethodsCount==1);
		SDL_Delay(30);
		//		for(int i=0;i<10000;i++)
		//{
		//	Platform::String^ s("wow");
		//	Platform::String^ s1("aa");
		//	Platform::String^ end=s+s1;
		//}
		return 0;
	}
}

 int
D3D11_UpdateVertexBuffer(SDL_Renderer *renderer,
                         const void * vertexData, unsigned int dataSizeInBytes)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;

		HRESULT result = S_OK;
		D3D11_BUFFER_DESC vertexBufferDesc;

		if (rendererData->vertexBuffer) {
			rendererData->vertexBuffer->GetDesc(&vertexBufferDesc);
		} else {
			memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
		}

		if (vertexBufferDesc.ByteWidth >= dataSizeInBytes) {
			rendererData->m_context->UpdateSubresource(rendererData->vertexBuffer.Get(), 0, NULL, vertexData, dataSizeInBytes, 0);
		} else {
			vertexBufferDesc.ByteWidth = dataSizeInBytes;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
			vertexBufferData.pSysMem = vertexData;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;

			result = rendererData->m_device->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&rendererData->vertexBuffer
				);
			if (FAILED(result)) {
				WIN_SetErrorFromHRESULT(__FUNCTION__, result);
				return -1;
			}
		}

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		rendererData->m_context->IASetVertexBuffers(
			0,
			1,
			rendererData->vertexBuffer.GetAddressOf(),
			&stride,
			&offset
			);

		return 0;

}

void D3D11_RenderStartDrawOp(SDL_Renderer * renderer)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;

    rendererData->m_context->OMSetRenderTargets(
        1,
		rendererData->mainRenderTargetView.GetAddressOf(),
        nullptr
        );
		//const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
		//auto ptr=D3D11_GetCurrentRenderTargetView(renderer);
		//rendererData->m_context->ClearRenderTargetView(ptr.Get(),midnightBlue);

	//int par1=1;
	//rendererData->operations.push_back(0x00);
	//void ** params;
	//params[0]=&par1;
	//params[1]=D3D11_GetCurrentRenderTargetView(renderer).GetAddressOf();
	//params[2]=nullptr;
	//rendererData->paramCount.push_back(3);
	//rendererData->callerID.push_back(0x01);
	//while(rendererData->callerID.top()==0x01);


}

static void
D3D11_RenderSetBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
    switch (blendMode) {
        case SDL_BLENDMODE_BLEND:
            rendererData->m_context->OMSetBlendState(rendererData->blendModeBlend.Get(), 0, 0xFFFFFFFF);
            break;
        case SDL_BLENDMODE_ADD:
            rendererData->m_context->OMSetBlendState(rendererData->blendModeAdd.Get(), 0, 0xFFFFFFFF);
            break;
        case SDL_BLENDMODE_MOD:
            rendererData->m_context->OMSetBlendState(rendererData->blendModeMod.Get(), 0, 0xFFFFFFFF);
            break;
        case SDL_BLENDMODE_NONE:
            rendererData->m_context->OMSetBlendState(NULL, 0, 0xFFFFFFFF);
            break;
    }
}

 void
D3D11_SetPixelShader(SDL_Renderer * renderer,
                     ID3D11PixelShader * shader,
                     ID3D11ShaderResourceView * shaderResource,
                     ID3D11SamplerState * sampler)
{

    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;

		rendererData->m_context->PSSetShader(shader, nullptr, 0);

		rendererData->m_context->PSSetShaderResources(0, 1, &shaderResource);
		rendererData->m_context->PSSetSamplers(0, 1, &sampler);


}

 void
D3D11_RenderFinishDrawOp(SDL_Renderer * renderer,
                         D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,
                         UINT vertexCount)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
		rendererData->m_context->UpdateSubresource(
			rendererData->vertexShaderConstants.Get(),
			0,
			NULL,
			&rendererData->vertexShaderConstantsData,
			0,
			0
			);

		rendererData->m_context->IASetPrimitiveTopology(primitiveTopology);
		rendererData->m_context->IASetInputLayout(rendererData->inputLayout.Get());
		rendererData->m_context->VSSetShader(rendererData->vertexShader.Get(), nullptr, 0);
		rendererData->m_context->VSSetConstantBuffers(0, 1, rendererData->vertexShaderConstants.GetAddressOf());
		rendererData->m_context->RSSetState(rendererData->mainRasterizer.Get());
		rendererData->m_context->Draw(vertexCount, 0);
	
}

static int
D3D11_RenderDrawPoints(SDL_Renderer * renderer,
                       const SDL_FPoint * points, int count)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
	if(rendererData->MethodsCount==2)
	{
		float r, g, b, a;

		r = (float)(renderer->r / 255.0f);
		g = (float)(renderer->g / 255.0f);
		b = (float)(renderer->b / 255.0f);
		a = (float)(renderer->a / 255.0f);

		vector<VertexPositionColor> vertices;
		vertices.reserve(count);
		for (int i = 0; i < count; ++i) {
			VertexPositionColor v = {XMFLOAT3(points[i].x, points[i].y, 0.0f),  XMFLOAT2(0.0f, 0.0f), XMFLOAT4(r, g, b, a)};
			vertices.push_back(v);
		}

		D3D11_RenderStartDrawOp(renderer);
		D3D11_RenderSetBlendMode(renderer, renderer->blendMode);
		if (D3D11_UpdateVertexBuffer(renderer, &vertices[0], vertices.size() * sizeof(VertexPositionColor)) != 0) {
			return -1;
		}

		D3D11_SetPixelShader(
			renderer,
			rendererData->colorPixelShader.Get(),
			nullptr,
			nullptr);

		D3D11_RenderFinishDrawOp(renderer, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, vertices.size());

		return 0;
	}
	else 
	{
		rendererData->renderer=renderer;
		rendererData->constmagics[0]=points;
		rendererData->magics[0]=&count;
		rendererData->magicCount=2;
		rendererData->MethodsCount=2;
		//while(rendererData->MethodsCount==2);
		SDL_Delay(30);
		//		for(int i=0;i<10000;i++)
		//{
		//	Platform::String^ s("wow");
		//	Platform::String^ s1("aa");
		//	Platform::String^ end=s+s1;
		//}
		return 0;
	}
}

static int
D3D11_RenderDrawLines(SDL_Renderer * renderer,
                      const SDL_FPoint * points, int count)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
	if(rendererData->MethodsCount==3)
	{
		float r, g, b, a;

		r = (float)(renderer->r / 255.0f);
		g = (float)(renderer->g / 255.0f);
		b = (float)(renderer->b / 255.0f);
		a = (float)(renderer->a / 255.0f);

		vector<VertexPositionColor> vertices;
		vertices.reserve(count);
		for (int i = 0; i < count; ++i) {
			VertexPositionColor v = {XMFLOAT3(points[i].x, points[i].y, 0.0f),  XMFLOAT2(0.0f, 0.0f), XMFLOAT4(r, g, b, a)};
			vertices.push_back(v);
		}

		D3D11_RenderStartDrawOp(renderer);
		D3D11_RenderSetBlendMode(renderer, renderer->blendMode);
		if (D3D11_UpdateVertexBuffer(renderer, &vertices[0], vertices.size() * sizeof(VertexPositionColor)) != 0) {
			return -1;
		}

		D3D11_SetPixelShader(
			renderer,
			rendererData->colorPixelShader.Get(),
			nullptr,
			nullptr);

		D3D11_RenderFinishDrawOp(renderer, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, vertices.size());

		return 0;
	}
	else 
	{
		rendererData->renderer=renderer;
		rendererData->magicCount=2;
		rendererData->constmagics[0]=points;
		rendererData->magics[0]=&count;
		rendererData->MethodsCount=3;
		//while(rendererData->MethodsCount==3);
		SDL_Delay(30);
		//		for(int i=0;i<10000;i++)
		//{
		//	Platform::String^ s("wow");
		//	Platform::String^ s1("aa");
		//	Platform::String^ end=s+s1;
		//}
		return 0;

	}
}

static int
D3D11_RenderFillRects(SDL_Renderer * renderer,
                      const SDL_FRect * rects, int count)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
	if(rendererData->MethodsCount==4)
	{
		float r, g, b, a;

		r = (float)(renderer->r / 255.0f);
		g = (float)(renderer->g / 255.0f);
		b = (float)(renderer->b / 255.0f);
		a = (float)(renderer->a / 255.0f);

	#if 0
		// Set up a test pattern:
		SDL_FRect _rects[] = {
			{-1.1f, 1.1f, 1.1f, -1.1f},
			{-1.0f, 1.0f, 1.0f, -1.0f},     // red
			{0.0f, 1.0f, 1.0f, -1.0f},      // green
			{-1.0f, 0.0f, 1.0f, -1.0f},     // blue
			{0.0f, 0.0f, 1.0f, -1.0f}       // white
		};
		count = sizeof(_rects) / sizeof(SDL_FRect);
	#define rects _rects
	#endif

		for (int i = 0; i < count; ++i) {
			D3D11_RenderStartDrawOp(renderer);
			D3D11_RenderSetBlendMode(renderer, renderer->blendMode);

	#if 0
			// Set colors for the test pattern:
			a = 1.0f;
			switch (i) {
				case 0: r = 1.0f; g = 1.0f; b = 0.0f; break;
				case 1: r = 1.0f; g = 0.0f; b = 0.0f; break;
				case 2: r = 0.0f; g = 1.0f; b = 0.0f; break;
				case 3: r = 0.0f; g = 0.0f; b = 1.0f; break;
				case 4: r = 1.0f; g = 1.0f; b = 1.0f; break;
			}
	#endif

			VertexPositionColor vertices[] = {
				{XMFLOAT3(rects[i].x, rects[i].y, 0.0f),                           XMFLOAT2(0.0f, 0.0f), XMFLOAT4(r, g, b, a)},
				{XMFLOAT3(rects[i].x, rects[i].y + rects[i].h, 0.0f),              XMFLOAT2(0.0f, 0.0f), XMFLOAT4(r, g, b, a)},
				{XMFLOAT3(rects[i].x + rects[i].w, rects[i].y, 0.0f),              XMFLOAT2(0.0f, 0.0f), XMFLOAT4(r, g, b, a)},
				{XMFLOAT3(rects[i].x + rects[i].w, rects[i].y + rects[i].h, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT4(r, g, b, a)},
			};
			if (D3D11_UpdateVertexBuffer(renderer, vertices, sizeof(vertices)) != 0) {
				return -1;
			}

			D3D11_SetPixelShader(
				renderer,
				rendererData->colorPixelShader.Get(),
				nullptr,
				nullptr);

			D3D11_RenderFinishDrawOp(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sizeof(vertices) / sizeof(VertexPositionColor));
		}

		return 0;
	}
	else 
	{
		rendererData->renderer=renderer;
		rendererData->magics[0]=const_cast<SDL_FRect *>(rects);
		rendererData->magics[1]=&count;
		rendererData->MethodsCount=4;
		SDL_Delay(30);
		//for(int i=0;i<10000;i++)
		//{
		//	Platform::String^ s("wow");
		//	Platform::String^ s1("aa");
		//	Platform::String^ end=s+s1;
		//}
		return 0;
	}
}

static int
D3D11_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture,
                 const SDL_Rect * srcrect, const SDL_FRect * dstrect)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
    D3D11_TextureData *textureData = (D3D11_TextureData *) texture->driverdata;
		const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	if(rendererData->MethodsCount==5)
	{
		D3D11_RenderStartDrawOp(renderer);
		D3D11_RenderSetBlendMode(renderer, texture->blendMode);
		//rendererData->m_context->ClearRenderTargetView(rendererData->mainRenderTargetView.Get(),midnightBlue);
		float minu = (float) srcrect->x / texture->w;
		float maxu = (float) (srcrect->x + srcrect->w) / texture->w;
		float minv = (float) srcrect->y / texture->h;
		float maxv = (float) (srcrect->y + srcrect->h) / texture->h;

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;
		if (texture->modMode & SDL_TEXTUREMODULATE_COLOR) {
			r = (float)(texture->r / 255.0f);
			g = (float)(texture->g / 255.0f);
			b = (float)(texture->b / 255.0f);
		}
		if (texture->modMode & SDL_TEXTUREMODULATE_ALPHA) {
			a = (float)(texture->a / 255.0f);
		}

		VertexPositionColor vertices[] = {
			{XMFLOAT3(dstrect->x, dstrect->y, 0.0f),                           XMFLOAT2(minu, minv), XMFLOAT4(r, g, b, a)},
			{XMFLOAT3(dstrect->x, dstrect->y + dstrect->h, 0.0f),              XMFLOAT2(minu, maxv), XMFLOAT4(r, g, b, a)},
			{XMFLOAT3(dstrect->x + dstrect->w, dstrect->y, 0.0f),              XMFLOAT2(maxu, minv), XMFLOAT4(r, g, b, a)},
			{XMFLOAT3(dstrect->x + dstrect->w, dstrect->y + dstrect->h, 0.0f), XMFLOAT2(maxu, maxv), XMFLOAT4(r, g, b, a)},
		};
		if (D3D11_UpdateVertexBuffer(renderer, vertices, sizeof(vertices)) != 0) {
			return -1;
		}

		D3D11_SetPixelShader(
			renderer,
			rendererData->texturePixelShader.Get(),
			textureData->mainTextureResourceView.Get(),
			rendererData->mainSampler.Get());

		D3D11_RenderFinishDrawOp(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sizeof(vertices) / sizeof(VertexPositionColor));
		//D3D11_RenderFinishDrawOp(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4);//debug purpose

		return 0;
	}
	else 
	{
		rendererData->renderer=renderer;
		rendererData->magics[0]=texture;
		rendererData->magics[1]=const_cast<SDL_Rect *> (srcrect);
		rendererData->magics[2]=const_cast<SDL_FRect *> (dstrect);
		rendererData->MethodsCount=5;
		SDL_Delay(30);
		//for(int i=0;i<10000;i++)
		//{
		//	Platform::String^ s("wow");
		//	Platform::String^ s1("aa");
		//	Platform::String^ end=s+s1;
		//}
		return 0;
	}
}

static int
D3D11_RenderCopyEx(SDL_Renderer * renderer, SDL_Texture * texture,
                   const SDL_Rect * srcrect, const SDL_FRect * dstrect,
                   const double angle, const SDL_FPoint * center, const SDL_RendererFlip flip)
{
    D3DSharedData *rendererData = (D3DSharedData *) renderer->driverdata;
    D3D11_TextureData *textureData = (D3D11_TextureData *) texture->driverdata;
	if(rendererData->MethodsCount==6)
	{
		D3D11_RenderStartDrawOp(renderer);
		D3D11_RenderSetBlendMode(renderer, texture->blendMode);

		float minu = (float) srcrect->x / texture->w;
		float maxu = (float) (srcrect->x + srcrect->w) / texture->w;
		float minv = (float) srcrect->y / texture->h;
		float maxv = (float) (srcrect->y + srcrect->h) / texture->h;

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;
		if (texture->modMode & SDL_TEXTUREMODULATE_COLOR) {
			r = (float)(texture->r / 255.0f);
			g = (float)(texture->g / 255.0f);
			b = (float)(texture->b / 255.0f);
		}
		if (texture->modMode & SDL_TEXTUREMODULATE_ALPHA) {
			a = (float)(texture->a / 255.0f);
		}

		if (flip & SDL_FLIP_HORIZONTAL) {
			float tmp = maxu;
			maxu = minu;
			minu = tmp;
		}
		if (flip & SDL_FLIP_VERTICAL) {
			float tmp = maxv;
			maxv = minv;
			minv = tmp;
		}

		XMFLOAT4X4 oldModelMatrix = rendererData->vertexShaderConstantsData.model;
		XMStoreFloat4x4(
			&rendererData->vertexShaderConstantsData.model,
			XMMatrixMultiply(
				XMMatrixRotationZ((float)(XM_PI * (float) angle / 180.0f)),
				XMMatrixTranslation(dstrect->x + center->x, dstrect->y + center->y, 0)
				));

		const float minx = -center->x;
		const float maxx = dstrect->w - center->x;
		const float miny = -center->y;
		const float maxy = dstrect->h - center->y;

		VertexPositionColor vertices[] = {
			{XMFLOAT3(minx, miny, 0.0f), XMFLOAT2(minu, minv), XMFLOAT4(r, g, b, a)},
			{XMFLOAT3(minx, maxy, 0.0f), XMFLOAT2(minu, maxv), XMFLOAT4(r, g, b, a)},
			{XMFLOAT3(maxx, miny, 0.0f), XMFLOAT2(maxu, minv), XMFLOAT4(r, g, b, a)},
			{XMFLOAT3(maxx, maxy, 0.0f), XMFLOAT2(maxu, maxv), XMFLOAT4(r, g, b, a)},
		};
		if (D3D11_UpdateVertexBuffer(renderer, vertices, sizeof(vertices)) != 0) {
			return -1;
		}

		D3D11_SetPixelShader(
			renderer,
			rendererData->texturePixelShader.Get(),
			textureData->mainTextureResourceView.Get(),
			rendererData->mainSampler.Get());

		D3D11_RenderFinishDrawOp(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sizeof(vertices) / sizeof(VertexPositionColor));

		rendererData->vertexShaderConstantsData.model = oldModelMatrix;

		return 0;
	}
	else 
	{
		rendererData->renderer=renderer;
		rendererData->magics[0]=texture;
		rendererData->magics[1]=const_cast<SDL_Rect *> (srcrect);
		rendererData->magics[2]=const_cast<SDL_FRect *> (dstrect);
		rendererData->magics[3]=const_cast<double *> (&angle);
		rendererData->magics[4]=const_cast < SDL_FPoint *>(center);
		rendererData->magics[5]=const_cast<SDL_RendererFlip *> (&flip);
		rendererData->MethodsCount=6;
		SDL_Delay(30);
		//for(int i=0;i<10000;i++)
		//{
		//	Platform::String^ s("wow");
		//	Platform::String^ s1("aa");
		//	Platform::String^ end=s+s1;
		//}
		return 0;
	}
}

static int
D3D11_RenderReadPixels(SDL_Renderer * renderer, const SDL_Rect * rect,
                       Uint32 format, void * pixels, int pitch)
{
    D3DSharedData * data = (D3DSharedData *) renderer->driverdata;
    HRESULT result = S_OK;

    // Retrieve a pointer to the back buffer:
    ComPtr<ID3D11Texture2D> backBuffer;
	backBuffer=data->m_SyncTexture;
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__ ", Get Back Buffer", result);
        return -1;
    }

    // Create a staging texture to copy the screen's data to:
    ComPtr<ID3D11Texture2D> stagingTexture;
    D3D11_TEXTURE2D_DESC stagingTextureDesc;
    backBuffer->GetDesc(&stagingTextureDesc);
    stagingTextureDesc.Width = rect->w;
    stagingTextureDesc.Height = rect->h;
    stagingTextureDesc.BindFlags = 0;
    stagingTextureDesc.MiscFlags = 0;
    stagingTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingTextureDesc.Usage = D3D11_USAGE_STAGING;
    result = data->m_device->CreateTexture2D(
        &stagingTextureDesc,
        NULL,
        &stagingTexture);
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__ ", Create Staging Texture", result);
        return -1;
    }

    // Copy the desired portion of the back buffer to the staging texture:
    D3D11_BOX srcBox;
    srcBox.left = rect->x;
    srcBox.right = rect->x + rect->w;
    srcBox.top = rect->y;
    srcBox.bottom = rect->y + rect->h;
    srcBox.front = 0;
    srcBox.back = 1;
    data->m_context->CopySubresourceRegion(
        stagingTexture.Get(),
        D3D11CalcSubresource(0, 0, 0),
        0, 0, 0,
        backBuffer.Get(),
        D3D11CalcSubresource(0, 0, 0),
        &srcBox);

    // Map the staging texture's data to CPU-accessible memory:
    D3D11_MAPPED_SUBRESOURCE textureMemory = {0};
    result = data->m_context->Map(
        stagingTexture.Get(),
        D3D11CalcSubresource(0, 0, 0),
        D3D11_MAP_READ,
        0,
        &textureMemory);
    if (FAILED(result)) {
        WIN_SetErrorFromHRESULT(__FUNCTION__ ", Map Staging Texture to CPU Memory", result);
        return -1;
    }

    // Copy the data into the desired buffer, converting pixels to the
    // desired format at the same time:
    if (SDL_ConvertPixels(
        rect->w, rect->h,
        DXGIFormatToSDLPixelFormat(stagingTextureDesc.Format),
        textureMemory.pData,
        textureMemory.RowPitch,
        format,
        pixels,
        pitch) != 0)
    {
        // When SDL_ConvertPixels fails, it'll have already set the format.
        // Get the error message, and attach some extra data to it.
        std::string errorMessage = string(__FUNCTION__ ", Convert Pixels failed: ") + SDL_GetError();
        SDL_SetError(errorMessage.c_str());
        return -1;
    }

    // Unmap the texture:
    data->m_context->Unmap(
        stagingTexture.Get(),
        D3D11CalcSubresource(0, 0, 0));

    // All done.  The staging texture will be cleaned up in it's container
    // ComPtr<>'s destructor.
    return 0;
}

static void
D3D11_RenderPresent(SDL_Renderer * renderer)
{
    D3DSharedData *data = (D3DSharedData *) renderer->driverdata;

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    //HRESULT hr = data->swapChain->Present(1, 0);
#else
    // The application may optionally specify "dirty" or "scroll"
    // rects to improve efficiency in certain scenarios.
    // This option is not available on Windows Phone 8, to note.
    DXGI_PRESENT_PARAMETERS parameters = {0};
    parameters.DirtyRectsCount = 0;
    parameters.pDirtyRects = nullptr;
    parameters.pScrollRect = nullptr;
    parameters.pScrollOffset = nullptr;
    
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = data->swapChain->Present1(1, 0, &parameters);
#endif

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    //data->m_dev->DiscardView(data->mainRenderTargetView.Get());

    // If the device was removed either by a disconnect or a driver upgrade, we 
    // must recreate all device resources.
    //
    // TODO, WinRT: consider throwing an exception if D3D11_RenderPresent fails, especially if there is a way to salvedge debug info from users' machines
    //if (hr == DXGI_ERROR_DEVICE_REMOVED)
    //{
    //    hr = D3D11_HandleDeviceLost(renderer);
    //    if (FAILED(hr)) {
    //        WIN_SetErrorFromHRESULT(__FUNCTION__, hr);
    //    }
    //}
    //else
    //{
    //    WIN_SetErrorFromHRESULT(__FUNCTION__, hr);
    //}
}

#endif /* SDL_VIDEO_RENDER_D3D && !SDL_RENDER_DISABLED */
