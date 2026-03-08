#include "pch.h"
#include "SuperSampling.h"
#include "Sphere.h"

bool SuperSampling::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	// cubemap
	const std::string nx = "./nx.jpg";
	const std::string px = "./px.jpg";
	const std::string ny = "./ny.jpg";
	const std::string py = "./py.jpg";
	const std::string nz = "./nz.jpg";
	const std::string pz = "./pz.jpg";

	cubemap = new Cubemap();
	if (!cubemap->Init(nx, px, ny, py, nz, pz)) {
		std::cout << "cubemap->init() failed" << std::endl;
		return false;
	}

	// object
	Sphere *sphere = new Sphere();
	sphere->radius = 0.5f;
	sphere->center = glm::vec3(0.0f, 0.0f, 0.5f);
	sphere->ambient = glm::vec3(0.1f, 0.02f, 0.02f);
	sphere->diffuse = glm::vec3(0.8f, 0.1f, 0.1f);
	sphere->specular = glm::vec3(1.0f, 0.8f, 0.8f);
	sphere->shininess = 128.0f;
	sphere->reflection = 0.6f;
	objects.push_back(sphere);

	return true;
}

void SuperSampling::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Super Sampling");

	ImGui::Checkbox("super sampling", &superSampling);

	ImGui::End();
}

void SuperSampling::CPURender() {
	const auto clearColor = glm::vec4(0.1f, 0.2f, 0.4f, 1.0f);
	std::fill(canvasData.begin(), canvasData.end(), clearColor);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const auto posWorld = ScreenToWorld(glm::vec3((float)j + 0.5f, (float)i + 0.5f, 0.0f));
			const auto rayDir = glm::normalize(posWorld - camPos);
			if (superSampling) {
				canvasData[i * width + j] = glm::vec4(TraceRaySuper(posWorld, rayDir, 3), 1.0f);
			} else {
				canvasData[i * width + j] = glm::vec4(TraceRay(posWorld, rayDir, 3), 1.0f);
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(canvas, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, canvasData.data(), canvasData.size() * sizeof(glm::vec4));
	context->Unmap(canvas, 0);
}
