#pragma once
#include "Object.h"
#include "Light.h"
#include "Hit.h"
#include "Cubemap.h"

class App {
public:
	virtual ~App() = default;

	virtual bool init(HWND h_wnd);
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	glm::vec3 screen_to_world(const glm::vec3 &pos);
	glm::vec3 trace_ray(const glm::vec3 &pos, const glm::vec3 &dir, const int level = 0);
	glm::vec3 blinn_phong(const Hit &hit, const glm::vec3 &light_dir, const glm::vec3 &cam_dir, const float light_strength);

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

	ID3D11Texture2D *canvas = nullptr;
	ID3D11ShaderResourceView *canvas_srv = nullptr;
	std::vector<glm::vec4> canvas_data;
	ID3D11SamplerState *sampler = nullptr;

	bool draw_shadow = false;
	bool linear_sampling = false;
	bool wrap = false;
	bool expand = false;

	Light *light = nullptr;
	std::vector<Object *> objects;
	Cubemap *cubemap = nullptr;
};