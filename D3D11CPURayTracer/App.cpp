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

	// light
	light = new Light(1.0f, glm::vec3(0.0f, 1.0f, -1.0f));

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
glm::vec3 App::screen_to_world(const glm::vec3 &pos) {
	auto x = pos.x * 2.0f * aspect / (width - 1) - aspect;
	auto y = pos.y * 2.0f / (height - 1) - 1.0f;
	return glm::vec3(x, -y, 0.0f);
}

glm::vec3 App::trace_ray(const glm::vec3 &pos, const glm::vec3 &dir, const int level) {
	if (level < 0) {
		return glm::vec3(0.0f);
	}

	Ray ray(pos, dir);
	Hit closest_hit(-1.0f, glm::vec3(0.0f, 0.0f, -1.0f));
	float min_d = 100.0f;

	for (auto &object : objects) {
		Hit hit = object->intersect(ray);
		if (hit.d < 0.0f || hit.d > min_d) {
			continue;
		}
		closest_hit = hit;
		min_d = hit.d;
	}

	if (!closest_hit.obj) {
		if (cubemap) {
			return cubemap->sample(dir);
		}
		return glm::vec3(0.0f);
	}

	if (expand) {
		closest_hit.uv *= 4.0f;
	}

	auto light_vec = glm::normalize(light->pos - closest_hit.pos);
	auto cam_dir = glm::normalize(-ray.dir);
	auto color = blinn_phong(closest_hit, light_vec, cam_dir, light->strength);

	if (closest_hit.obj->reflection > 0.0f) {
		glm::vec3 reflect_dir = glm::reflect(ray.dir, closest_hit.normal);
		glm::vec3 reflected_color = trace_ray(closest_hit.pos + reflect_dir * 1e-3f, reflect_dir, level - 1);
		color = glm::mix(color, reflected_color, closest_hit.obj->reflection);
	}

	if (draw_shadow) {
		Ray shadow_ray(closest_hit.pos + closest_hit.normal * 1e-3f, light_vec);
		bool is_shadowed = false;
		for (auto &object : objects) {
			Hit hit = object->intersect(shadow_ray);
			if (hit.d < 0.0f) {
				continue;
			}
			is_shadowed = true;
			break;
		}
		if (is_shadowed) {
			color = closest_hit.obj->ambient;
			if (closest_hit.obj->texture) {
				color *= closest_hit.obj->texture->sample_point(closest_hit.uv, false);
			}
		}
	}

	return color;
}

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
glm::vec3 App::blinn_phong(const Hit &hit, const glm::vec3 &light_dir, const glm::vec3 &cam_dir, const float light_strength) {
	auto halfway = glm::normalize(light_dir + cam_dir);

	auto ambient = hit.obj->ambient;
	if (hit.obj->texture) {
		if (linear_sampling) {
			ambient *= hit.obj->texture->sample_linear(hit.uv, wrap);
		} else {
			ambient *= hit.obj->texture->sample_point(hit.uv, wrap);
		}
	}
	auto diffuse = glm::max(glm::dot(hit.normal, light_dir), 0.0f) * hit.obj->diffuse;
	auto specular = glm::pow(glm::max(glm::dot(hit.normal, halfway), 0.0f), hit.obj->shininess) * hit.obj->specular;
	auto color = ambient + (diffuse + specular) * light_strength;

	return color;
}
