#include "SDL_D3D11_STDINC.h"
#include <agile.h>
#include <queue>
#ifndef SHAREDDATA
#define SHAREDDATA
#ifndef RENDER_D3D11
struct SDL_VertexShaderConstants
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
};
struct ModelViewProjectionConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};
struct Vertex	//Overloaded Vertex Structure
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v)
		: pos(x,y,z), texCoord(u, v){}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};
#endif
typedef struct 
{
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_SyncTexture;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mainRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> currentOffscreenRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> texturePixelShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> colorPixelShader;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeBlend;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeAdd;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeMod;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> mainSampler;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mainRasterizer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backbuffer;
    D3D_FEATURE_LEVEL featureLevel;

    // Vertex buffer constants:
    SDL_VertexShaderConstants vertexShaderConstantsData;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexShaderConstants;

    // Cached renderer properties.
    DirectX::XMFLOAT2 windowSizeInDIPs;
    DirectX::XMFLOAT2 renderTargetSize;
    Windows::Graphics::Display::DisplayOrientations orientation;
	//queue<void **> params;
	//queue<UINT> paramCount;
	//queue<char> operations;
	//queue<char > callerID;
	const void ** constmagics;
	void*  magics[20];
	UINT magicCount;
	SDL_Renderer* renderer;
	int  MethodsCount;
    // Transform used for display orientation.
    DirectX::XMFLOAT4X4 orientationTransform3D;
}D3DSharedData;

extern D3DSharedData GlobalSharedData;
#endif