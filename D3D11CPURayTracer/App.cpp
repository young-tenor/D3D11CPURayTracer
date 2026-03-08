#include "pch.h"
#include "App.h"

bool App::Init(HWND hWnd) {
	this->hWnd = hWnd;

	RECT rc;
	GetClientRect(hWnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	aspect = width / height;

	// device & swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		nullptr,
		&context);
	if (FAILED(hr)) {
		std::cout << "D3D11CreateDeviceAndSwapChain() failed." << std::endl;
		return false;
	}

	// render target view
	ID3D11Texture2D *backBuffer = nullptr;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);
	if (FAILED(hr)) {
		std::cout << "GetBuffer() failed." << std::endl;
		return false;
	}

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
	if (FAILED(hr)) {
		std::cout << "CreateRenderTargetView() failed." << std::endl;
		return false;
	}
	backBuffer->Release();

	// viewport
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *psBlob = nullptr;

	// vertex shader
	hr = D3DCompileFromFile(L"Shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: vertex shader" << std::endl;
		return false;
	}

	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs);
	vsBlob->Release();

	// pixel shader
	hr = D3DCompileFromFile(L"Shaders.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: pixel shader" << std::endl;
		return false;
	}

	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();

	// texture
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateTexture2D(&textureDesc, nullptr, &canvas);
	if (FAILED(hr)) {
		std::cout << "CreateTexture2D() failed." << std::endl;
		return false;
	}

	hr = device->CreateShaderResourceView(canvas, nullptr, &canvasSrv);
	if (FAILED(hr)) {
		std::cout << "CreateShaderResourceView() failed." << std::endl;
		return false;
	}

	canvasData.resize(width * height);

	// sampler
	D3D11_SAMPLER_DESC samplerDesc = { 0 };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr)) {
		std::cout << "CreateSamplerState() failed." << std::endl;
		return false;
	}

	// light
	light = new Light();
	light->strength = 1.0f;
	light->pos = glm::vec3(0.0f, 1.0f, -1.0f);

	// GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);

	return true;
}

void App::Render() {
	CPURender();

	const float clearColor[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(rtv, clearColor);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, nullptr);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vs, nullptr, 0);
	context->PSSetShader(ps, nullptr, 0);

	context->PSSetShaderResources(0, 1, &canvasSrv);
	context->PSSetSamplers(0, 1, &sampler);

	context->Draw(3, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}

// [0, w - 1] * [0, h - 1] -> [-aspect, aspect] * [-1, 1]
glm::vec3 App::ScreenToWorld(const glm::vec3 &pos) {
	const float x = pos.x * 2.0f * aspect / (width - 1) - aspect;
	const float y = pos.y * 2.0f / (height - 1) - 1.0f;
	return glm::vec3(x, -y, 0.0f);
}

glm::vec3 App::TraceRay(const glm::vec3 &pos, const glm::vec3 &dir, const int level) {
	if (level < 0) {
		return glm::vec3(0.0f);
	}

	const Ray ray(pos, dir);
	Hit closestHit;
	float minDist = 100.0f;

	for (const auto &object : objects) {
		const Hit hit = object->Intersect(ray);
		if (hit.d < 0.0f || hit.d > minDist) {
			continue;
		}
		closestHit = hit;
		minDist = hit.d;
	}

	if (!closestHit.obj) {
		if (cubemap) {
			return cubemap->Sample(dir);
		}
		return glm::vec3(0.0f);
	}

	if (expand) {
		closestHit.uv *= 4.0f;
	}

	const auto lightVec = glm::normalize(light->pos - closestHit.pos);
	const auto camVec = glm::normalize(-ray.dir);
	auto color = BlinnPhong(closestHit, lightVec, camVec, light->strength);

	if (closestHit.obj->reflection > 0.0f) {
		const glm::vec3 reflectDir = glm::reflect(ray.dir, closestHit.normal);
		const glm::vec3 reflectedColor = TraceRay(closestHit.pos + reflectDir * 1e-3f, reflectDir, level - 1);
		color = glm::mix(color, reflectedColor, closestHit.obj->reflection);
	}

	if (drawShadow) {
		const Ray shadowRay(closestHit.pos + closestHit.normal * 1e-3f, lightVec);
		bool isShadowed = false;
		for (const auto &object : objects) {
			const Hit hit = object->Intersect(shadowRay);
			if (hit.d < 0.0f) {
				continue;
			}
			isShadowed = true;
			break;
		}
		if (isShadowed) {
			color = closestHit.obj->ambient;
			if (closestHit.obj->texture) {
				color *= closestHit.obj->texture->SamplePoint(closestHit.uv, false);
			}
		}
	}

	return color;
}

glm::vec3 App::TraceRaySuper(const glm::vec3 &pos, const glm::vec3 &dir, const int level) {
	const float dx = 2.0f / height;
	auto color = glm::vec3(0.0f);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			const float offsetX = (i - 0.5f) * 0.5f * dx;
			const float offsetY = (j - 0.5f) * 0.5f * dx;
			const auto rayOrig = glm::vec3(pos.x + offsetX, pos.y + offsetY, pos.z);
			const auto rayDir = glm::normalize(rayOrig - camPos);
			color += TraceRay(rayOrig, rayDir, level);
		}
	}
	color /= 4.0f;

	return color;
}

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
glm::vec3 App::BlinnPhong(const Hit &hit, const glm::vec3 &lightDir, const glm::vec3 &camVec, const float lightStrength) {
	const auto halfway = glm::normalize(lightDir + camVec);

	auto ambient = hit.obj->ambient;
	if (hit.obj->texture) {
		if (linearSampling) {
			ambient *= hit.obj->texture->SampleLinear(hit.uv, wrap);
		} else {
			ambient *= hit.obj->texture->SamplePoint(hit.uv, wrap);
		}
	}
	const auto diffuse = glm::max(glm::dot(hit.normal, lightDir), 0.0f) * hit.obj->diffuse;
	const auto specular = glm::pow(glm::max(glm::dot(hit.normal, halfway), 0.0f), hit.obj->shininess) * hit.obj->specular;
	const auto color = ambient + (diffuse + specular) * lightStrength;

	return color;
}
