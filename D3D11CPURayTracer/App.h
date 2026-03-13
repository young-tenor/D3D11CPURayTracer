#pragma once
#include "Object.h"
#include "Light.h"
#include "Hit.h"
#include "Cubemap.h"

class App {
public:
	virtual ~App() = default;

	virtual bool Init(HWND hWnd);
	virtual void Update() = 0;
	void Render();

protected:
	virtual void CPURender() = 0;
	glm::vec3 ScreenToWorld(const glm::vec3 &pos);
	glm::vec3 TraceRay(const glm::vec3 &pos, const glm::vec3 &dir, const int level = 0);
	glm::vec3 TraceRaySuper(const glm::vec3 &pos, const glm::vec3 &dir, const int level = 0);
	glm::vec3 BlinnPhong(const Hit &hit, const glm::vec3 &lightDir, const glm::vec3 &toEye, const float lightStrength);

protected:
	HWND hWnd = nullptr;
	float width = 1280.0f;
	float height = 720.0f;
	float aspect = 1280.0f / 720.0f;

	ID3D11Device *device = nullptr;
	ID3D11DeviceContext *context = nullptr;
	IDXGISwapChain *swapChain = nullptr;
	ID3D11RenderTargetView *rtv = nullptr;
	D3D11_VIEWPORT viewport = { 0 };

	ID3D11VertexShader *vs = nullptr;
	ID3D11PixelShader *ps = nullptr;

	ID3D11Texture2D *canvas = nullptr;
	ID3D11ShaderResourceView *canvasSrv = nullptr;
	std::vector<glm::vec4> canvasData;
	ID3D11SamplerState *sampler = nullptr;

	bool drawShadow = false;
	bool linearSampling = false;
	bool wrap = false;
	bool expand = false;
	bool superSampling = false;

	Light *light = nullptr;
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -1.0f);
	std::vector<Object *> objects;
	Cubemap *cubemap = nullptr;
};