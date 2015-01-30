#include "SDL_config.h"
#include <SDL_pixels.h>
#include <D3D11_1.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <agile.h>
#include <../src/render/SDL_sysrender.h>
#include <SDL_render_d3d11_cpp.h>
#include <mutex>
#include  <Direct3DBase.h>
#include <SharedData.h>
namespace SDL_Render
{

 ref class SDLRenderer sealed : public Direct3DBase
	{
	public:
		 SDLRenderer();

	// Direct3DBase 方法。
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	

	void Exit()
	{
		
	}
	// 更新时间相关对象的方法。
	void Update(float timeTotal, float timeDelta);
	internal:
		D3DSharedData SharedData;
		D3DSharedData GetSharedData();
		virtual ID3D11Texture2D* GetTexture() override
		{
				return SharedData.m_SyncTexture.Get();
		}
private:
	bool m_loadingComplete;
	bool IsInitialized;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	bool doOnce;
	};

}
