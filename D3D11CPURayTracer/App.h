#pragma once
#include <directxtk/SimpleMath.h>
#include "Circle.h";
#include "Light.h";
#include "Sphere.h";

class App {
public:
	bool init(HWND h_wnd);
	void update();
	void render();

private:
	DirectX::SimpleMath::Vector2 screen_to_world(DirectX::SimpleMath::Vector2 pos);
	DirectX::SimpleMath::Vector3 screen_to_world(DirectX::SimpleMath::Vector3 pos);

private:
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

	Light *light = nullptr;

	Circle *circle = nullptr;
	Sphere *sphere = nullptr;
};