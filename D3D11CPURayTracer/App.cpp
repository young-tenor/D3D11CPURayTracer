#include "pch.h"
#include "App.h"

bool App::init(HWND h_wnd) {
	this->h_wnd = h_wnd;

	RECT rc;
	GetClientRect(h_wnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	aspect = width / height;

	// device & swap chain
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = h_wnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;

	D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		feature_levels,
		1,
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&swap_chain,
		&device,
		nullptr,
		&context);
	if (FAILED(hr)) {
		std::cout << "D3D11CreateDeviceAndSwapChain() failed." << std::endl;
		return false;
	}

	// render target view
	ID3D11Texture2D *back_buffer = nullptr;
	hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&back_buffer);
	if (FAILED(hr)) {
		std::cout << "GetBuffer() failed." << std::endl;
		return false;
	}

	hr = device->CreateRenderTargetView(back_buffer, nullptr, &rtv);
	if (FAILED(hr)) {
		std::cout << "CreateRenderTargetView() failed." << std::endl;
		return false;
	}
	back_buffer->Release();

	// viewport
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	ID3DBlob *vs_blob = nullptr;
	ID3DBlob *ps_blob = nullptr;

	// vertex shader
	hr = D3DCompileFromFile(L"Shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vs_blob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: vertex shader" << std::endl;
		return false;
	}

	hr = device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vs);
	vs_blob->Release();

	// pixel shader
	hr = D3DCompileFromFile(L"Shaders.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &ps_blob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: pixel shader" << std::endl;
		return false;
	}

	// texture
	hr = device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &ps);
	ps_blob->Release();

	D3D11_TEXTURE2D_DESC texture_desc = { 0 };
	texture_desc.Width = width;
	texture_desc.Height = height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.Usage = D3D11_USAGE_DYNAMIC;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateTexture2D(&texture_desc, nullptr, &canvas);
	if (FAILED(hr)) {
		std::cout << "CreateTexture2D() failed." << std::endl;
		return false;
	}

	hr = device->CreateShaderResourceView(canvas, nullptr, &canvas_srv);
	if (FAILED(hr)) {
		std::cout << "CreateShaderResourceView() failed." << std::endl;
		return false;
	}

	canvas_data.resize(width * height);

	// sampler
	D3D11_SAMPLER_DESC sampler_desc = { 0 };
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampler_desc, &sampler);
	if (FAILED(hr)) {
		std::cout << "CreateSamplerState() failed." << std::endl;
		return false;
	}

	// GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(h_wnd);
	ImGui_ImplDX11_Init(device, context);

	return true;
}

// [0, w - 1] * [0, h - 1] -> [-aspect, aspect] * [-1, 1]
DirectX::SimpleMath::Vector3 App::screen_to_world(DirectX::SimpleMath::Vector3 pos) {
	float x = pos.x * 2.0f * aspect / (width - 1) - aspect;
	float y = pos.y * 2.0f / (height - 1) - 1.0f;
	return { x, -y, 0.0f };
}

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
DirectX::SimpleMath::Vector3 App::blinn_phong(
	DirectX::SimpleMath::Vector3 normal,
	DirectX::SimpleMath::Vector3 light_dir,
	DirectX::SimpleMath::Vector3 cam_dir,
	float light_strength,
	Object *object) {
	auto halfway = light_dir + cam_dir;
	halfway.Normalize();

	auto ambient = object->ambient;
	auto diffuse = std::max(normal.Dot(light_dir), 0.0f) * object->diffuse;
	auto specular = std::pow(std::max(normal.Dot(halfway), 0.0f), object->shininess) * object->specular;
	auto color = ambient + (diffuse + specular) * light_strength;

	return color;
}
