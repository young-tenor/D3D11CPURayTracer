#pragma once
#include <directxtk/SimpleMath.h>

class App {
public:
	virtual ~App() = default;

	virtual bool init(HWND h_wnd);
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	DirectX::SimpleMath::Vector3 screen_to_world(DirectX::SimpleMath::Vector3 pos);

protected:
	HWND h_wnd = nullptr;
	float width = 1280.0f;
	float height = 720.0f;
	float aspect = 1280.0f / 720.0f;

	ID3D11Device *device = nullptr;
	ID3D11DeviceContext *context = nullptr;
	IDXGISwapChain *swap_chain = nullptr;
	ID3D11RenderTargetView *rtv = nullptr;
	D3D11_VIEWPORT viewport = { 0 };

	ID3D11VertexShader *vs = nullptr;
	ID3D11PixelShader *ps = nullptr;

	ID3D11Texture2D *texture = nullptr;
	ID3D11ShaderResourceView *texture_srv = nullptr;
	std::vector<DirectX::SimpleMath::Vector4> texture_data;
	ID3D11SamplerState *sampler = nullptr;
};