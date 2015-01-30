//#pragma once
#include <F:\Documents\Open Source Projects\DavidLudwig-sdl-25883bdf3cab\VisualC-WinPhone\SDL\Direct3DBase.h>
struct SDL_WindowData;
struct ModelViewProjectionConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

ref class SDL_WinRTApp sealed : public  Direct3DBase
{
public:
    SDL_WinRTApp();
    
    //// IFrameworkView Methods.
    //virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
    //virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
    //virtual void Load(Platform::String^ entryPoint);
    //virtual void Run();
    //virtual void Uninitialize();
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	
	// 更新时间相关对象的方法。
	void Update(float timeTotal, float timeDelta);

internal:
    // SDL-specific methods
    SDL_DisplayMode GetMainDisplayMode();
    void PumpEvents();
    const SDL_WindowData * GetSDLWindowData() const;
    bool HasSDLWindowData() const;
    void SetRelativeMouseMode(bool enable);
    void SetSDLWindowData(const SDL_WindowData * windowData);
    void SetSDLVideoDevice(const SDL_VideoDevice * videoDevice);
    Windows::Foundation::Point TransformCursor(Windows::Foundation::Point rawPosition);

protected:
    // Event Handlers.
    //void OnOrientationChanged(Platform::Object^ sender);
    //void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
    //void OnLogicalDpiChanged(Platform::Object^ sender);
    //void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
    //void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
    //void OnResuming(Platform::Object^ sender, Platform::Object^ args);
    //void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
    //void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
    //void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
    //void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
    //void OnPointerWheelChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
    //void OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
    //void OnMouseMoved(Windows::Devices::Input::MouseDevice^ mouseDevice, Windows::Devices::Input::MouseEventArgs^ args);
    //void OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
    //void OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);

private:
    bool m_windowClosed;
    bool m_windowVisible;
    const SDL_WindowData* m_sdlWindowData;
    const SDL_VideoDevice* m_sdlVideoDevice;
    bool m_useRelativeMouseMode;
	bool m_loadingComplete;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	uint32 m_indexCount;
	ModelViewProjectionConstantBuffer m_constantBufferData;
};
